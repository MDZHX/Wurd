#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"

#include <stack>
#include <list>

class StudentUndo : public Undo {
public:
    StudentUndo(): Undo() {};
    ~StudentUndo();
	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    // struct for an operation
    struct Operation {
        Operation(Action act, int r, int c, char ch) : action(act), row(r), col(c) { text.push_back(ch); }
        Action action;
        int row;
        int col;
        std::list<char> text;
    };
    
    // operation stack
    std::stack<Operation*> m_ops;
};

#endif // STUDENTUNDO_H_
