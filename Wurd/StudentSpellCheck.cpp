#include "StudentSpellCheck.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

SpellCheck* createSpellCheck() {
	return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck() {
    createEmptyTrie();
}

StudentSpellCheck::~StudentSpellCheck() {
    freeTrie(m_root);
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    std::ifstream infile(dictionaryFile);
    if (!infile) {
        return false;
    }
    
    freeTrie(m_root);
    createEmptyTrie();
    
    std::string line;
    while (getline(infile, line)) {
        std::string word;
        for (char c : line) {
            if (isalpha(c) || c == '\'')
                word += tolower(c);
        }
        if (word.length() > 0)
            insert(word);
    }
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    if (word.length() == 0) return true; // TODO: remove this?
    
    if (find(m_root, word)) return true;
    
    suggestions.clear();
    search(m_root, word, 0, max_suggestions, suggestions);
    
	return false; // TODO: check empty word?
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
}

//
// Private Member Functions
//

StudentSpellCheck::TrieNode::TrieNode()
: isWord(false) {
   for (int i = 0; i < 27; i++) {
       children[i] = nullptr;
   }
}

// unsafe to assume ASCII scheme,
// char values might not be consecutive i.e. 'b' - 'a' might not be 1
// thus use a separate function to map char to int
int StudentSpellCheck::indexOf(char c) const {
    return chars.find(c);
}

// similarly, use a separate function to map int to char
char StudentSpellCheck::charOf(int i) const {
    return chars.at(i);
}

void StudentSpellCheck::createEmptyTrie() {
    m_root = new TrieNode();
}

void StudentSpellCheck::freeTrie(TrieNode* root) {
    for (TrieNode* i : root->children) {
        if (i != nullptr)
            freeTrie(i);
    }
    delete root;
}

bool StudentSpellCheck::insert(std::string word) {
    TrieNode* cur = m_root;
    for (char c : word) {
        int i = indexOf(c);
        if (cur->children[i] != nullptr) {
            cur = cur->children[i];
        }
        else {
            cur->children[i] = new TrieNode();
            cur = cur->children[i];
        }
    }
    if (cur->isWord) {
        return false;
    }
    else {
        cur->isWord = true;
        return true;
    }
}

bool StudentSpellCheck::find(TrieNode* root, std::string word) const {
    TrieNode* cur = root;
    for (char c : word) {
        int i = indexOf(c);
        if (cur->children[i] == nullptr) return false;
        cur = cur->children[i];
    }
    return cur->isWord;
}

void StudentSpellCheck::search(TrieNode* root, std::string word, int pos, int maxSuggestions, std::vector<std::string>& suggestions) {
    // TODO: base case
    if (pos == word.length() - 1) {
        for (int i = 0; i < 27; i++) {
            if (suggestions.size() == maxSuggestions) break;
            if (root->children[i] != nullptr && root->children[i]->isWord) {
                suggestions.push_back(word.substr(0, pos) + charOf(i));
            }
        }
        return;
    }
    
    char curChar = word.at(pos);
    int curCharIndex = indexOf(curChar);
    for (int i = 0; i < 27; i++) {
        if (suggestions.size() == maxSuggestions) break;
        if (i != curCharIndex && root->children[i] != nullptr) {
            if (find(root->children[i], word.substr(pos + 1)))
                suggestions.push_back(word.substr(0, pos) + charOf(i) + word.substr(pos + 1));
        }
    }
    
    if (suggestions.size() < maxSuggestions && root->children[curCharIndex] != nullptr) {
        search(root->children[curCharIndex], word, pos + 1, maxSuggestions, suggestions);
    }
}
