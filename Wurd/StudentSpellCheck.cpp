#include "StudentSpellCheck.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

SpellCheck* createSpellCheck() {
	return new StudentSpellCheck;
}

// create an empty Trie
StudentSpellCheck::StudentSpellCheck() {
    createEmptyTrie();
}

// free the memory
StudentSpellCheck::~StudentSpellCheck() {
    freeTrie(m_root);
}

// O(N)
bool StudentSpellCheck::load(std::string dictionaryFile) {
    std::ifstream infile(dictionaryFile);
    if (!infile) {
        return false; // directly return and do nothing
    }
    
    // first free the old Trie and create a new one
    freeTrie(m_root);
    createEmptyTrie();
    
    std::string line;
    while (getline(infile, line)) { // runs [# of lines] times
        std::string word;
        // only consider letters and apostrophe
        for (char c : line) { // O(1) since assumes a constant upper bound on the length of a line
            if (isalpha(c) || c == '\'')
                word += tolower(c); // convert to lowercase for easier searching
        }
        if (word.length() > 0) // there's a word in this line
            insert(word);
    }
    return true;
}

// runs in O(oldS + L^2 + maxSuggestions)
bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    if (word.length() == 0) return true; // don't check empty strings
    
    std::string checkWord;
    
    // O(L)
    // convert to lowercase for easier searching
    for (char c : word) {
        checkWord += tolower(c);
    }
    
    if (find(m_root, checkWord)) return true; // O(L)
    
    // if the word is not found, clear and add suggestions
    suggestions.clear(); // O(oldS)
    // collect all sugggestions
    search(m_root, checkWord, 0, max_suggestions, suggestions); // O(L^2 + maxSuggestions)
    
	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
    problems.clear(); // O(oldP)
    std::string word;
    int start = -1;
    for (int i = 0; i < line.length(); i++) { // O(S) loop
        if (isalpha(line[i]) || line[i] == '\'') {
            if (word == "") start = i; // mark the beginning of a new word
            word += tolower(line[i]);
        }
        else if (word != "") { // a new word
            if (!find(m_root, word)) { // find is O(L), enter this branch W times at most
                Position p;
                p.start = start;
                p.end = i - 1;
                problems.push_back(p);
            }
            word = "";
        }
    }
    if (word != "" && !find(m_root, word)) { // check for the last possible word, find is O(L)
        Position p;
        p.start = start;
        p.end = line.length() - 1;
        problems.push_back(p);
    }
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

// explained in the header file, O(1)
int StudentSpellCheck::indexOf(char c) const {
    return chars.find(c);
}

// explained in the header file, O(1)
char StudentSpellCheck::charOf(int i) const {
    return chars.at(i);
}

void StudentSpellCheck::createEmptyTrie() {
    m_root = new TrieNode(); // dummy node
}

// free the tree recursively
void StudentSpellCheck::freeTrie(TrieNode* root) {
    for (TrieNode* i : root->children) {
        if (i != nullptr) // since we never pass a nullptr to it and nullptr is checked here, no need for base case
            freeTrie(i); // recursively free Tries
    }
    delete root;
}

// O(L) insertion
bool StudentSpellCheck::insert(std::string word) {
    TrieNode* cur = m_root;
    for (char c : word) { // loop through every character of the word
        int i = indexOf(c); // find appropriate array index for the current character
        if (cur->children[i] != nullptr) { // if the corresponding node exists
            cur = cur->children[i];
        }
        else { // if no existing node, create new nodes
            cur->children[i] = new TrieNode();
            cur = cur->children[i];
        }
    }
    // the word already exists
    if (cur->isWord) {
        return false;
    }
    // the word does not exist, mark the ending
    else {
        cur->isWord = true;
        return true;
    }
}

// O(L) search
bool StudentSpellCheck::find(TrieNode* root, std::string word) const {
    TrieNode* cur = root;
    for (char c : word) { // loop through every character in the word
        int i = indexOf(c);
        if (cur->children[i] == nullptr) return false; // the character does not follow the current node, no such word exist
        cur = cur->children[i];
    }
    return cur->isWord; // also check if there is a ending mark at the last character
}

// search for matchings with the same length and differs by at most one character for word.substr(pos) starting at root, update suggestions
// for a word with length L, all suggestions = suggestions with the different character at the beginning + suggestions with the different character in the remaining part
// this gives a recurrence relation f(L) = 27 * L[complexity of find a strict match] + f(L-1)[search for the remaining part]
// which gives O(L^2) complexity for the recursion part
// adding the factor of maxSuggestions, the total complexity is O(L^2 + maxSuggestions)
void StudentSpellCheck::search(TrieNode* root, std::string word, int pos, int maxSuggestions, std::vector<std::string>& suggestions) {
    // spellCheck never calls search with empty string, so no need to check for that
    // base case, if only one character left, any existing word can be a suggestion since there can be a different character
    if (pos == word.length() - 1) { // reached the last character
        for (int i = 0; i < 27; i++) {
            if (suggestions.size() == maxSuggestions) break; // if there are enough suggestions, break immediately
            if (root->children[i] != nullptr && root->children[i]->isWord) { // if there is a letter and the letter is an ending
                suggestions.push_back(word.substr(0, pos) + charOf(i)); // concatenate the previous part of the word as well
            }
        }
        return;
    }
    
    // the current character in the word to search for a match
    char curChar = word.at(pos);
    // the corresponding index in the TrieNode array
    int curCharIndex = indexOf(curChar);
    
    // find all possible candidates that has the different character at position pos
    // for every character other than the original character, if there is a strict match for the rest of the word, it becomes a suggestion candidate
    for (int i = 0; i < 27; i++) {
        if (suggestions.size() == maxSuggestions) break; // if there are enough suggestions, break immediately
        if (i != curCharIndex && root->children[i] != nullptr) { // can't be the original character, and the character must exist
            if (find(root->children[i], word.substr(pos + 1))) // find strict match for the remaining part, since a candidate can differ by at most one character
                suggestions.push_back(word.substr(0, pos) + charOf(i) + word.substr(pos + 1)); // a suggestion word that differs at position pos
        }
    }
    
    // find all possible candidates that has the different character in the remaining part
    if (suggestions.size() < maxSuggestions && root->children[curCharIndex] != nullptr) {
        search(root->children[curCharIndex], word, pos + 1, maxSuggestions, suggestions);
    }
}
