/* Copyright (c) 2011 Russell Miller
MIT License - please see included COPYING file
or visit www.opensource.org/licenses/mit-license

board.h
*/

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
    int findDeuce(char);

private:
    int nextSlot(int);
    char matchAcross(int);
    char matchDiagDown(int);
    char matchDiagUp(int);
    char matchUpDown(int);
    int deuceAcross(char,int);
    int deuceUpDown(char,int);
    int deuceDiagUp(char,int);
    int deuceDiagDown(char,int);
    int height;
    char * column1;
    char * column2;
    char * column3;
};

#endif
