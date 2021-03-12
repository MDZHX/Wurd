#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include "Undo.h"

#include <list>

class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
    std::list<std::string> m_lines; // list of all lines of text
    std::list<std::string>::iterator m_line; // point to the current line
    int m_row; // cursor row
    int m_col; // cursor col
    
    std::list<std::string>::iterator getLine(int row) const; // return an iterator to the specified line
    void moveTo(int r, int c); // go to the specified position
    void moveTo(int r); // go to the last position of the specified line
    void join(); // join the next line with the current line
    void split(); // split the current line at the current position
    void createEmpty(); // create an empty document
};

#endif // STUDENTTEXTEDITOR_H_
