#ifndef CABIN_H
#define CABIN_H

#include <QObject>

#include "timer.h"
#include "doors.h"

class Cabin : public QObject
{
    Q_OBJECT

    enum stateCabin
    {
        MOVE,
        BUSY,
        FREE
    };

public:
    Cabin(QObject* parent = nullptr);

signals:
    void signalFloorReached(int floor, direction_t d);
    void signalCabinStopped();
    void signalFloorAchieved();
    void signalDoors();

public slots:
    void slotMove();
    void slotBusy();
    void slotFree();
    void slotSetTarget(int floor);

private:
    int currentFloor = 0;
    int targetFloor;

    direction_t direction;
    stateCabin state;
    Doors doors;

    QTimer timeMove;
};

#endif // CABIN_H
