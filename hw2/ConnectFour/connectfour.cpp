#include "connectfour.h"
#include <string>
#include <sstream>
using std::string;

ConnectFour::ConnectFour(QWidget *parent) :
    QWidget(parent)
{

    QLabel *slot[10];
    int i;
    for(i = 0; i < 10; i++) {
        std::stringstream nstream;
        nstream << i+1;
        string label = "Slot " + nstream.str();
        slot[i] = new QLabel(tr(label.c_str()));
    }
}

ConnectFour::~ConnectFour()
{

}
