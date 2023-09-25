#include "homecontroller.h"

homeController::homeController(QObject *parent)
    : QObject{parent}
    , m_speed(0)
    , m_status(false)
    , m_direction(true)
    , m_packTemp(0.0)
    , m_motorTemp(0.0)
{}

int homeController::speed() const
{
    return m_speed;
}

void homeController::setSpeed(int speed)
{
    if (m_speed != speed) {
        m_speed = speed;
        emit speedChanged(speed);
    }
}


bool homeController::status() const
{
    return m_status;
}

void homeController::setStatus(bool status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged(status);
    }
}


bool homeController::direction() const
{
    return m_direction;
}

void homeController::setDirection(bool direction)
{
    if (m_direction != direction) {
        m_direction = direction;
        emit directionChanged(direction);
    }
}


float homeController::packTemp() const
{
    return m_packTemp;
}

void homeController::setPackTemp(float packTemp)
{
    if (m_packTemp != packTemp) {
        m_packTemp = packTemp;
        emit packTempChanged(packTemp);
    }
}


float homeController::motorTemp() const
{
    return m_motorTemp;
}

void homeController::setMotorTemp(float motorTemp)
{
    if (m_motorTemp != motorTemp) {
        m_motorTemp = motorTemp;
        emit motorTempChanged(motorTemp);
    }
}
