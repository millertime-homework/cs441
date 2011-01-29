#include <QDebug>
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
        connect(this, SIGNAL(resetSlot(int)), slot[i], SLOT(resetSlot(int)));
        connect(slot[i], SIGNAL(pieceAdded(int)), this, SLOT(changeTurn(int)));
        slotLayout->addWidget(slot[i]);

        int j;
        for(j = 0; j < 10; j++)
            columns[i][j] = '-';
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

void ConnectThree::resetBoard()
{
    int i;
    for(i = 0; i < num_columns; i++) {
        emit resetSlot(i+1);  //signal the columns to reset

        int j;
        for(j = 0; j < 10; j++)
            columns[i][j] = '-';
    }
    whose_turn = "Black";
}

void ConnectThree::selectedSlot(int i)
{
    if (whose_turn == "Black")
        emit addPiece(i, whose_turn);
}

void ConnectThree::changeTurn(int i)
{
    updateBoardArray(i,whose_turn);
    if (checkForWin())
        return;
    if (whose_turn == "Black") {
        whose_turn = "Red";
        enemyTurn();
    }
    else {
        whose_turn = "Black";
    }
}

void ConnectThree::updateBoardArray(int column, QString player)
{
    char p = 'R';
    if (player == "Black")
        p = 'B';
    int i;
    for(i = 0; i < 10; i++) {
        if (columns[column-1][i] == '-') {
            columns[column-1][i] = p;
            break;
        }
    }
    //printArray();
}

void ConnectThree::printArray()
{
    QString slot1, slot2, slot3;
    int i;
    for(i = 0; i < 10; i++) {
        slot1 += columns[0][i];
        slot2 += columns[1][i];
        slot3 += columns[2][i];
    }
    qDebug() << slot1;
    qDebug() << slot2;
    qDebug() << slot3;
}

int ConnectThree::free_spaces(int column)
{
    int result = 0;
    int i;
    for(i = 0; i < 10; i++) {
        if (columns[column][i] == '-')
            result += 1;
    }
    return result;
}

void ConnectThree::enemyTurn()
{
    if (free_spaces(0) != 0 || free_spaces(1) != 0 || free_spaces(2) != 0) {
        int which_slot = rand() % 3;
        while(free_spaces(which_slot) == 0) {
            which_slot = ((which_slot+1) % 3);
        }
        emit addPiece(which_slot+1, "Red");
    } else {
        QMessageBox::information(this, "Full", "The board is all full!");
        resetBoard();
    }
}

char ConnectThree::matchAcross(int row)
{
    if (columns[0][row] != '-' &&
        ((columns[0][row] == columns[1][row]) &&
         (columns[1][row] == columns[2][row]))) {
        return columns[0][row];
    }
    else
        return '-';
}

char ConnectThree::matchDiagDown(int row)
{
    if (row > 7)
        return '-';   // avoid an array index error
    else if (columns[0][row+2] != '-' &&
             (columns[0][row+2] == columns[1][row+1] &&
              columns[1][row+1] == columns[2][row])) {
        return columns[0][row+2];
    }
    else
        return '-';
}

char ConnectThree::matchDiagUp(int row)
{
    if (row > 7)
        return '-';   // avoid an array index error
    else if (columns[0][row] != '-' &&
             (columns[0][row] == columns[1][row+1] &&
              columns[1][row+1] == columns[2][row+2])) {
        return columns[0][row];
    }
    else
        return '-';
}

char ConnectThree::matchUpDown(int row)
{
    int i;
    for(i = 0; i < 3; i++) {
        if (columns[i][row] != '-' &&
            (columns[i][row] == columns[i][row+1] &&
             columns[i][row+1] == columns[i][row+2])) {
            return columns[i][row];
        }
    }
    return '-';
}

bool ConnectThree::checkForWin()
{
    int i;
    char r;
    for(i = 0; i < 10; i++) {
        if ((r = matchAcross(i)) != '-') {
            announceWin(r);
            return true;
        }
        if ((r = matchDiagDown(i)) != '-') {
            announceWin(r);
            return true;
        }
        if ((r = matchDiagUp(i)) != '-') {
            announceWin(r);
            return true;
        }
        if ((r = matchUpDown(i)) != '-') {
            announceWin(r);
            return true;
        }
    }
    return false;
}

void ConnectThree::announceWin(char winner)
{
    if (winner == 'B')
        QMessageBox::information(this, "Winner!", "You win! Excellent!");
    else
        QMessageBox::information(this, "Loser", "Ha ha ha ha ha you lost. You're a failure.");
    resetBoard();
}
