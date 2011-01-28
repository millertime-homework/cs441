#include "pieceslot.h"

PieceSlot::PieceSlot(int id, int n, QWidget *parent) :
    QWidget(parent), my_id(id), num_spaces(n)
{
    //QVBoxLayout *pieceLayout = new QVBoxLayout;
    //int i;
    //for(i = 0; i < num_spaces; i++) {
        //position[i] = new QLabel();
        //pieceLayout->addWidget(position[i]);
    //}
    //setLayout(pieceLayout);
    //foo = new QLabel();
    //QGridLayout *myLayout = new QGridLayout;
    //myLayout->addWidget(foo, 0, 0);
    //setLayout(myLayout);
}

void PieceSlot::maybeAddPiece(int i, QString color)
{
    if ((i == my_id) && !full()) {
        addPiece(color);
    }
}

void PieceSlot::addPiece(QString color) {
    if (color == "Black") {
        //position[0]->setPixmap(QPixmap("../checkers.jpg"));
        QMessageBox::information(this, "OHSHIT", "Oh shit, it's broken.");
        //foo->setPixmap(QPixmap("../checkers.jpg"));

    }
}

bool PieceSlot::full()
{
    return false;
}
