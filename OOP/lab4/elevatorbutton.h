#ifndef ELEVATORBUTTON_H
#define ELEVATORBUTTON_H

#include <QObject>
#include <QDebug>
#include <QPushButton>

class ElevatorButton : public QPushButton
{
    Q_OBJECT

    enum stateButton
    {
        PRESSED,
        PASSIVE
    };

public:
    ElevatorButton(QWidget* parent = nullptr);
    void setFloorNumber(int floor);

public slots:
    void slotPress();
    void slotReset();

signals:
    void signalResetButton();
    void signalRequestFloor(int floor);

private:
    stateButton state;
    int numberFloor;
};

#endif // ELEVATORBUTTON_H
