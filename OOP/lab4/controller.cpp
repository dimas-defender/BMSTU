#include "controller.h"

Controller::Controller(QWidget* parent) : QWidget (parent)
{
    layout = new QVBoxLayout;
    this->setLayout(layout);

    state = FREE;
    currentFloor = 0;
    direction = STILL;

    for (int i = 0; i < FLOOR_NUMB; i++)
    {
        buttons[i] = new ElevatorButton();
        buttons[i]->setText(QString::number(i + 1));
        buttons[i]->setFloorNumber(i);

        floorQueue[i] = false;

        layout->addWidget(dynamic_cast<QPushButton*>(buttons[i]));
        QObject::connect(buttons[i], SIGNAL(signalRequestFloor(int)), this, SLOT(slotAddFloorQueue(int)));
        QObject::connect(this, SIGNAL(signalGetNextFloor(int)), this, SLOT(slotCabinReady(int)));
    }
}

Controller::~Controller()
{
    for (int i = 0; i < FLOOR_NUMB; i++)
        delete buttons[i];

    delete layout;
}

void Controller::slotFloorReached(int floor, direction_t d)
{
    if (state == BUSY)
    {
        currentFloor = floor;
        direction = d;
        floorQueue[floor] = false;

        emit buttons[floor]->signalResetButton();
        state = FREE;

        emit signalGetNextFloor(floor);
    }
}

void Controller::slotCabinReady(int floor)
{
    if (state == FREE)
    {
        if (getNewFloor(floor))
            emit signalSendFloor(floor);
        else
            qDebug() << "test failed   ";
    }
}

void Controller::slotAddFloorQueue(int floor)
{
    floorQueue[floor] = true;
    emit signalGetNextFloor(floor);
}

bool Controller::getNewFloor(int& newFloor)
{
    int step = direction;

    if (direction == STILL)
        step = -1;

    for (int i = currentFloor; i < FLOOR_NUMB && i >= 0; i += step)
    {
        if (floorQueue[i])
        {
            newFloor = i;
            state = BUSY;
            return true;
        }
    }

    step *= -1;

    for (int i = currentFloor; i < FLOOR_NUMB && i >= 0; i += step)
    {
        if (floorQueue[i])
        {
            newFloor = i;
            state = BUSY;
            return true;
        }
    }
    return false;
}
