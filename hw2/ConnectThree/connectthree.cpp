/* Copyright (c) 2011 Russell Miller
MIT License - please see included COPYING file
or visit www.opensource.org/licenses/mit-license

connectthree.cpp
Create window for game to be played in
Create widgets and layouts in that window
Control the flow of the game
Run the 3x4 proof
*/

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

// Constructor to build up the application
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

    testButton = new QPushButton(tr("&Prove 3x4"));
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
    enemyTurn();   // start the game with the enemy's turn
    //testBoard(); // for doing some individual game state tests
}

ConnectThree::~ConnectThree()
{
    delete quitButton;
    delete testButton;
}

// Slot that responds to the mouse clicks on the SlotButtons
void ConnectThree::selectedSlot(int i)
{
    if (whose_turn == 'B')
        emit addPiece(i, whose_turn);  // don't let them click out of turn
}

// Slot that responds to a piece getting added to a PieceSlot
void ConnectThree::changeTurn(int i)
{
    gameboard->insert(i,whose_turn);   // update the array
    int status = gameboard->eval('R'); // get current score
    if (status == 2) {
        announceWin('R');              // a win?
        return;
    } else if (status == -2) {
        announceWin('B');
        return;
    } else if (status == 1) {
        announceWin('D');              // draw
        return;
    }
    if (whose_turn == 'B') {           // or just switch players
        whose_turn = 'R';
        enemyTurn();                   // allow enemy to play
    }
    else {
        whose_turn = 'B';
    }
}

// For debugging
void ConnectThree::printArray(Board & board)
{
    string arraystring = board.toString();
    qDebug() << arraystring.c_str();
}

// Starting point for the computer opponent
void ConnectThree::enemyTurn()
{
    int which_slot = 0;      // we'll store the return value here
    int d1 = gameboard->findDeuce('R'); // see if we can get immediate win
    int d2 = gameboard->findDeuce('B'); // see if we can block immediate win
    if (d1 != -1)
        which_slot = d1;  // crush them!
    else if (d2 != -1)
        which_slot = d2;  // defer!
    else {
        int i;               // iterate the columns
        int best_move = -9000;
        // get minimax value for each possible move.
        for(i = 1; i < 4; i++) {
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
    }
    // now actually play, based on the best minimax result
    emit addPiece(which_slot, 'R');
}

// "Max" part of minimax. Find my best play.
int ConnectThree::tryRed(Board & board, int depth)
{
    int status = 0 - board.eval('B');  // Make it negative! Or die!
    if ((status != 0) || (depth == 0))
        return status;     // game already over, just return.
    int returnval = -9000;   // it can't be OVER 9000 can it?!
    int i;
    // try to play each column
    for(i = 1; i < 4; i++) {
        if (!(board.isColFull(i))) {
            // make a new board to try moves on
            Board new_board(board);
            new_board.insert(i, 'R');
            // now continue onward and play for the other guy
            int score = tryBlack(new_board, depth-1);
            if (score > returnval) {
                returnval = score;
            }
        }
    }
    return returnval;
}

// "Min" part of minimax. Find their best play (my worst).
int ConnectThree::tryBlack(Board & board, int depth)
{
    int status = board.eval('R');
    if ((status != 0) || (depth == 0)) {
        return status;
    }
    int returnval = 9000;
    int i;
    for(i = 1; i < 4; i++) {
        if (!(board.isColFull(i))) {
            Board new_board(board);
            new_board.insert(i, 'B');
            int score = tryRed(new_board, depth-1);
            // Here's the difference. Get minimum.
            if (score < returnval) {
                returnval = score;
            }
        }
    }
    return returnval;
}

// Take care of the endgame.
void ConnectThree::announceWin(char winner)
{
    if (winner == 'B')
        QMessageBox::information(this, "Winner!", "You win! Excellent!");
    else if (winner == 'R')
        QMessageBox::information(this, "Loser", "Ha ha ha ha ha you lost. You're a failure.");
    else
        QMessageBox::information(this, "Draw", "Filled the board! Draw!");
    gameboard->clear();  // clear the array
    int i;
    for(i = 1; i < 4; i ++)
        emit resetSlot(i);  // clear the gui
    whose_turn = 'R';   // start over the turns
    enemyTurn();
}

// "proof" button got clicked
void ConnectThree::canHasProof()
{
    proof();
}

// This is the proof that on a 3x4 board using a best-move
// algorithm, the game is a tie.
void ConnectThree::proof()
{
    // First we'll need a 3x4 board
    Board proofboard(4);
    // Now Red will go first and until the game is over, we'll continue getting minimax values
    char player = 'R';
    while (proofboard.eval('R') == 0) {
        qDebug() << "Playing " << player << "'s turn.";
        printArray(proofboard);
        int minimax = proofTakeTurn(proofboard, (char)player);  // get minimax value
        proofboard.insert(minimax, player);    //play it
        player = (player == 'R') ? 'B' : 'R';  // switch player
    }
    if (proofboard.eval('R') != 1)
        QMessageBox::information(this, "IDIOT!", "It wasn't a tie!"); // when it breaks..
    QMessageBox::information(this, "Done", "QED.");
    qDebug() << "Final state of board:";
    printArray(proofboard);
}

// This is a helper for the proof. It tries the 3 possible columns and
// responds with the best
int ConnectThree::proofTakeTurn(Board & board, char player)
{
    int col; // going to be iterated
    int which_col = 1;  // going to be best col, which will be returned
    int best_score;
    if (player == 'R')
        best_score = -9000;  // using the same minimax.. this player is the "max"
    else
        best_score = 9000;   // and this one is the "min"
    int minimax;
    // try each column
    for(col = 1; col < 4; col++) {
        if (!(board.isColFull(col))) {
            // copy current game state to new array
            Board new_board(board);
            // add a piece
            new_board.insert(col, player);
            if (player == 'R') {
                // evaluate rest of game
                minimax = tryBlack(new_board, 30);
                if (minimax > best_score) {
                    best_score = minimax;
                    which_col = col;
                }
            } else {
                minimax = tryRed(new_board, 30);
                // minimum
                if (minimax < best_score) {
                    best_score = minimax;
                    which_col = col;
                }
            }
        }
    }
    return which_col;
}

void ConnectThree::testBoard()
{
    Board test(10);
    test.insert(1,'R');
    test.insert(1,'R');
    test.insert(1,'B');
    test.insert(1,'R');
    test.insert(1,'R');
    test.insert(1,'B');
    test.insert(1,'R');
    test.insert(1,'R');
    test.insert(1,'B');
    test.insert(1,'R');

    test.insert(3,'B');
    test.insert(3,'B');
    test.insert(3,'R');
    test.insert(3,'B');
    test.insert(3,'B');
    Board testone(test);
    testone.insert(1, 'B');
    int one = tryRed(testone, 10);
    qDebug() << "Column 1: " << one;
    Board testtwo(test);
    testtwo.insert(2, 'B');
    int two = tryRed(testtwo, 10);
    qDebug() << "Column 2: " << two;
    Board testthree(test);
    testthree.insert(3, 'B');
    int three = tryRed(testthree, 10);
    qDebug() << "Column 3: " << three;
}
