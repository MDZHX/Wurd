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

// always O(1), since it Operation uses list<char> which gives O(1) insertion at front/end for batching
void StudentUndo::submit(const Action action, int row, int col, char ch) {
    if (!m_ops.empty() && // ensures nonempty before checking the top
        m_ops.top()->action == Action::DELETE && action == Action::DELETE && // two deletes
        m_ops.top()->row == row && m_ops.top()->col == col) { // deletion at the same spot
        m_ops.top()->text.push_back(ch);
    }
    else if (!m_ops.empty() && // ensures nonempty before checking the top
             m_ops.top()->action == Action::DELETE && action == Action::DELETE && // two deletes
             m_ops.top()->row == row && m_ops.top()->col - 1 == col) { // position of series of backspace
        m_ops.top()->text.push_front(ch); // add to front
        m_ops.top()->col--; // update the position
    }
    else if (!m_ops.empty() && // ensures nonempty before checking the top
             m_ops.top()->action == Action::INSERT && action == Action::INSERT && // two inserts
             m_ops.top()->row == row && m_ops.top()->col + 1 == col) { // position of consecutive inserts
        m_ops.top()->text.push_back(ch);
        m_ops.top()->col++; // update the position
    }
    else {
        Operation* newOp = new Operation(action, row, col, ch); // no batching needed
        m_ops.push(newOp);
    }
}

// O(1) in the average case, increase to O(L) for undoing deletion
StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    if (m_ops.empty()) return Action::ERROR; // check for empty stack before looking at the top
    
    Operation* prev = m_ops.top(); // the most recent operation
    int cnt = 1; // only undoing INSERT requires a count different from 1
    std::string txt; // only undoing DELETE requires a text string other than the empty string
    Action ret;
    if (prev->action == Action::INSERT) {
        cnt = prev->text.size(); // set appropriate count size
        ret = Action::DELETE;
    }
    else if (prev->action == Action::DELETE) {
        // generate the text string from the list, O(L)
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
    
    delete prev; // free the dynamically allocated Operation
    m_ops.pop();
    
    return ret;
}

// O(N)
void StudentUndo::clear() {
    while (!m_ops.empty()) {
        delete m_ops.top();
        m_ops.pop();
    }
}
