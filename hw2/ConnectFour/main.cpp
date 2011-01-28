#include <QtGui>
#include "connectfour.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectFour w;
    w.show();

    return a.exec();
}
