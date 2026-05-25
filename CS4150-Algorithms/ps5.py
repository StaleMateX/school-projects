import heapq
import sys

# Constants
INF = float('inf')


# Function to perform Dijkstra's algorithm with a limit on edge length
def dijkstra_with_limit(n, adj, max_edge_length):
    dist = [INF] * n
    dist[0] = 0
    priority_queue = [(0.0, 0)]  # (distance, node)

    while priority_queue:
        d, u = heapq.heappop(priority_queue)

        if d > dist[u]:
            continue

        for v, k in adj[u]:
            if k <= max_edge_length and dist[u] + k < dist[v]:
                dist[v] = dist[u] + k
                heapq.heappush(priority_queue, (dist[v], v))

    return dist[n - 1]


# Main function to find the minimum possible maximum highway length
def min_max_highway_length(n, m, X, highways):
    # Build the adjacency list for the graph
    adj = [[] for _ in range(n)]
    for u, v, k in highways:
        adj[u - 1].append((v - 1, k))
        adj[v - 1].append((u - 1, k))

    # Get the initial shortest path without edge limit
    initial_dist = dijkstra_with_limit(n, adj, INF)
    if initial_dist == INF:
        return -1  # No path exists

    # Calculate the maximum allowed path distance
    max_allowed_distance = initial_dist * (1 + X / 100.0)

    # Binary search on the maximum edge length
    low, high = 1, max(k for _, _, k in highways)
    result = high

    while low <= high:
        mid = (low + high) // 2

        # Run Dijkstra's with max_edge_length set to 'mid'
        dist_with_mid = dijkstra_with_limit(n, adj, mid)

        # Check if we can reach city N within the allowed path length
        if dist_with_mid <= max_allowed_distance:
            result = mid  # This edge length works, try smaller
            high = mid - 1
        else:
            low = mid + 1  # This edge length doesn't work, increase it

    return result


# Reading input in the format expected by competitive platforms (no input prompts)
if __name__ == "__main__":
    input = sys.stdin.read
    data = input().split()

    n, m, X = int(data[0]), int(data[1]), int(data[2])
    highways = []
    idx = 3
    for _ in range(m):
        u, v, k = int(data[idx]), int(data[idx + 1]), int(data[idx + 2])
        highways.append((u, v, k))
        idx += 3

    # Call the function with the parsed input
    result = min_max_highway_length(n, m, X, highways)
    print(result)  # Output the result
