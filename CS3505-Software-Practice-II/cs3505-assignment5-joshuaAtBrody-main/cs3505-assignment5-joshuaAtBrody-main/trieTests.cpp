/*
Authors: Joshua Brody, Jacob Xu
CS3505 Assignment 5, 2/15/2024
*/
#include <cassert>
#include <gtest/gtest.h>
#include"trie.h"

// Test case to verify basic functionality of addWord and isWord methods
TEST(TrieTest, BasicFunctionality) {
    Trie trie;
    // Add words to the trie
    trie.addWord("hello");
    trie.addWord("world");

    // Check if words are present in the trie
    EXPECT_TRUE(trie.isWord("hello"));
    EXPECT_TRUE(trie.isWord("world"));
    EXPECT_FALSE(trie.isWord("foo"));
}

// Test case to verify behavior of empty trie
TEST(TrieTest, EmptyTrie) {
    Trie trie;
    // Verify that empty trie does not contain any words
    EXPECT_FALSE(trie.isWord("hello"));
}

// Test case to verify add same words
TEST(TrieTest, CheckDuplicate) {
    Trie trie;
    // Add words to the trie
    trie.addWord("hello");
    trie.addWord("hello");
    std::vector<std::string> wordsWithPrefix = trie.allWordsStartingWithPrefix("hello");

    // Check duplicate word
    EXPECT_EQ(1, wordsWithPrefix.size());
}

// Test case to verify behavior of prefix search with non-existent prefixes
TEST(TrieTest, NonExistentPrefix) {
    Trie trie;
    // Add words to the trie
    trie.addWord("hello");
    trie.addWord("world");

    // Check prefix search with a non-existent prefix
    std::vector<std::string> wordsWithPrefix = trie.allWordsStartingWithPrefix("foo");

    // Check if no words are returned for a non-existent prefix
    EXPECT_TRUE(wordsWithPrefix.empty());
}

// Test case to verify behavior with words starting with a common prefix
TEST(TrieTest, WordsWithCommonPrefix) {
    Trie trie;
    // Add words with common prefix to the trie
    trie.addWord("apple");
    trie.addWord("app");
    trie.addWord("application");

    // Check words with common prefix
    EXPECT_TRUE(trie.isWord("apple"));
    EXPECT_TRUE(trie.isWord("app"));
    EXPECT_TRUE(trie.isWord("application"));
}

// Test case to verify behavior of allWordsStartingWithPrefix method
TEST(TrieTest, AllWordsStartingWithPrefix) {
    Trie trie;
    // Add words to the trie
    trie.addWord("apple");
    trie.addWord("app");
    trie.addWord("application");

    // Get words starting with prefix "app"
    std::vector<std::string> wordsWithPrefix = trie.allWordsStartingWithPrefix("app");

    // Debugging output to print the words returned by the method
    std::cout << "Words starting with prefix 'app': ";
    for (const auto& word : wordsWithPrefix) {
        std::cout << word << " ";
    }
    std::cout << std::endl;

    // Check if the correct words are returned
    EXPECT_EQ(3, wordsWithPrefix.size());
    EXPECT_TRUE(std::find(wordsWithPrefix.begin(), wordsWithPrefix.end(), "app") != wordsWithPrefix.end());
    EXPECT_TRUE(std::find(wordsWithPrefix.begin(), wordsWithPrefix.end(), "application") != wordsWithPrefix.end());
}


// Test case to verify copy constructor behavior
TEST(TrieTest, CopyConstructor) {
    Trie trie;
    // Add words to the original trie
    trie.addWord("hello");
    trie.addWord("world");

    // Create a new trie using copy constructor
    Trie copiedTrie(trie);

    // Check if the copied trie contains the same words
    EXPECT_TRUE(copiedTrie.isWord("hello"));
    EXPECT_TRUE(copiedTrie.isWord("world"));

    // Modify the original trie
    trie.addWord("foo");

    // Check if modification in original trie affects copied trie
    EXPECT_FALSE(copiedTrie.isWord("foo"));
}

// Test case to verify copy assignment operator behavior
TEST(TrieTest, CopyAssignment) {
    Trie trie;
    
    // Add words with common prefix to the trie
    trie.addWord("apple");
    trie.addWord("app");
    trie.addWord("application");

    Trie trie2;
    trie2 = trie;
    // Check words with number
    EXPECT_TRUE(trie2.isWord("apple"));
    EXPECT_TRUE(trie2.isWord("app"));
    EXPECT_TRUE(trie2.isWord("application"));
}

// Test case to verify trie copying and assignment with empty trie
TEST(TrieTest, CopyAssignmentEmptyTrie) {
    Trie trie1;
    Trie trie2;

    // Copy assignment from an empty trie
    trie2 = trie1;

    // Verify that the copied trie is empty
    EXPECT_FALSE(trie2.isWord("hello"));
}

// Main function to run all the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


