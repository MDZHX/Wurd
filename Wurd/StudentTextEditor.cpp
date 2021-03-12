#include "StudentTextEditor.h"
#include "Undo.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

TextEditor* createTextEditor(Undo* un) {
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo), m_row(0), m_col(0) {
     createEmpty(); // create an empty document to edit
}

StudentTextEditor::~StudentTextEditor() {
    m_lines.clear(); // No dynamic memory allocation, but to meet the spec's requirement
}

// O(M+N+U)
bool StudentTextEditor::load(std::string file) {
    std::ifstream infile(file);
    if (!infile) {
        return false; // return directly if can't open
    }
    reset(); // call reset as required, O(M+U)
    std::string line;
    while (getline(infile, line)) { // O(N)
        if (!line.empty() && line.at(line.length()-1) == '\r') {
            line.pop_back(); // strip the ending carriage return
        }
        m_lines.push_back(line);
    }
    m_line = m_lines.begin(); // set line to the beginning
	return true;
}

// O(M)
bool StudentTextEditor::save(std::string file) {
    std::ofstream outfile(file);
    if (!outfile) {
        return false; // return directly if can't save
    }
    for (auto i = m_lines.begin(); i != m_lines.end(); i++) {
        outfile << *i << std::endl;
    }
	return true;
}

// O(N+U)
void StudentTextEditor::reset() {
    m_lines.clear(); // O(N)
    m_line = m_lines.begin();
    m_row = 0;
    m_col = 0;
    getUndo()->clear(); // O(U)
}

// O(1)
void StudentTextEditor::move(Dir dir) {
    if (m_lines.size() == 0) return; // in the case of opening an empty file, can't move the cursor
    
    if (dir == Dir::UP) {
        if (m_row > 0) {
            m_row--;
            m_line--;
            if (m_col > m_line->length()) m_col = m_line->length(); // change col accordingly if the line is shorter
        }
    }
    else if (dir == Dir::DOWN) {
        if (m_row < m_lines.size() - 1) {
            m_row++;
            m_line++;
            if (m_col > m_line->length()) m_col = m_line->length(); // change col accordingly if the line is shorter
        }
    }
    else if (dir == Dir::LEFT) {
        if (m_col > 0) {
            m_col--;
        }
        else if (m_row > 0) { // if there is a previous line, go to the previous line
            m_row--;
            m_line--;
            m_col = m_line->length();
        }
    }
    else if (dir == Dir::RIGHT) {
        if (m_col < m_line->length()) {
            m_col++;
        }
        else if (m_row < m_lines.size() - 1) { // if there is a next line, go to the next line
            m_row++;
            m_line++;
            m_col = 0;
        }
    }
    else if (dir == Dir::HOME) {
        m_col = 0;
    }
    else if (dir == Dir::END) {
        m_col = m_line->length();
    }
}

// O(L) or O(L1+L2)
void StudentTextEditor::del() {
    if (m_lines.size() == 0) return; // in the case of opening an empty file, can't delete
    
    // if at the end of a line other than the last one
    if (m_col == m_line->length() && m_row < m_lines.size() - 1)  {
        join(); // O(L1+L2)
        getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
    }
    // not at the end of a line
    else if (m_col < m_line->length()) { // it's always guaranteed that m_row < m_lines.size()
        getUndo()->submit(Undo::Action::DELETE, m_row, m_col, m_line->at(m_col)); // since position doesn't change, can sumbit before deletion
        m_line->erase(m_col, 1); // O(L)
    }
}

// O(L) or O(L1+L2)
void StudentTextEditor::backspace() {
    if (m_lines.size() == 0) return; // in the case of opening an empty file, can't backspace
    
    // if at the beginning of a line other than the first one
    if (m_col == 0 && m_row > 0) {
        moveTo(m_row - 1); // O(1), move to the position (end of previous line) after the join
        join(); // join at the previous line
        getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
    }
    // not at the beginning of a line
    else if (m_col > 0) { // it's always guaranteed that m_row < m_lines.size()
        m_col--;
        getUndo()->submit(Undo::Action::DELETE, m_row, m_col, m_line->at(m_col)); // position after deletion
        m_line->erase(m_col, 1);
    }
}

// O(L)
void StudentTextEditor::insert(char ch) {
    if (m_lines.size() == 0) { // in the case of opening an empty file, create a new document to edit
        createEmpty();
    }
    
    // convert tab to four spaces
    if (ch == '\t') {
        for (int i = 0; i < 4; i++) {
            m_line->insert(m_col++, 1, ' '); // O(L)
            getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ' ');
        }
    }
    else {
        m_line->insert(m_col++, 1, ch); // O(L)
        getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
    }
}

// O(L)
void StudentTextEditor::enter() {
    if (m_lines.size() == 0) { // in the case of opening an empty file, create a new document to edit
        createEmpty();
    }
    
    // submit position before spliting
    getUndo()->submit(Undo::Action::SPLIT, m_row, m_col);
    split(); // O(L)
    moveTo(m_row + 1, 0);  // O(1), move to the next row
    
}

void StudentTextEditor::getPos(int& row, int& col) const {
    row = m_row;
    col = m_col;
}

// O(oldR + abs(current row number - startRow) + numRows*L)
int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    if (startRow < 0 || numRows < 0 || startRow > m_lines.size()) return -1;
    
    lines.clear(); // O(oldR)
    
    auto it = getLine(startRow); // O(abs(current row number - startRow))
    
    int count = 0;
    while (count < numRows && it != m_lines.end()) // runs numRows time
    {
        lines.push_back(*it); // push an length-L string
        it++;
        count++;
    }
	return count;
}

void StudentTextEditor::undo() {
    int r, c, cnt;
    std::string txt;
    Undo::Action act = getUndo()->get(r, c, cnt, txt);
    if (act == Undo::Action::DELETE) {
        moveTo(r, c - cnt); // move to the starting position of the deletion
        m_line->erase(c-cnt, cnt); // delete cnt characters
    }
    else if (act == Undo::Action::INSERT) {
        moveTo(r, c); // move to the starting position of the deletion
        m_line->insert(m_col, txt);
    }
    else if (act == Undo::Action::JOIN) {
        moveTo(r, c);  // move to the starting position of the join
        join();
    }
    else if (act == Undo::Action::SPLIT) {
        moveTo(r, c);  // move to the starting position of the split
        split();
    }
}

//
// Private Member Functions
//

// O(abs(current row number - startRow)), when the absolute value is 1, runs in O(1)
std::list<std::string>::iterator StudentTextEditor::getLine(int row) const {
    auto line = m_line;
    int i = m_row;
    
    // decrement or increment the iterator depending on the target row
    if (row >= m_row) {
        while (i != row) {
            i++;
            line++;
        }
    }
    else {
        while (i != row) {
            i--;
            line--;
        }
    }
    
    return line;
}

// Move to row r column c
void StudentTextEditor::moveTo(int r, int c) {
    m_line = getLine(r); // O(abs(current row number - startRow)), when the absolute value is 1, runs in O(1)
    m_row = r;
    m_col = c;
}

// Move to the end of the row r
void StudentTextEditor::moveTo(int r) {
    m_line = getLine(r);
    m_row = r;
    m_col = m_line->length();
}

void StudentTextEditor::join() {
    auto nextLine = m_line;
    nextLine++;
    m_line->append(*nextLine); // append to the current line
    m_lines.erase(nextLine); // erase the next line
}

void StudentTextEditor::split() {
    std::string firstHalf = m_line->substr(0, m_col);
    m_line->erase(0, m_col); // erase the first half
    
    m_lines.insert(m_line, firstHalf); // insert the first half before the current line
    m_line--; // go to the newly inserted line
}

void StudentTextEditor::createEmpty() {
    m_lines.push_back(""); // empty string to start with
    m_line = m_lines.begin();
}
