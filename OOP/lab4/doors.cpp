#include "doors.h"

Doors::Doors()
{
    state = CLOSED;

    QObject::connect(&timeOpen,     SIGNAL(timeout()),          this, SLOT(slotDoorOpen()));
    QObject::connect(&timeClose,    SIGNAL(timeout()),          this, SLOT(slotDoorClose()));
    QObject::connect(&timeWait,     SIGNAL(timeout()),          this, SLOT(slotDoorClosing()));
}


void Doors::slotDoorOpen()
{
    if (state == OPENING)
    {
        state = OPENED;
        qDebug() << "Doors opened";

        timeWait.start(TIME_DOOR_WAIT);
    }
}

void Doors::slotDoorOpening()
{
    if (state == CLOSED)
    {
        state = OPENING;
        qDebug() << "Doors opening...";

        timeOpen.start(TIME_DOOR_PROC);
    }
    if (state == CLOSING)
    {
        state = OPENING;
        qDebug() << "Doors opening...";

        int time = timeClose.remainingTime();

        timeClose.stop();
        timeOpen.start(TIME_DOOR_PROC - time);
    }
}

void Doors::slotDoorClose()
{
    if (state == CLOSING)
    {
        state = CLOSED;
        qDebug() << "Doors closed";
    }
}

void Doors::slotDoorClosing()
{
    if (state == OPENED)
    {
        state = CLOSING;
        qDebug() << "Doors closing...";

        timeClose.start(TIME_DOOR_PROC);
    }
}
