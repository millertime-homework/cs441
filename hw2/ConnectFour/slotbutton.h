#ifndef SLOTBUTTON_H
#define SLOTBUTTON_H

#include <QPushButton>

class SlotButton : public QPushButton
{
    Q_OBJECT

signals:
    void gotClicked(int);

public:
    SlotButton(const QString &, int, QWidget *parent = 0);

protected:
    void mouseReleaseEvent(QMouseEvent * event);

private:
    int my_id;
};

#endif // SLOTBUTTON_H
