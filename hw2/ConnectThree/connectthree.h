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
    int free_spaces(int);
    void updateBoardArray(int,QString);
    void checkForWin();
    char matchAcross(int);
    char matchDiagDown(int);
    char matchDiagUp(int);
    char matchUpDown(int);
    void announceWin(char);
    void resetBoard();
    QPushButton *quitButton;
    int num_columns;
    QString whose_turn;
    char columns[3][10];
};

#endif // MAINWINDOW_H
