#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

#include "timer.h"
#include "elevatorbutton.h"

class Controller : public QWidget
{
    Q_OBJECT

    enum stateController
    {
        FREE,
        BUSY
    };

public:
    Controller(QWidget *parent = nullptr);
    ~Controller();

signals:
    void signalSendFloor(int floor);
    void signalGetNextFloor(int floor);

public slots:
    void slotFloorReached(int floor, direction_t d);
    void slotCabinReady(int floor);

private slots:
    void slotAddFloorQueue(int floor);

private:
    bool getNewFloor(int& newFloor);

    stateController state;
    int currentFloor;
    direction_t direction;

    bool floorQueue[FLOOR_NUMB];
    ElevatorButton* buttons[FLOOR_NUMB];
    QVBoxLayout* layout;
};

#endif // CONTROLLER_H
