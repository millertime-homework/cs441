/* Copyright (c) 2011 Russell Miller
MIT License - please see included COPYING file
or visit www.opensource.org/licenses/mit-license

pieceslot.cpp
Qt class to visualize the slots you can drop a gamepiece into.
They keep track of how full they are, and will only let a play be
made if a move is valid.
*/

#include <QDebug>
#include "pieceslot.h"

// Constructor to build a single column of the GUI
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

// It's "maybe" cause it doesn't allow it if it's invalid.
// next_spot gets a decrement to count how many spots are available
void PieceSlot::maybeAddPiece(int i, char color)
{
    if ((i == my_id) && !full()) {
        if (color == 'B') {
            position[next_spot--]->setPixmap(QPixmap(":/pictures/black.jpg"));
        } else {
            position[next_spot--]->setPixmap(QPixmap(":/pictures/red.jpg"));
        }
        emit pieceAdded(i);  // only actually send the signal when it was valid!
    }
}

// Qt slot that connects to the reset signal
void PieceSlot::resetSlot(int i)
{
    if (i == my_id) {
        int j;
        for(j = 0; j < num_spaces; j++) {
            position[j]->setPixmap(QPixmap(":/pictures/blank.jpg"));
        }
        next_spot = num_spaces-1;
    }
}

// Neat way to check if they're full
bool PieceSlot::full()
{
    return (next_spot == -1);
}
