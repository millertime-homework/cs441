#include <QDebug>
#include <algorithm>
#include <sstream>
#include <string>
#include "connectthree.h"
#include "slotbutton.h"
#include "pieceslot.h"

using std::string;
using std::min;
using std::max;

ConnectThree::ConnectThree(QWidget *parent) :
    QWidget(parent),whose_turn('R')
{
    SlotButton *slotButton[3];
    PieceSlot *slot[3];
    QHBoxLayout *slotButtonLayout = new QHBoxLayout;
    QHBoxLayout *slotLayout = new QHBoxLayout;
    int i;
    for(i = 0; i < 3; i++) {
        std::stringstream nstream;
        nstream << i+1;
        string label = "Slot &" + nstream.str();
        slotButton[i] = new SlotButton(tr(label.c_str()), i+1);
        slotButton[i]->show();
        connect(slotButton[i], SIGNAL(gotClicked(int)), this, SLOT(selectedSlot(int)));
        slotButtonLayout->addWidget(slotButton[i]);

        slot[i] = new PieceSlot(i+1, 10);
        connect(this, SIGNAL(addPiece(int,char)), slot[i], SLOT(maybeAddPiece(int,char)));
        connect(this, SIGNAL(resetSlot(int)), slot[i], SLOT(resetSlot(int)));
        connect(slot[i], SIGNAL(pieceAdded(int)), this, SLOT(changeTurn(int)));
        slotLayout->addWidget(slot[i]);
    }

    quitButton = new QPushButton(tr("&Quit"));
    quitButton->show();
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    testButton = new QPushButton(tr("&Run Tests..."));
    testButton->show();
    connect(testButton, SIGNAL(clicked()), this, SLOT(canHasProof()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(testButton);
    buttonLayout->addWidget(quitButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(slotButtonLayout, 0, 0);
    mainLayout->addLayout(slotLayout, 1, 0);
    mainLayout->addLayout(buttonLayout, 2, 0);

    setLayout(mainLayout);
    setWindowTitle(tr("Connect Three"));
    gameboard = new Board(10);
    enemyTurn();
}

ConnectThree::~ConnectThree()
{
    delete quitButton;
    delete testButton;
}

void ConnectThree::selectedSlot(int i)
{
    if (whose_turn == 'B')
        emit addPiece(i, whose_turn);
}

void ConnectThree::changeTurn(int i)
{
    gameboard->insert(i,whose_turn);
    int status = gameboard->eval('R');
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
    if (whose_turn == 'B') {
        whose_turn = 'R';
        enemyTurn();
    }
    else {
        whose_turn = 'B';
    }
}

void ConnectThree::printArray(Board & board)
{
    string arraystring = board.toString();
    qDebug() << arraystring.c_str();
}

void ConnectThree::enemyTurn()
{
    int which_slot = 0;      // we'll store the return value here
    int i;               // iterate the columns
    int best_move = -9000;
    // get minimax value for each possible move.
    for(i = 0; i < 3; i++) {
        // only try on columns that are available
        if (!(gameboard->isColFull(i))) {
            // have to insert into the temp board before minimaxing
            Board new_board(*gameboard);     // temporary board to evaluate
            new_board.insert(i,'R');
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
    emit addPiece(which_slot+1, 'R');
}

int ConnectThree::tryRed(Board & board, int depth)
{
    int status = 0 - board.eval('B');
    if ((status != 0) || (depth == 0))
        return status;
    int returnval = -9000;   // it can't be OVER 9000 can it?!
    int i;   // iterator for columns
    for(i = 0; i < 3; i++) {
        if (board.isColFull(i)) {  // last slot still available, not full
            Board new_board(board);
            new_board.insert(i, 'R');
            int score = tryBlack(new_board, depth-1);  // recurse
            if (score > returnval) {
                returnval = score;
            }
        }
    }
    return returnval * (depth+1);
}

int ConnectThree::tryBlack(Board & board, int depth)
{
    int status = board.eval('R');
    if ((status != 0) || (depth == 0))
        return status;
    int returnval = 9000;
    int i;
    for(i = 0; i < 3; i++) {
        if (board.isColFull(i)) {
            Board new_board(board);
            new_board.insert(i, 'B');
            int score = tryRed(new_board, depth-1);
            if (score < returnval) {
                returnval = score;
            }
        }
    }
    return returnval * (depth+1);
}

void ConnectThree::announceWin(char winner)
{
    if (winner == 'B')
        QMessageBox::information(this, "Winner!", "You win! Excellent!");
    else if (winner == 'R')
        QMessageBox::information(this, "Loser", "Ha ha ha ha ha you lost. You're a failure.");
    else
        QMessageBox::information(this, "Draw", "Filled the board! Draw!");
    gameboard->clear();
}

void ConnectThree::canHasProof()
{
    proof();
}

/* This is the proof that on a 3x4 board using a best-move
algorithm, the game is always a tie. */
void ConnectThree::proof()
{
    // First I'll need a 3x4 board
    Board proofboard(4);
    // Now Red will go first and until the game is over, we'll continue getting minimax values
    char player = 'R';
    while (proofboard.eval('R') == 0) {
        int minimax = proofTakeTurn(proofboard, (char)player);  // get minimax value
        proofboard.insert(minimax, player);    //play it
        player = (player == 'R') ? 'B' : 'R';  // switch player
    }
    if (proofboard.eval('R') != 1)
        QMessageBox::information(this, "IDIOT!", "It wasn't a tie!");
    QMessageBox::information(this, "Done", "QED.");
}

/* This is a helper for my proof. It tries the 3 possible columns and
   responds with the best */
int ConnectThree::proofTakeTurn(Board & board, char player)
{
    int col; // going to be iterated
    int which_col;  // going to be best col, which will be returned
    int best_score = -9000;
    int minimax;
    // try each column
    for(col = 0; col < 3; col++) {
        // copy current game state to new array
        Board new_board(board);
        // add a piece
        new_board.insert(col, player);
        if (player == 'R') {
            minimax = tryBlack(new_board, 10);
            if (minimax > best_score) {
                best_score = minimax;
                which_col = col;
            }
        } else {
            minimax = tryRed(new_board, 10);
            if (minimax > best_score) {
                best_score = minimax;
                which_col = col;
            }
        }
    }
    return which_col;
}
