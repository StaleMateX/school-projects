/* 
    By Joshua Brody u1351827, Jacob Xu u1448572
    CS3505 Assignment 5, 2/15/2024
*/

#include <iostream>
#include "trie.h"

/// @brief Default Constructor, makes an empty root node.
Trie::Trie() {};

/// @brief uses the clearTrie method to delete the dynamic trie.
Trie::~Trie() {};

/// @brief Copy Constructor that makes a copy of the other without changing other.
/// @param other trie to be copied from
Trie::Trie(const Trie& other) {
    root = other.root;
}

/// @brief Assignment Operator that copies other and then deletes other.
/// @param copyThisTrie trie to be copied from and deleted
/// @return A copy of other
Trie& Trie::operator=(Trie copyThisTrie) {
    std::swap(root, copyThisTrie.root);
    return *this;
}

/// @brief Method to a word to the trie
/// @param word a lowercase string to be added
void Trie::addWord(const std::string& word) {
    Node* currentBranch = &root;
    for (char currentCharInWord : word) {
        if (isCharAbsentInBranchHelper(currentBranch, currentCharInWord)) {
            currentBranch->branches[currentCharInWord] = Node();
        }
        currentBranch = &(currentBranch->branches[currentCharInWord]);
    }
    currentBranch->isEndOfWord = true;
}

/// @brief Check if a word is in the trie
/// @param word the lowercase string to be found in the trie
/// @return false is word is not found, true otherwise
bool Trie::isWord(const std::string& word) {
    Node* currentBranch = &root;
    for (char currentCharInWord : word) {
        if (isCharAbsentInBranchHelper(currentBranch, currentCharInWord)) {
            return false;
        }
        currentBranch = &(currentBranch->branches[currentCharInWord]);
    }
    return currentBranch->isEndOfWord;
}

/// @brief helper method for implifying readability from the boolean logic.
/// @param currentNode the current branch we have iterated through
/// @param currentCharInWord the character in the word we are comparing with the branch
/// @return whether that word was found in the trie.
bool Trie::isCharAbsentInBranchHelper(Node* currentNode, char currentCharInWord){
    return currentNode->branches.find(currentCharInWord) == currentNode->branches.end();
}

/// @brief will return a group of words that begin with the given prefix
/// @param prefix a lowercase string to be compared in the trie
/// @return a vector string that contains all the words that start with the given prefix
std::vector<std::string> Trie::allWordsStartingWithPrefix(const std::string& prefix) {
    std::vector<std::string> result;
    Node* currentBranch = &root;

    // Traverse the trie until the end of the prefix
    for (char currentCharInPrefix : prefix) {
        if (isCharAbsentInBranchHelper(currentBranch, currentCharInPrefix)) {
            return result; // Prefix not found
        }
        currentBranch = &(currentBranch->branches[currentCharInPrefix]);
    }

    // Perform DFS to get all words starting with the prefix
    // using a recursive helper function
    std::string currentWord = prefix;
    allWordsStartingWithPrefixHelper(currentBranch, currentWord, result);

    return result;
}

/// @brief Helper method to recursively search whether the current word branches to the next correct letter of the prefix
/// @param node the letter node given from the previous function the contains the next letter in the prefix to search for
/// @param currentWord the current branch we are searching for the letter
/// @param result a lowercase string with the correct word that begins with the prefix
void Trie::allWordsStartingWithPrefixHelper(Node* currentBranchNode, std::string& currentWord, std::vector<std::string>& result) {
    if (currentBranchNode->isEndOfWord) {
        result.push_back(currentWord);
    }

    for (auto& [key, val] : currentBranchNode->branches) {
        currentWord.push_back(key);
        allWordsStartingWithPrefixHelper(&(currentBranchNode->branches[key]), currentWord, result);
        currentWord.pop_back();
    }
}
