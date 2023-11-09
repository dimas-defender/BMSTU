#include "elevatorbutton.h"

ElevatorButton::ElevatorButton(QWidget *parent) : QPushButton (parent)
{
    state = PASSIVE;
    numberFloor = 0;

    QObject::connect(this, SIGNAL(clicked()),           this, SLOT(slotPress()));
    QObject::connect(this, SIGNAL(signalResetButton()), this, SLOT(slotReset()));
}

void ElevatorButton::slotPress()
{
    if (this->state == PASSIVE)
    {
        this->state = PRESSED;
        this->setEnabled(false);

        emit signalRequestFloor(numberFloor);
    }
}

void ElevatorButton::slotReset()
{
    if (this->state == PRESSED)
    {
        this->state = PASSIVE;
        this->setEnabled(true);
    }
}

void ElevatorButton::setFloorNumber(int floor)
{
    this->numberFloor = floor;
}
