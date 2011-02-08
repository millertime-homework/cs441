#ifndef BOARD_H
#define BOARD_H

#include <string>
using std::string;

class Board
{
public:
    Board(int);
    Board(const Board &);
    ~Board();
    int eval(char);
    void insert(int,char);
    void clear();
    string toString();
    bool isFull();
    bool isColFull(int);

private:
    int nextSlot(int);
    char matchAcross(int);
    char matchDiagDown(int);
    char matchDiagUp(int);
    char matchUpDown(int);
    int height;
    char * column1;
    char * column2;
    char * column3;
};

#endif
