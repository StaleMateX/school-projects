# Makefile for fernPNG by Jacob Xu
# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall

# Libpng path
LIBPNG = cs3505/libpng-1.6.40

# Target: fernPNG (executable)
all: fernPNG

# Rule to build fernPNG
fernPNG: fernPNG.o transform.o point.o pngWriter.o
	$(CC) $(CFLAGS) -o fernPNG -L$(LIBPNG) fernPNG.o transform.o point.o pngWriter.o -lpng

# Rule to build fernPNG.o (object file for fernPNG.cpp)
fernPNG.o: fernPNG.cpp transform.h point.h pngWriter.h
	$(CC) $(CFLAGS) -I$(LIBPNG) -c fernPNG.cpp 

transform.o: transform.cpp transform.h point.h
	$(CC) $(CFLAGS) -I$(LIBPNG) -c transform.cpp

point.o: point.cpp point.h
	$(CC) $(CFLAGS) -I$(LIBPNG) -c point.cpp 

pngWriter.o: pngWriter.cpp pngWriter.h
	$(CC) $(CFLAGS) -I$(LIBPNG) -c pngWriter.cpp 

# Clean rule to remove generated files
clean:
	rm -f fernPNG *.o