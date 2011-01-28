#include <string>
#include <sstream>
#include "connectfour.h"
#include "slotbutton.h"
#include "pieceslot.h"

using std::string;

ConnectFour::ConnectFour(QWidget *parent) :
    QWidget(parent),num_columns(3),whose_turn("Black")
{
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
    setWindowTitle(tr("Connect Four"));
}

ConnectFour::~ConnectFour()
{
    delete quitButton;
}

//TODO: don't change turn every time there's a click!
void ConnectFour::selectedSlot(int i)
{
    emit addPiece(i, whose_turn);
    changeTurn();
}

void ConnectFour::changeTurn()
{
    if (whose_turn == "Black")
        whose_turn = "Red";
    else
        whose_turn = "Black";
}
