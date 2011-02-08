#ifndef CONNECTTHREE_H
#define CONNECTTHREE_H

#include <QtGui>
#include "board.h"

class QLabel;
class QPushButton;

class ConnectThree : public QWidget
{
    Q_OBJECT

public slots:
    void selectedSlot(int);
    void changeTurn(int);
    void canHasProof();

signals:
    void addPiece(int,char);
    void resetSlot(int);

public:
    ConnectThree(QWidget *parent = 0);
    ~ConnectThree();
    void proof();
    int proofTakeTurn(Board &, char);

private:
    void enemyTurn();
    int tryRed(Board &, int);
    int tryBlack(Board &, int);
    void announceWin(char);
    void printArray(Board &);
    QPushButton *quitButton;
    QPushButton *testButton;
    char whose_turn;
    Board *gameboard;
};

#endif // MAINWINDOW_H
