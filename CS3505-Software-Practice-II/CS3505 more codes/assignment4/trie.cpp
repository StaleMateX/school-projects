#include "trie.h"
/**
 * trie.cpp
 * by Jacob Xu
 */
#include <iostream>

/// @brief A default constructor
Trie::Trie()
{
    root = new Node();
}
/// @brief A destructor
Trie::~Trie()
{
    deleteNode(root);
}
/// @brief Recursively delete nodes
/// @param node
void Trie::deleteNode(Node *node)
{
    // loop through entire array
    if (!node)
        return;
    for (int i = 0; i < 26; ++i)
    {
        deleteNode(node->letters[i]);
    }
    delete node;
}
/// @brief copy constructor
/// @param other other trie
Trie::Trie(const Trie &other)
{
    // Generate new root node.
    root = new Node();
    // Recursively copy the nodes from the other Trie
    copyNodes(root, other.root);
}
/// @brief an assignment= operator using the approaches from the Rule-Of-Three slides.
/// @param other other trie
/// @return copy trie
Trie &Trie::operator=(Trie &other)
{
    if (this != &other)
    {
        // delete the root node
        deleteNode(root);
        // Generate new root node.
        root = new Node();
        // Copy all the input value into (this) Trie.
        copyNodes(root, other.root);
    }
    // Return the new copied Trie
    return *this;
}
/// @brief The word passed into the method should be added to the Trie object. Duplicate adds should not change the Trie.
/// @param word the word that need to add
void Trie::addWord(string word)
{
    Node *current = root;
    for (char c : word)
    {
        int index = charToIndex(c);
        if (!current->letters[index])
        {
            current->letters[index] = new Node();
        }
        current = current->letters[index];
    }
    current->isEOW = true;
}
/// @brief If the word is found in the Trie, the method should return true, otherwise return false.
/// @param word the word that need to check
/// @return return true if it is a word
bool Trie::isWord(string word)
{
    Node *current = root;
    for (char c : word)
    {
        int index = charToIndex(c);
        if (!current->letters[index])
        {
            return false; // Prefix not found
        }
        current = current->letters[index];
    }
    return current && current->isEOW; // Check if it's a complete word
}

/// @brief If the prefix is a word, that should be included. An empty prefix should return all words in the Trie.
/// @param word the word that need to be check
/// @return vector string words
vector<string> Trie::allWordsStartingWithPrefix(string prefix)
{
    vector<string> words;
    Node *current = root;
    for (char c : prefix)
    {
        int index = charToIndex(c);
        if (!current->letters[index])
        {
            return words; // Prefix not found
        }
        current = current->letters[index];
    }
    std::string prefixCopy = prefix;
    collectWords(current, prefixCopy, words);
    return words;
}