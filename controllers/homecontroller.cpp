#include "homecontroller.h"

enum Direction = {forward, reverse};
enum Status = {on, off};

HomeController::HomeController()
{
public:
    int speed = 0;
    Status status = off;
    Direction direction = forward;
    int packTemp = 0;
    int motorTemp = 0;
    int stateOfCharge = 0;
};

void HomeController::updateSpeed(int newSpeed) 
{
    speed = newSpeed;
}

void HomeController::updateStatus(Status newStatus)
{
    status = newStatus;
}

void HomeController::updateDirection(Direction newDirection)
{
    direction = newDirection;
}

void HomeController::updatePackTemp(int newPackTemp)
{
    packTemp = newPackTemp;
}

void HomeController::updateMotorTemp(int newMotorTemp)
{
    motorTemp = newMotorTemp;
}

void HomeController::updateStateOfCharge(int newStateOfCharge)
{
    stateOfCharge = newStateOfCharge;
}