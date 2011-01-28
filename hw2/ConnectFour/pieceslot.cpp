#include "pieceslot.h"

PieceSlot::PieceSlot(int id, int n, QWidget *parent) :
    QWidget(parent), my_id(id), num_spaces(n)
{
    QVBoxLayout *pieceLayout = new QVBoxLayout;
    int i;
    for(i = 0; i < num_spaces; i++) {
        position[i] = new QLabel();
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
        addPiece(color);
    }
}

void PieceSlot::addPiece(QString color) {
    if (color == "Black") {
        position[num_spaces-1]->setPixmap(QPixmap(":/pictures/black-sm.jpg"));
    }
}

bool PieceSlot::full()
{
    return false;
}
