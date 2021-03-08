#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"

#include <stack>

class StudentUndo : public Undo {
public:
    StudentUndo();
    ~StudentUndo();
	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    struct Operation {
        Operation(Action act, int r, int c, char ch) : action(act), row(r), col(c), text(1, ch) {}
        Action action;
        int row;
        int col;
        std::string text;
    };
    
    std::stack<Operation*> m_ops;
};

#endif // STUDENTUNDO_H_
