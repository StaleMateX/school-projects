#ifndef TRIE_H
#define TRIE_H
/**
 * trie interface
 * by Jacob Xu
 */
#include <string>
#include <vector>
#include <iostream>
using namespace std;
/// @brief Your job is to make a Trie class. The Trie class should have a root node, where nodes have internal storage for branches of the Trie and a flag determining whether or not that node represents the end of a valid word. The class needs the following public interface:
class Trie
{
    /// @brief a Node helper class for the Trie
    struct Node
    {
        // Our alphabet will be the set of 26 lower-case English letters, 'a' to 'z'.
        // My suggestion is to make this array an instance variable of the class. (from module guide)
        Node *letters[26];
        // word flag, is it the end of word
        bool isEOW;
        // make sure to initialize all pointers to nullptr so that they are not pointing to garbage memory locations
        Node() : isEOW(false)
        {
            for (int i = 0; i < 26; i++)
            {
                letters[i] = nullptr;
            }
        }
    };
    /// @brief Root node of the Trie
    Node *root;
    /// @brief Helper method to recursively delete nodes
    /// @param node node
    void deleteNode(Node *node);
    /// @brief Helper method to convert character to index in letters array
    /// @param c chars
    /// @return index of that char
    int charToIndex(char c)
    {
        // by subtracting the ASCII value of 'a' from the ASCII value of the input character c,
        // this method maps each lowercase letter to a unique index in the range [0, 25],
        // which can be used to access the appropriate branch or node in the trie array.
        if (c >= 'a' && c <= 'z')
        {
            return c - 'a';
        }
        else
        {
            // Handle invalid characters here
            return 0;
        }
    }
    /// @brief Helper method to collect words recursively
    /// @param node node
    /// @param prefix the prefix
    /// @param words vector string words
    void collectWords(Node *node, string &prefix, vector<string> &words)
    {
        if (!node)
            return;
        if (node->isEOW)
        {
            // Add word to the vector if it's the end of a word
            words.push_back(prefix);
        }
        for (int i = 0; i < 26; ++i)
        {
            if (node->letters[i])
            {
                // Recursively collect words for each branch
                prefix.push_back('a' + i);
                collectWords(node->letters[i], prefix, words);
                prefix.pop_back();
            }
        }
    }
    /// @brief Helper function to recursively copy nodes
    /// @param destination dest node
    /// @param source source node
    void copyNodes(Node *destination, Node *source)
    {
        if (!source)
            return; // Base case

        // Copy the state of the source node to the destination node
        destination->isEOW = source->isEOW;
        for (int i = 0; i < 26; ++i)
        {
            if (source->letters[i])
            {
                // Create a new node in the destination Trie and copy the contents of the corresponding node in the source Trie
                destination->letters[i] = new Node();
                copyNodes(destination->letters[i], source->letters[i]);
            }
        }
    }

public:
    /// @brief A default constructor
    Trie();
    /// @brief A destructor
    ~Trie();
    /// @brief copy constructor
    /// @param other other trie
    Trie(const Trie &other);
    /// @brief an assignment= operator using the approaches from the Rule-Of-Three slides.
    /// @param other other trie
    /// @return copy trie
    Trie &operator=(Trie &other);
    /// @brief The word passed into the method should be added to the Trie object. Duplicate adds should not change the Trie.
    /// @param word the word that need to add
    void addWord(string word);
    /// @brief If the word is found in the Trie, the method should return true, otherwise return false.
    /// @param word the word that need to check
    /// @return return true if it is a word
    bool isWord(string word);
    /// @brief If the prefix is a word, that should be included. An empty prefix should return all words in the Trie.
    /// @param word the word that need to be check
    /// @return vector string words
    vector<string> allWordsStartingWithPrefix(string word);
};

#endif