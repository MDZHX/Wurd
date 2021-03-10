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
    // TODO: move these
    class Trie {
    public:
        Trie();
        ~Trie();
        bool insert(std::string word);
        bool find(std::string word) const;
    private:
        struct TrieNode {
            TrieNode();
            TrieNode* children[27];
            bool isWord;
        };
        
        TrieNode* root;
        
        // unsafe to assume ASCII scheme,
        // char values might not be consecutive i.e. 'b' - 'a' might not be 1
        // thus use a separate function to map char to int
        int indexOf(char c) const {
            return chars.find(c);
        }
        void freeTrie(TrieNode* root);
    };
    
    Trie* m_trie;
};

#endif  // STUDENTSPELLCHECK_H_
