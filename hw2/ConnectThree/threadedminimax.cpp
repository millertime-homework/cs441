#include "threadedminimax.h"
#include "connectthree.h"

void ThreadedMinimax::init(ConnectThree * parent, int col, char b[3][10])
{
    daddy = parent;
    column = col;
    int i,j;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 10; j++) {
            board[i][j] = b[i][j];
        }
    }
}

int ThreadedMinimax::result()
{
    return my_val;
}

void ThreadedMinimax::run()
{
    int ns = daddy->nextSlot(board,column);
    board[column][ns] = 'R';
    my_val = tryBlack(board, 3);
}

int ThreadedMinimax::tryRed(char board[3][10], int depth)
{
    int status = 0 - daddy->eval(board, 'B');
    //qDebug() << "tryRed, depth " << depth << ", score " << status;
    if ((status != 0) || (depth == 0))
        return status;
    int returnval = -9000;   // it can't be OVER 9000 can it?!
    int i;
    for(i = 0; i < 3; i++) {
        if (board[i][9] == '-') {  // last slot still available, not full
            char new_board[3][10];
            int j,k;
            for(j = 0; j < 3; j++) {
                for(k = 0; k < 10; k++) {
                    new_board[j][k] = board[j][k];
                }
            }
            int ns = daddy->nextSlot(new_board, i);
            new_board[i][ns] = 'R';
            int score = tryBlack(new_board, depth-1);  // recurse
            if (score > returnval) {
                returnval = score;
            }
        }
    }
    qDebug() << "About to multiply min " << returnval << "times depth " << depth;
    return returnval * (depth+1);
}

int ThreadedMinimax::tryBlack(char board[3][10], int depth)
{
    int status = daddy->eval(board, 'R');
    //qDebug() << "tryBlack, depth " << depth << ", score " << status;
    if ((status != 0) || (depth == 0))
        return status;
    int returnval = 9000;
    int i;
    for(i = 0; i < 3; i++) {
        if (board[i][9] == '-') {
            char new_board[3][10];
            int j,k;
            for(j = 0; j < 3; j++) {
                for(k = 0; k < 10; k++) {
                    new_board[j][k] = board[j][k];
                }
            }
            int ns = daddy->nextSlot(new_board, i);
            new_board[i][ns] = 'B';
            int score = tryRed(new_board, depth-1);
            if (score < returnval) {
                returnval = score;
            }
        }
    }
    qDebug() << "About to multiply min " << returnval << "times depth " << depth;
    return returnval * (depth+1);
}
