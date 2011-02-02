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
