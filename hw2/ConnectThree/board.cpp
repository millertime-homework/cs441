/* Copyright (c) 2011 Russell Miller
MIT License - please see included COPYING file
or visit www.opensource.org/licenses/mit-license

board.cpp
Store the current game state.
Helpful methods like inserting a piece and looking up score
*/

#include <string>
#include <string.h>
#include "board.h"

using std::string;

// Only normal constructor, must specify board height
Board::Board(int h) : height(h)
{
    column1 = new char[h];
    column2 = new char[h];
    column3 = new char[h];
    clear();    // fill the board with '-' to represent empty spaces
}

// Copy constructor, for making temporary copies
Board::Board(const Board & copy) : height(copy.height)
{
    column1 = new char[copy.height];
    column2 = new char[copy.height];
    column3 = new char[copy.height];
    int i;
    for(i = 0; i < copy.height; i++) {
        column1[i] = copy.column1[i];
        column2[i] = copy.column2[i];
        column3[i] = copy.column3[i];
    }
}

Board::~Board()
{
    delete [] column1;
    delete [] column2;
    delete [] column3;
}

// Main method to get score of current game state
int Board::eval(char player)
{
    int i;
    char r, enemy;
    if (player == 'B')
        enemy = 'R';
    else
        enemy = 'B';
    for(i = 0; i < height; i++) {
        // X X X
        if ((r = matchAcross(i)) != '-') {
            if (r == enemy)
                return -2;  // bad guy won
            else if (r == player)
                return 2;   // good guy won
        }
        // X
        //   X
        //     X
        if ((r = matchDiagDown(i)) != '-') {
            if (r == enemy)
                return -2;
            else if (r == player)
                return 2;
        }
        //     X
        //   X
        // X
        if ((r = matchDiagUp(i)) != '-') {
            if (r == enemy)
                return -2;
            else if (r == player)
                return 2;
        }
        // X
        // X
        // X
        if ((r = matchUpDown(i)) != '-') {
            if (r == enemy)
                return -2;
            else if (r == player)
                return 2;
        }
        // last slot in each column taken? board full. draw.
        if (isFull()) {
            return 1;
        }
    }
    return 0;
}

// Add a gamepiece to the board in column 'col' with label 'player'
void Board::insert(int col, char player)
{
    if (isColFull(col))
        return;
    int ns = nextSlot(col);
    switch(col) {
    case 1:
        column1[ns] = player;
        break;
    case 2:
        column2[ns] = player;
        break;
    case 3:
        column3[ns] = player;
        break;
    default:
        return;
    }
}

void Board::clear()
{
    int i;
    for(i = 0; i < height; i++) {
        column1[i] = '-';
        column2[i] = '-';
        column3[i] = '-';
    }
}

// Useful for debugging
string Board::toString()
{
    column1[height] = '\0';
    column2[height] = '\0';
    column3[height] = '\0';
    return (string(column1) + string("\n") +
            string(column2) + string("\n") +
            string(column3));
}

// Is it a draw?
bool Board::isFull()
{
    return ((column1[height-1] != '-') &&
            (column2[height-1] != '-') &&
            (column3[height-1] != '-'));
}

// Is it valid to add a piece to a column?
// Useful for search algorithm to not go down dead ends..
bool Board::isColFull(int col)
{
    switch(col) {
    case 1:
        return (column1[height-1] != '-');
        break;
    case 2:
        return (column2[height-1] != '-');
        break;
    case 3:
        return (column3[height-1] != '-');
        break;
    }
    return true;
}

// Are there 3 symbols in a row that match?
char Board::matchAcross(int row)
{
    if (column1[row] != '-' &&
        ((column1[row] == column2[row]) &&
         (column2[row] == column3[row]))) {
        return column1[row];
    }
    else
        return '-';
}

char Board::matchDiagDown(int row)
{
    if (row > (height-3))
        return '-';   // avoid an array index error
    else if (column1[row+2] != '-' &&
             (column1[row+2] == column2[row+1] &&
              column2[row+1] == column3[row])) {
        return column1[row+2];
    }
    else
        return '-';
}

char Board::matchDiagUp(int row)
{
    if (row > (height-3))
        return '-';   // avoid an array index error
    else if (column1[row] != '-' &&
             (column1[row] == column2[row+1] &&
              column2[row+1] == column3[row+2])) {
        return column1[row];
    }
    else
        return '-';
}

char Board::matchUpDown(int row)
{
    if (row > (height-3))
        return '-';
    if (column1[row] != '-' &&
       ((column1[row] == column1[row+1]) &&
        (column1[row+1] == column1[row+2])))
            return column1[row];
    if (column2[row] != '-' &&
       ((column2[row] == column2[row+1]) &&
        (column2[row+1] == column2[row+2])))
            return column2[row];
    if (column3[row] != '-' &&
       ((column3[row] == column3[row+1]) &&
        (column3[row+1] == column3[row+2])))
            return column3[row];
    return '-';
}

// Necessary for adding a gamepiece. Tells where first '-' is.
int Board::nextSlot(int col)
{
    char * tmp = NULL;
    switch(col) {
    case 1:
        tmp = column1;
        break;
    case 2:
        tmp = column2;
        break;
    case 3:
        tmp = column3;
        break;
    }
    int i;
    for(i = 0; i < height; i++) {
        if (tmp[i] == '-') {
            return i;
        }
    }
    return 0;  // shouldn't reach here
}
