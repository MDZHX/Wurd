#include "StudentUndo.h"

#include <iostream>

Undo* createUndo() {
	return new StudentUndo;
}

StudentUndo::StudentUndo()
 : Undo() {
    // TODO: Might change
}

StudentUndo::~StudentUndo() {
    // TODO: Might change
    clear();
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
    if (!m_ops.empty() &&
        m_ops.top()->action == Action::DELETE && action == Action::DELETE &&
        m_ops.top()->row == row && m_ops.top()->col == col) {
        m_ops.top()->text += ch;
    }
    else if (!m_ops.empty() &&
             m_ops.top()->action == Action::DELETE && action == Action::DELETE &&
             m_ops.top()->row == row && m_ops.top()->col - 1 == col) {
        m_ops.top()->text = ch + m_ops.top()->text;
        m_ops.top()->col--;
    }
    else if (!m_ops.empty() &&
             m_ops.top()->action == Action::INSERT && action == Action::INSERT &&
             m_ops.top()->row == row && m_ops.top()->col + 1 == col) {
        m_ops.top()->text += ch;
        m_ops.top()->col++;
    }
    else {
        Operation* newOp = new Operation(action, row, col, ch);
        m_ops.push(newOp);
    }
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    if (m_ops.empty()) return Action::ERROR;
    
    Operation* prev = m_ops.top();
    int cnt = 1;
    std::string txt;
    Action ret;
    if (prev->action == Action::INSERT) {
        cnt = prev->text.length();
        ret = Action::DELETE;
    }
    else if (prev->action == Action::DELETE) {
        txt = prev->text;
        ret = Action::INSERT;
    }
    else if (prev->action == Action::SPLIT) {
        ret = Action::JOIN;
    }
    else if (prev->action == Action::JOIN) {
        ret = Action::SPLIT;
    }
    else {
        return Action::ERROR;
    }
    
    row = prev->row;
    col = prev->col;
    count = cnt;
    text = txt;
    
    delete prev;
    m_ops.pop();
    
    return ret;
}

void StudentUndo::clear() {
    while (!m_ops.empty()) {
        delete m_ops.top();
        m_ops.pop();
    }
}
