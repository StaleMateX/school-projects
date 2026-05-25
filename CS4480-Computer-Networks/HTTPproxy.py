import signal
from optparse import OptionParser
import sys
import socket
import threading
import time
from urllib.parse import urlparse

# Global variables
CACHE_ENABLED = False  # Flag to enable/disable caching
BLOCKLIST_ENABLED = False  # Flag to enable/disable blocklist
CACHE = {}  # Cache dictionary to store cached responses
BLOCKLIST = set()  # Set to store URLs in the blocklist

# Signal handler for Ctrl+C
def ctrl_c_pressed(signal, frame):
    sys.exit(0)

# Function to handle incoming client requests
def handle_client_request(client_socket):
    """
    Handle an incoming client request.

    Parameters:
    - client_socket (socket): Client socket for communication.
    """
    client_socket.settimeout(10)  # Set a timeout of 10 seconds
    
    # Receive and parse the HTTP request from the client
    request_data = b''
    try:
        while True:
            chunk = client_socket.recv(4096)
            if not chunk:
                break
            request_data += chunk
            if b'\r\n\r\n' in request_data:
                break
    except socket.timeout:
        print("Socket operation timed out")
        client_socket.close()
        return
    
    if not request_data:
        return
    
    # Extract the requested URL from the request
    request_lines = request_data.split(b'\r\n')
    first_line_tokens = request_lines[0].split()
    
    # Check for a malformed request line
    if len(first_line_tokens) < 3:
        client_socket.send(b'HTTP/1.0 400 Bad Request\r\n\r\n')
        client_socket.close()  # Close socket
        return
    
    method = first_line_tokens[0].decode()
    if method != 'GET':
        client_socket.send(b'HTTP/1.0 501 Not Implemented\r\n\r\n')
        client_socket.close()  # Close socket
        return
    
    url = first_line_tokens[1].decode()
    http_version = first_line_tokens[2].decode()
    
    # Check HTTP version
    if http_version != 'HTTP/1.0':
        client_socket.send(b'HTTP/1.0 400 Bad Request\r\n\r\n')
        client_socket.close()  # Close socket
        return
    
    # Ensure 'Connection: close' header is included
    if not any(b'Connection: close' in line for line in request_lines[1:]):
        request_data = request_data.replace(b'\r\n\r\n', b'\r\nConnection: close\r\n\r\n', 1)
    
    # Check if the request is a control interface request
    if url.startswith('/proxy/'):
        handle_control_interface(client_socket, url)
        return  # Return immediately after handling control interface request
    
    # Check if caching is enabled and the object is in the cache
    if CACHE_ENABLED and url in CACHE:
        # Check if the cached object is still valid
        last_modified = CACHE[url]['last_modified']
        expires = CACHE[url].get('expires', None)
        if expires is not None and time.time() < expires:
            # Object is still valid, return cached response
            client_socket.sendall(CACHE[url]['data'])
            client_socket.close()
            return
        
        # Object may be stale, send conditional GET request to origin server
        if 'If-Modified-Since' not in request_data.decode():
            # Include If-Modified-Since header
            request_data += f'If-Modified-Since: {last_modified}\r\n'.encode()
    
    # Connect to the remote server specified in the URL
    remote_host, port = parse_url(url)
    if remote_host is None or port is None:
        client_socket.send(b'HTTP/1.0 400 Bad Request\r\n\r\n')
        client_socket.close()  # Close socket
        return
    
    remote_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        remote_socket.connect((remote_host, port))
    except Exception as e:
        print(f"Error connecting to remote server: {e}")
        client_socket.send(b'HTTP/1.0 502 Bad Gateway\r\n\r\n')
        client_socket.close()  # Close socket
        return
    
    # Forward the HTTP request to the remote server
    remote_socket.sendall(request_data)
    
    # Receive the response from the remote server
    response_data = b''
    while True:
        chunk = remote_socket.recv(4096)
        if not chunk:
            break
        response_data += chunk
    
    # Check if the response is 304 Not Modified
    if response_data.startswith(b'HTTP/1.0 304'):
        # Retrieve the cached response and send it back to the client
        cached_response = CACHE[url]['data']
        client_socket.sendall(cached_response)
    else:
        # Check if caching is enabled and the response is cacheable
        if CACHE_ENABLED and response_data.startswith(b'HTTP/1.0 200') and b'no-cache' not in response_data:
            # Parse Last-Modified and Expires headers to determine cache validity
            last_modified = time.strftime("%a, %d %b %Y %H:%M:%S %Z", time.gmtime())
            expires = None
            for header in response_data.split(b'\r\n'):
                header_tokens = header.split(b':')
                if len(header_tokens) == 2:
                    header_name = header_tokens[0].strip().decode().lower()
                    header_value = header_tokens[1].strip().decode()
                    if header_name == 'last-modified':
                        last_modified = header_value
                    elif header_name == 'expires':
                        expires = time.mktime(time.strptime(header_value, "%a, %d %b %Y %H:%M:%S %Z"))
            
            # Update cache
            CACHE[url] = {'data': response_data, 'last_modified': last_modified, 'expires': expires}
        
        # Forward the response back to the client
        client_socket.sendall(response_data)
    
    # Close sockets
    remote_socket.close()
    client_socket.close()

# Function to parse URL and extract host and port
def parse_url(url):
    """
    Parse the URL to extract the host and port.

    Parameters:
    - url (str): The URL to parse.

    Returns:
    - str: Hostname extracted from the URL.
    - int: Port number extracted from the URL.
    """
    parsed_url = urlparse(url)
    if not parsed_url.scheme or not parsed_url.netloc:
        return None, None
    
    host = parsed_url.netloc.split(':')[0]
    port = parsed_url.port if parsed_url.port else 80
    
    return host, port

# Function to handle client connections
def client_connection_handler(server_socket):
    """
    Handle incoming client connections.

    Parameters:
    - server_socket (socket): The server socket to accept incoming connections.
    """
    while True:
        # Accept incoming connection
        client_socket, client_address = server_socket.accept()
        client_socket.settimeout(10)  # Set a timeout of 10 seconds
        
        print(f"Received connection from {client_address}")
        
        # Handle client request in a new thread
        client_thread = threading.Thread(target=handle_client_request, args=(client_socket,))
        client_thread.start()

# Function to handle control interface requests
def handle_control_interface(client_socket, request_path):
    """
    Handle control interface requests.

    Parameters:
    - client_socket (socket): Client socket for communication.
    - request_path (str): Path of the control interface request.
    """
    global CACHE_ENABLED, BLOCKLIST_ENABLED, CACHE, BLOCKLIST
    
    # Process control interface requests
    if request_path == '/proxy/cache/enable':
        CACHE_ENABLED = True
    elif request_path == '/proxy/cache/disable':
        CACHE_ENABLED = False
    elif request_path == '/proxy/cache/flush':
        CACHE.clear()
    elif request_path == '/proxy/blocklist/enable':
        BLOCKLIST_ENABLED = True
    elif request_path == '/proxy/blocklist/disable':
        BLOCKLIST_ENABLED = False
    elif request_path.startswith('/proxy/blocklist/add/'):
        string_to_block = request_path.split('/')[-1]
        BLOCKLIST.add(string_to_block)
    elif request_path.startswith('/proxy/blocklist/remove/'):
        string_to_unblock = request_path.split('/')[-1]
        BLOCKLIST.discard(string_to_unblock)
    elif request_path == '/proxy/blocklist/flush':
        BLOCKLIST.clear()
    
    # Send '200 OK' response to acknowledge the request
    client_socket.sendall(b'HTTP/1.0 200 OK\r\n\r\n')

# Start of program execution
parser = OptionParser()
parser.add_option('-p', type='int', dest='serverPort')
parser.add_option('-a', type='string', dest='serverAddress')
(options, args) = parser.parse_args()

port = options.serverPort
address = options.serverAddress
if address is None:
    address = 'localhost'
if port is None:
    port = 2100

# Set up signal handling (Ctrl+C)
signal.signal(signal.SIGINT, ctrl_c_pressed)

# Create a TCP socket to listen for incoming connections
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((address, port))
server_socket.listen(5)
print(f"Proxy server is listening on {address}:{port}...")

# Start a thread to handle each client connection
client_connection_handler(server_socket)
