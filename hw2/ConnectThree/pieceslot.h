#ifndef PIECESLOT_H
#define PIECESLOT_H

#include <QtGui>

class PieceSlot : public QWidget
{
    Q_OBJECT
public:
    PieceSlot(int, int, QWidget *parent = 0);
    ~PieceSlot();

signals:
    void pieceAdded(int);

public slots:
    void maybeAddPiece(int,QString);
    void resetSlot(int);

private:
    bool full();
    int my_id;
    int num_spaces;
    int next_spot;
    QLabel *position[10];
};

#endif // PIECESLOT_H
