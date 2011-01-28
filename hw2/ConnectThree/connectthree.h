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

public:
    ConnectThree(QWidget *parent = 0);
    ~ConnectThree();

private:
    void enemyTurn();
    QPushButton *quitButton;
    int num_columns;
    QString whose_turn;
    int free_spaces[3];
};

#endif // MAINWINDOW_H
