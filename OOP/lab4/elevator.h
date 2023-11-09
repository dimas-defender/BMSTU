#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>

#include "timer.h"
#include "controller.h"
#include "cabin.h"

class Elevator : public QObject
{
    Q_OBJECT

public:
    Elevator(QObject* parent = nullptr);
    QWidget* getWidget();

private:
    Controller controller;
    Cabin cabin;
};

#endif // ELEVATOR_H
