#include <QDebug>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "connectthree.h"
#include "slotbutton.h"
#include "pieceslot.h"

using std::string;
using std::min;
using std::max;

// note that Black goes first, human is Black player.
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
    int which_slot = 0;      // we'll store the return value here
    int i;               // iterate the columns
    int best_move = -9000;
    // get minimax value for each possible move.
    for(i = 0; i < 3; i++) {
        // only try on columns that are available
        if (columns[i][9] == '-') {
            // have to insert into the temp board before minimaxing
            char new_board[3][10];     // temporary board to evaluate
            int k,l;
            for(k = 0; k < 3; k++) {
                for(l = 0; l < 10; l++) {
                    new_board[k][l] = columns[k][l];   // copying whole board for evaluating
                }
            }
            int ns = nextSlot(new_board, i);
            new_board[i][ns] = 'R';
            // now start trying to add enemy pieces and go from there...
            int score = tryBlack(new_board, 10);
            if (score > best_move) {
                // get the best result, store it..
                best_move = score;
                which_slot = i;
            }
        }
    }
    // now actually play, based on the best minimax result
    emit addPiece(which_slot+1, "Red");
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

int ConnectThree::tryRed(char board[3][10], int depth)
{
    int status = 0 - eval(board, 'B');
    if ((status != 0) || (depth == 0))
        return status;
    int returnval = -9000;   // it can't be OVER 9000 can it?!
    int i;   // iterator for columns
    for(i = 0; i < 3; i++) {
        if (board[i][9] == '-') {  // last slot still available, not full
            char new_board[3][10];
            int j,k;
            for(j = 0; j < 3; j++) {
                for(k = 0; k < 10; k++) {
                    new_board[j][k] = board[j][k];
                }
            }
            int ns = nextSlot(new_board, i);
            new_board[i][ns] = 'R';
            int score = tryBlack(new_board, depth-1);  // recurse
            if (score > returnval) {
                returnval = score;
            }
        }
    }
    return returnval;
}

int ConnectThree::tryBlack(char board[3][10], int depth)
{
    int status = eval(board, 'R');
    if ((status != 0) || (depth == 0))
        return status;
    int returnval = 9000;
    int i;
    for(i = 0; i < 3; i++) {
        if (board[i][9] == '-') {
            char new_board[3][10];
            int j,k;
            for(j = 0; j < 3; j++) {
                for(k = 0; k < 10; k++) {
                    new_board[j][k] = board[j][k];
                }
            }
            int ns = nextSlot(new_board, i);
            new_board[i][ns] = 'B';
            int score = tryRed(new_board, depth-1);
            if (score < returnval) {
                returnval = score;
            }
        }
    }
    return returnval;
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
