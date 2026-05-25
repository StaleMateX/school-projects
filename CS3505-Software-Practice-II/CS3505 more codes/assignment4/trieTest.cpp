#include <iostream>
#include <fstream>
#include <string>
#include "trie.h"
/**
 * trieTest.cpp
 * by Jacob Xu
 */
using namespace std;

/// @brief It should take in two command-line arguments, each one a filename. The first is a file of words, each on their own line, with the words all lowercase and only made up of characters a-z. The second is a file of queries, with each word or partial word on its own line.
/// @param argc
/// @param argv
/// @return
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "We need two filename!" << endl;
        return 0;
    }
    // generate string to hold the both files
    string dictionary = argv[1];
    string queries = argv[2];
    // empty string for future use
    string word;
    // create trie
    Trie *trie = new Trie();
    // read the dictionary file
    ifstream wordsDict;
    // open the dictionary file and read it
    wordsDict.open(dictionary);
    // while the dictionary is open, read each line from the file
    if (wordsDict.is_open())
    {
        while (getline(wordsDict, word))
        {
            // add the word into trie
            trie->addWord(word);
        }
    }
    else
    {
        cout << "unable to open the file. check errors" << endl;
        return 0;
    }
    wordsDict.close();
    // empty string for test file
    string test;

    ifstream wordsQueries;
    wordsQueries.open(queries);

    if (wordsQueries.is_open())
    {
        while (getline(wordsQueries, test))
        {
            // if the word is in the trie, print found message
            if (trie->isWord(test))
            {
                cout << "The word " + test + " founded!" << endl;
            }
            else
            { // else print not found
                cout << "The word " + test + " is not found!" << endl;
            }
            // Attempt to find all the suggested words
            auto wordsPrefix = trie->allWordsStartingWithPrefix(test);
            // Loop thought and return all the suggested words.
            for (auto &text : wordsPrefix)
            {
                cout << text << " ";
            }
            cout << endl;
        }
    }
    else
    {
        cout << "unable to open the file. check errors" << endl;
        return 0;
    }
    wordsQueries.close();
    // Delete the Trie
    delete trie;
    return 0;
}