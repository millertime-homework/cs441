#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <QtGui>

class QLabel;
class QPushButton;

class ConnectFour : public QWidget
{
    Q_OBJECT

public slots:
    void selectedSlot(int);
    void changeTurn(int);

signals:
    void addPiece(int,QString);

public:
    ConnectFour(QWidget *parent = 0);
    ~ConnectFour();

private:
    void enemyTurn();
    QPushButton *quitButton;
    int num_columns;
    QString whose_turn;
    int free_spaces[3];
};

#endif // MAINWINDOW_H
