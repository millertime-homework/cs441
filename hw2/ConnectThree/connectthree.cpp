#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "connectthree.h"
#include "slotbutton.h"
#include "pieceslot.h"

using std::string;

ConnectThree::ConnectThree(QWidget *parent) :
    QWidget(parent),num_columns(3),whose_turn("Black")
{
    srand(time(NULL));  //seed my random turn generator

    int n = 10;   // number_of_pieces_per_column

    SlotButton *slotButton[num_columns];
    PieceSlot *slot[num_columns];
    QHBoxLayout *slotButtonLayout = new QHBoxLayout;
    QHBoxLayout *slotLayout = new QHBoxLayout;
    int i;
    for(i = 0; i < num_columns; i++) {
        std::stringstream nstream;
        nstream << i+1;
        string label = "Slot &" + nstream.str();
        slotButton[i] = new SlotButton(tr(label.c_str()), i+1);
        slotButton[i]->show();
        connect(slotButton[i], SIGNAL(gotClicked(int)), this, SLOT(selectedSlot(int)));
        slotButtonLayout->addWidget(slotButton[i]);

        slot[i] = new PieceSlot(i+1, n);
        connect(this, SIGNAL(addPiece(int,QString)), slot[i], SLOT(maybeAddPiece(int,QString)));
        connect(slot[i], SIGNAL(pieceAdded(int)), this, SLOT(changeTurn(int)));
        slotLayout->addWidget(slot[i]);
    }

    quitButton = new QPushButton(tr("&Quit"));
    quitButton->show();
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(quitButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(slotButtonLayout, 0, 0);
    mainLayout->addLayout(slotLayout, 1, 0);
    mainLayout->addLayout(buttonLayout, 2, 0);

    setLayout(mainLayout);
    setWindowTitle(tr("Connect Three"));
}

ConnectThree::~ConnectThree()
{
    delete quitButton;
}

void ConnectThree::selectedSlot(int i)
{
    if (whose_turn == "Black")
        emit addPiece(i, whose_turn);
}

void ConnectThree::changeTurn(int i)
{
    if (whose_turn == "Black") {
        whose_turn = "Red";
        enemyTurn();
    }
    else {
        whose_turn = "Black";
    }
    --free_spaces[i-1];
}

void ConnectThree::enemyTurn()
{
    if (free_spaces[0] != 0 && free_spaces[1] != 0 && free_spaces[2] != 0) {
        int which_slot = rand() % 3;
        while(free_spaces[which_slot] == 0)
            which_slot = rand() %3;
        emit addPiece(which_slot+1, "Red");
    }
}
