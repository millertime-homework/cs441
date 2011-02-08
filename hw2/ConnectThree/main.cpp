/* Copyright (c) 2011 Russell Miller
MIT License - please see included COPYING file
or visit www.opensource.org/licenses/mit-license

main.cpp
Qt generates this file. It's boilerplate.
*/

#include <QtGui>
#include "connectthree.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectThree w;
    w.show();

    return a.exec();
}
