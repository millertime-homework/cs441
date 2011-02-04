#ifndef THREADEDMINIMAX_H
#define THREADEDMINIMAX_H

#include <QThread>
#include <QWidget>
#include "connectthree.h"

class ThreadedMinimax : public QThread
{
  Q_OBJECT

public:
    void init(ConnectThree*, int, char[3][10]);
    int result();
    int getCol();

protected:
    void run();

private:
    int tryRed(char[3][10], int);
    int tryBlack(char[3][10], int);

    int column;
    char board[3][10];
    int my_val;
    ConnectThree * daddy;
};

#endif
