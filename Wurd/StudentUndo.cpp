#include "StudentUndo.h"

#include <iostream>
#include <stack>
#include <list>

Undo* createUndo() {
	return new StudentUndo;
}

StudentUndo::~StudentUndo() {
    clear(); // frees the dynamically allocated Operations
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
    if (!m_ops.empty() && // ensures nonempty before checking the top
        m_ops.top()->action == Action::DELETE && action == Action::DELETE && // two consecutive deletes
        m_ops.top()->row == row && m_ops.top()->col == col) { // deletion at the same spot
        m_ops.top()->text.push_back(ch);
    }
    else if (!m_ops.empty() && // ensures nonempty before checking the top
             m_ops.top()->action == Action::DELETE && action == Action::DELETE && // two consecutive deletes
             m_ops.top()->row == row && m_ops.top()->col - 1 == col) {
        m_ops.top()->text.push_front(ch);
        m_ops.top()->col--;
    }
    else if (!m_ops.empty() && // ensures nonempty before checking the top
             m_ops.top()->action == Action::INSERT && action == Action::INSERT && // two consecutive inserts
             m_ops.top()->row == row && m_ops.top()->col + 1 == col) {
        m_ops.top()->text.push_back(ch);
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
        cnt = prev->text.size();
        ret = Action::DELETE;
    }
    else if (prev->action == Action::DELETE) {
        for (auto i = prev->text.begin(); i != prev->text.end(); i++) {
            txt += (*i);
        }
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
