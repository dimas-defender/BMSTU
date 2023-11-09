#include "elevator.h"

Elevator::Elevator(QObject* parent) : QObject(parent)
{
    QObject::connect(&cabin, SIGNAL(signalFloorReached(int, direction_t)),  &controller, SLOT(slotFloorReached(int, direction_t)));
    QObject::connect(&controller, SIGNAL(signalSendFloor(int)), &cabin, SLOT(slotSetTarget(int)));
    QObject::connect(&controller, SIGNAL(signalSendFloor(int)), &cabin, SLOT(slotMove()));
}

QWidget* Elevator::getWidget()
{
    return &controller;
}
