#ifndef CONNECTTHREE_H
#define CONNECTTHREE_H

#include <QtGui>

class QLabel;
class QPushButton;

class ConnectThree : public QWidget
{
    Q_OBJECT

public slots:
    void selectedSlot(int);
    void changeTurn(int);

signals:
    void addPiece(int,QString);
    void resetSlot(int);

public:
    ConnectThree(QWidget *parent = 0);
    ~ConnectThree();

private:
    void enemyTurn();
    int tryRed(char[3][10]);
    int tryBlack(char[3][10]);
    void updateBoardArray(int,QString);
    int eval(char[3][10]);
    int nextSlot(char[3][10], int);
    char matchAcross(int, char[3][10]);
    char matchDiagDown(int, char[3][10]);
    char matchDiagUp(int, char[3][10]);
    char matchUpDown(int, char[3][10]);
    void announceWin(char);
    void resetBoard();
    void printArray(char board[3][10]);
    QPushButton *quitButton;
    int num_columns;
    QString whose_turn;
    char columns[3][10];
};

#endif // MAINWINDOW_H
