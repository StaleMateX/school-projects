#Authors: Joshua Brody, Jacob Xu
#CS3505 Assignment 5, 2/15/2024

# Define the directory where googletest is located
GOOGLETEST ?= ./googletest

# Include directories for googletest
GTEST_INCLUDE_DIR = $(GOOGLETEST)/googletest/include
GTEST_LIB_DIR = $(GOOGLETEST)/lib

# Define compiler and flags
CPPFLAGS = -std=c++17 -Wall -fsanitize=address -fsanitize=undefined -I$(GTEST_INCLUDE_DIR) -I.
LDFLAGS = -L$(GTEST_LIB_DIR) -lgtest_main -lgtest -pthread

# Target for the test executable
trieTests: trieTests.cpp trie.o
	g++ $(CPPFLAGS) $^ $(LDFLAGS) -o $@

# Target to run the tests
test: trieTests
	./trieTests

# Phony target to ensure test is always run
.PHONY: test

# Target to clean the directory
clean:
	rm -f *.o trieTests