#include <QtGui>
#include "connectthree.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectThree w;
    w.show();

    return a.exec();
}
