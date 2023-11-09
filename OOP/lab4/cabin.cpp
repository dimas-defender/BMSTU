#include "cabin.h"

Cabin::Cabin(QObject* parent) : QObject(parent)
{
    state = FREE;

    currentFloor = 0;
    targetFloor = 0;

    direction = STILL;

    QObject::connect(&timeMove, SIGNAL(timeout()),              this,   SLOT(slotMove()));
    QObject::connect(this,      SIGNAL(signalFloorAchieved()),  this,   SLOT(slotBusy()));
    QObject::connect(this,      SIGNAL(signalDoors()),  &doors, SLOT(slotDoorOpening()));
    QObject::connect(this,      SIGNAL(signalCabinStopped()),  this, SLOT(slotFree()));
}


void Cabin::slotMove()
{
    if (state == MOVE)
    {
        state = MOVE;

        if (currentFloor == targetFloor)
            emit signalFloorAchieved();
        else
        {
            qDebug() << "Moving from floor   " << currentFloor + 1;

            if (direction == UP)
                currentFloor++;
            else
                currentFloor--;

            timeMove.start(TIME_FLOOR_MOVE);
        }
    }
    else if (state == FREE)
    {
        state = MOVE;

        if (currentFloor == targetFloor)
            emit signalFloorAchieved();
        else
            timeMove.start(TIME_FLOOR_MOVE);
    }
}

void Cabin::slotBusy()
{
    if (state == MOVE)
    {
        state = BUSY;
        qDebug() << "Stopped on the floor " << currentFloor + 1;
        emit signalCabinStopped();
    }
}

void Cabin::slotFree()
{
    if (state == BUSY)
    {
        emit signalDoors();
        state = FREE;
        emit signalFloorReached(currentFloor, direction);
    }
}

void Cabin::slotSetTarget(int floor)
{
    targetFloor = floor;
    if (currentFloor < targetFloor)
        direction = UP;
    else
        direction = DOWN;
}
