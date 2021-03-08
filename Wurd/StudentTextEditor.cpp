#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>

TextEditor* createTextEditor(Undo* un) {
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo), m_row(0), m_col(0) {
	// TODO: Might change
     m_lines.push_back("");
     m_line = m_lines.begin();
}

StudentTextEditor::~StudentTextEditor() {
	// TODO: Implement this
}

bool StudentTextEditor::load(std::string file) {
	return false;  // TODO
}

bool StudentTextEditor::save(std::string file) {
	return false;  // TODO
}

void StudentTextEditor::reset() {
	// TODO
}

void StudentTextEditor::move(Dir dir) {
    // TODO: check changing col
    if (dir == Dir::UP) {
        if (m_row > 0) {
            m_row--;
            m_line--;
            if (m_col > m_line->length()) m_col = m_line->length();
        }
    }
    else if (dir == Dir::DOWN) {
        if (m_row < m_lines.size() - 1) {
            m_row++;
            m_line++;
            if (m_col > m_line->length()) m_col = m_line->length();
        }
    }
    else if (dir == Dir::LEFT) {
        if (m_col > 0) {
            m_col--;
        }
        else if (m_row > 0) {
            m_row--;
            m_line--;
            m_col = m_line->length();
        }
    }
    else if (dir == Dir::RIGHT) {
        if (m_col < m_line->length()) {
            m_col++;
        }
        else if (m_row < m_lines.size() - 1) {
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

void StudentTextEditor::del() {
	// TODO
}

void StudentTextEditor::backspace() {
	// TODO
}

void StudentTextEditor::insert(char ch) {
    if (ch == '\t') {
        for (int i = 0; i < 4; i++) {
            m_line->insert(m_col++, 1, ' ');
            getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ' ');
        }
    }
    else {
        (*m_line).insert(m_col++, 1, ch);
        getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
    }
}

void StudentTextEditor::enter() {
	// TODO
}

void StudentTextEditor::getPos(int& row, int& col) const {
    row = m_row;
    col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    if (startRow < 0 || numRows < 0 || startRow > m_lines.size()) return -1;
    
    lines.clear();
    
    std::list<std::string>::iterator it = m_line;
    int l = m_row;
    if (startRow >= m_row) {
        while (l != startRow) {
            l++;
            it++;
        }
    }
    else {
        while (l != startRow) {
            l--;
            it--;
        }
    }
    
    int i;
    for (i = 0; i < numRows; i++) {
        if (it == m_lines.end()) break;
        lines.push_back(*it);
        it++;
    }
	return i;
}

void StudentTextEditor::undo() {
	// TODO
}
