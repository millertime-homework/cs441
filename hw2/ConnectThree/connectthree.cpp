#include <QDebug>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "connectthree.h"
#include "slotbutton.h"
#include "pieceslot.h"
#include "threadedminimax.h"

using std::string;
using std::min;
using std::max;

ConnectThree::ConnectThree(QWidget *parent) :
    QWidget(parent),num_columns(3),whose_turn("Red")
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
    enemyTurn();
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
    whose_turn = "Red";
    enemyTurn();
}

void ConnectThree::selectedSlot(int i)
{
    if (whose_turn == "Black")
        emit addPiece(i, whose_turn);
}

void ConnectThree::changeTurn(int i)
{
    updateBoardArray(i,whose_turn);
    int status = eval(columns, 'R');
    if (status == 2) {
        announceWin('R');
        return;
    } else if (status == -2) {
        announceWin('B');
        return;
    } else if (status == 1) {
        announceWin('D');   // draw
        return;
    }
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
    //printArray(columns);
}

void ConnectThree::printArray(char board[3][10])
{
    QString slot1, slot2, slot3;
    int i;
    for(i = 0; i < 10; i++) {
        slot1 += board[0][i];
        slot2 += board[1][i];
        slot3 += board[2][i];
    }
    qDebug() << slot1;
    qDebug() << slot2;
    qDebug() << slot3;
}

void ConnectThree::enemyTurn()
{
    // start new threads for each possible move to get minimax value.
    ThreadedMinimax minimaxes[3];

    // keep track of full columns
    bool is_full[3];

    int i;
    for(i = 0; i < 3; i++) {
      // only try on columns that are available
      if (columns[i][9] == '-') {
	is_full[i] = false;
	// have to insert into the temp board before minimaxing
	char new_board[3][10];
	int k,l;
	for(k = 0; k < 3; k++) {
	  for(l = 0; l < 10; l++) {
	    new_board[k][l] = columns[k][l];
	  }
	}
	int ns = nextSlot(new_board, i);
        new_board[i][ns] = 'R';
        minimaxes[i].init(this, i, new_board);
      }
    }

    // start the threads
    for(i = 0; i < 3; i++) {
      if (!is_full[i])
	minimaxes[i].start();
    }
    
    // wait for the threads to finish
    for(i = 0; i < 3; i++) {
      if (!is_full[i])
	minimaxes[i].wait();
    }

    // get the best value
    int best = -9000;
    int which_column;
    for(i = 0; i < 3; i++) {
      if (!is_full[i]) {
	int mm = minimaxes[i].result();
	if (mm > best) {
	  best = mm;
          which_column = minimaxes[i].getCol();
	}
      }
    }
    // now actually play, based on the best minimax result
    emit addPiece(which_column+1, "Red");
}

int ConnectThree::nextSlot(char board[3][10], int column)
{
    int i;
    for(i = 0; i < 10; i++) {
        if (board[column][i] == '-') {
            return i;
        }
    }
    return 0;  // shouldn't reach here
}

char ConnectThree::matchAcross(int row, char board[3][10])
{
    if (board[0][row] != '-' &&
        ((board[0][row] == board[1][row]) &&
         (board[1][row] == board[2][row]))) {
        return board[0][row];
    }
    else
        return '-';
}

char ConnectThree::matchDiagDown(int row, char board[3][10])
{
    if (row > 7)
        return '-';   // avoid an array index error
    else if (board[0][row+2] != '-' &&
             (board[0][row+2] == board[1][row+1] &&
              board[1][row+1] == board[2][row])) {
        return board[0][row+2];
    }
    else
        return '-';
}

char ConnectThree::matchDiagUp(int row, char board[3][10])
{
    if (row > 7)
        return '-';   // avoid an array index error
    else if (board[0][row] != '-' &&
             (board[0][row] == board[1][row+1] &&
              board[1][row+1] == board[2][row+2])) {
        return board[0][row];
    }
    else
        return '-';
}

char ConnectThree::matchUpDown(int row, char board[3][10])
{
    if (row > 7)
        return '-';
    int i;
    for(i = 0; i < 3; i++) {
        if (board[i][row] != '-' &&
            (board[i][row] == board[i][row+1] &&
             board[i][row+1] == board[i][row+2])) {
            return board[i][row];
        }
    }
    return '-';
}

int ConnectThree::eval(char board[3][10], char good_guy)
{
    int i;
    char r, bad_guy;
    if (good_guy == 'B')
        bad_guy = 'R';
    else
        bad_guy = 'B';
    for(i = 0; i < 10; i++) {
        // X X X
        if ((r = matchAcross(i, board)) != '-') {
            if (r == bad_guy)
                return -2;  // bad guy won
            else if (r == good_guy)
                return 2;   // good guy won
        }
        // X
        //   X
        //     X
        if ((r = matchDiagDown(i, board)) != '-') {
            if (r == bad_guy)
                return -2;
            else if (r == good_guy)
                return 2;
        }
        //     X
        //   X
        // X
        if ((r = matchDiagUp(i, board)) != '-') {
            if (r == bad_guy)
                return -2;
            else if (r == good_guy)
                return 2;
        }
        // X
        // X
        // X
        if ((r = matchUpDown(i, board)) != '-') {
            if (r == bad_guy)
                return -2;
            else if (r == good_guy)
                return 2;
        }
        // last slot in each column taken? board full. draw.
        if ((board[0][9] != '-') && (board[1][9] != '-') && (board[2][9] != '-')) {
            return 1;
        }
    }
    return 0;
}

void ConnectThree::announceWin(char winner)
{
    if (winner == 'B')
        QMessageBox::information(this, "Winner!", "You win! Excellent!");
    else if (winner == 'R')
        QMessageBox::information(this, "Loser", "Ha ha ha ha ha you lost. You're a failure.");
    else
        QMessageBox::information(this, "Draw", "Filled the board! Draw!");
    resetBoard();
}
