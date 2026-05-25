# Makefile
# by Jacob Xu

# Compiler Flags
CC = g++
# Compiler flags
CFLAGS = -Wall -fsanitize=address -fsanitize=undefined

# Executable target
TARGET = trieTest

# Default target
all: $(TARGET)

# Rule to build trieTest executable
$(TARGET): trie.o trieTest.o
	$(CC) $(CFLAGS) -o $(TARGET) trie.o trieTest.o

# Rule to build trie.o object file
trie.o: trie.cpp trie.h
	$(CC) $(CFLAGS) -c trie.cpp

# Rule to build trieTest.o object file
trieTest.o: trieTest.cpp trie.h
	$(CC) $(CFLAGS) -c trieTest.cpp

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) trie.o trieTest.o

# Test target to run trieTest with needed files
test: $(TARGET)
	./$(TARGET) words.txt queries.txt

.PHONY: all clean test