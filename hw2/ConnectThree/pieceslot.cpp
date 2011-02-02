#include <QDebug>
#include "pieceslot.h"

PieceSlot::PieceSlot(int id, int n, QWidget *parent) :
    QWidget(parent), my_id(id), num_spaces(n), next_spot(n-1)
{
    QVBoxLayout *pieceLayout = new QVBoxLayout;
    int i;
    for(i = 0; i < num_spaces; i++) {
        position[i] = new QLabel();
        position[i]->setPixmap(QPixmap(":/pictures/blank.jpg"));
        pieceLayout->addWidget(position[i]);
    }
    setLayout(pieceLayout);
}

PieceSlot::~PieceSlot()
{
    int i;
    for(i = 0; i < num_spaces; i++) {
        delete position[i];
    }
}

void PieceSlot::maybeAddPiece(int i, QString color)
{
    if ((i == my_id) && !full()) {
        if (color == "Black") {
            position[next_spot--]->setPixmap(QPixmap(":/pictures/black.jpg"));
        } else {
            position[next_spot--]->setPixmap(QPixmap(":/pictures/red.jpg"));
        }
        emit pieceAdded(i);
    }
}

void PieceSlot::resetSlot(int i)
{
    if (i == my_id) {
        //qDebug() << "Signal received to reset slot " << i;
        int j;
        for(j = 0; j < num_spaces; j++) {
            position[j]->setPixmap(QPixmap(":/pictures/blank.jpg"));
        }
        next_spot = num_spaces-1;
    }
}

bool PieceSlot::full()
{
    return (next_spot == -1);
}