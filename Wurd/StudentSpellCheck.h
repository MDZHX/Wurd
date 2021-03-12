#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

const std::string chars = "abcdefghijklmnopqrstuvwxyz'";

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck();
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    struct TrieNode {
        TrieNode();
        TrieNode* children[27]; // 26 letters + apostrophe
        bool isWord; // if there is some words that end at this node
    };
    
    // in some encoding schemes,
    // char values might not be consecutive i.e. 'b' - 'a' might not be 1
    // thus use separate functions to map char to int and vice versa
    int indexOf(char c) const; // take a char, returns its appropriate index in the TrieNode array
    char charOf(int i) const; // take an index in the TrieNode array, returns the corresponding char
    
    void createEmptyTrie(); // create an empty Trie rooted at m_root, with a dummy node
    void freeTrie(TrieNode* root); // free up the whole tree rooted at root
    
    bool insert(std::string word); // insert the word into the Trie rooted at m_root
    bool find(TrieNode* root, std::string word) const; // return true if the word exists in the tree rooted at root
    void search(TrieNode* root, std::string word, int pos, int maxSuggestions, std::vector<std::string>& suggestions); // search for possible suggestions
    
    TrieNode* m_root; // root of the Trie
};

#endif  // STUDENTSPELLCHECK_H_
