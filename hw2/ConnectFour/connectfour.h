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

signals:
    void addPiece(int,QString);

public:
    ConnectFour(QWidget *parent = 0);
    ~ConnectFour();

private:
    QPushButton *quitButton;
    int num_columns;

};

#endif // MAINWINDOW_H
