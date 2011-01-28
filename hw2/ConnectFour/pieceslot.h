#ifndef PIECESLOT_H
#define PIECESLOT_H

#include <QtGui>

class PieceSlot : public QWidget
{
    Q_OBJECT
public:
    PieceSlot(int, int, QWidget *parent = 0);

signals:

public slots:
    void maybeAddPiece(int,QString);

private:
    void addPiece(QString);
    bool full();
    int my_id;
    int num_spaces;
    //QLabel *position[6];
    //QLabel *foo;

};

#endif // PIECESLOT_H
