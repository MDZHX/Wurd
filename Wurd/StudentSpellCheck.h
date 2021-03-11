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
        TrieNode* children[27];
        bool isWord;
    };
    
    int indexOf(char c) const;
    char charOf(int i) const;
    
    void createEmptyTrie();
    void freeTrie(TrieNode* root);
    
    bool insert(std::string word);
    bool find(TrieNode* root, std::string word) const;
    void search(TrieNode* root, std::string word, int pos, int maxSuggestions, std::vector<std::string>& suggestions);
    
    TrieNode* m_root;
};

#endif  // STUDENTSPELLCHECK_H_
