#include "StudentSpellCheck.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

SpellCheck* createSpellCheck() {
	return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck() {
    m_trie = new Trie();
}

StudentSpellCheck::~StudentSpellCheck() {
    delete m_trie;
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    std::ifstream infile(dictionaryFile);
    if (!infile) {
        return false;
    }
    
    delete m_trie;
    m_trie = new Trie();
    
    std::string line;
    while (getline(infile, line)) {
        std::string word;
        for (char c : line) {
            if (isalpha(c) || c == '\'')
                word += tolower(c);
        }
        if (word.length() > 0)
            m_trie->insert(word);
    }
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	return false; // TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
}

//
// Trie implementation
//

StudentSpellCheck::Trie::Trie() {
    root = new TrieNode();
}

StudentSpellCheck::Trie::~Trie() {
    freeTrie(root);
}

bool StudentSpellCheck::Trie::insert(std::string word) {
    TrieNode* cur = root;
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

bool StudentSpellCheck::Trie::find(std::string word) const {
    TrieNode* cur = root;
    for (char c : word) {
        int i = indexOf(c);
        if (cur->children[i] == nullptr) return false;
        cur = cur->children[i];
    }
    return cur->isWord;
}

void StudentSpellCheck::Trie::freeTrie(TrieNode* root) {
    for (TrieNode* i : root->children) {
        if (i != nullptr)
            freeTrie(i);
    }
    delete root;
}

//
// TrieNode Implementation
//

StudentSpellCheck::Trie::TrieNode::TrieNode()
: isWord(false) {
   for (int i = 0; i < 27; i++) {
       children[i] = nullptr;
   }
}
