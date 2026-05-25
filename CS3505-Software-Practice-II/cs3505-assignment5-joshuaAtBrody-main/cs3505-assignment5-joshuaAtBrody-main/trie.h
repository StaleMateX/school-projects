/*
    By Joshua Brody u1351827, Jacob Xu u1448572
    CS3505 Assignment 5, 2/15/2024
*/

#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

/// @brief A class for storing words by connection their letters with Nodes using a trie. 
class Trie{
    private:

    /// @brief A private Node struct to act as the tree/trie and checks if the current node is the end of some word.
    struct Node {
        std::map<char, Node> branches;
        bool isEndOfWord;

        /// @brief constructor for Node class
        Node() : isEndOfWord(false) {}

        /// @brief Destructor for Node class
        ~Node() {}
    };

    /// @brief the first letter of any given word
    Node root;

    /// @brief helper class that be able to copy the nodes
    /// @param src source node
    /// @param dest destination node
    void copyNodes(Node* src, Node* dest);

    public:
    /// @brief Default constructor
    Trie(); 

    /// @brief Destructor
    ~Trie(); 

    /// @brief Copy constructor
    /// @param other from other trie
    Trie(const Trie& other); 

    /// @brief Assignment operator
    /// @param other other trie
    /// @return 
    Trie& operator=(Trie other);

    /// @brief method that be able to add word into trie
    /// @param word 
    void addWord(const std::string& word);

    /// @brief check if it is a legal word
    /// @param word the word that need to be check
    /// @return return true if it is a legal word
    bool isWord(const std::string& word);

    /// @brief be able to check the word prefix
    /// @param prefix 
    /// @return 
    std::vector<std::string> allWordsStartingWithPrefix(const std::string& prefix);

    /// @brief helper method for the allWordsStartingWithPrefix that be able to recursively check the words
    /// @param node node
    /// @param currentWord current word
    /// @param result 
    void allWordsStartingWithPrefixHelper(Node* node, std::string& currentWord, std::vector<std::string>& result);

    /// @brief helper method for implifying readability from the boolean logic.
    /// @param currentNode the current branch we have iterated through
    /// @param currentCharInWord the character in the word we are comparing with the branch
    /// @return whether that word was found in the trie.
    bool isCharAbsentInBranchHelper(Node* currentNode, char currentCharInWord);
};


#endif