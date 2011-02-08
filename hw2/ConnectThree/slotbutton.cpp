/* Copyright (c) 2011 Russell Miller
MIT License - please see included COPYING file
or visit www.opensource.org/licenses/mit-license

slotbutton.cpp
Override the mouse click to send an id number
so I know which button got clicked
*/

#include <QDebug>
#include "slotbutton.h"

SlotButton::SlotButton(const QString & text, int i, QWidget *parent) :
    QPushButton(text, parent), my_id(i)
{
}

void SlotButton::mouseReleaseEvent(QMouseEvent * event)
{
    if (!event)
        return;
    emit gotClicked(my_id);
}
