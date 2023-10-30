#include "homecontroller.h"

HomeController::HomeController(QObject *parent)
    : QObject{parent}, m_speed(0), m_status(false), m_direction(true),
      m_packTemp(0.0), m_motorTemp(0.0), m_stateOfCharge(0.0) {}

int HomeController::speed() const { return m_speed; }

void HomeController::setSpeed(int speed) {
  if (m_speed != speed) {
    m_speed = speed;
    emit speedChanged(speed);
  }
}

bool HomeController::status() const { return m_status; }

void HomeController::setStatus(bool status) {
  if (m_status != status) {
    m_status = status;
    emit statusChanged(status);
  }
}

bool HomeController::direction() const { return m_direction; }

void HomeController::setDirection(bool direction) {
  if (m_direction != direction) {
    m_direction = direction;
    emit directionChanged(direction);
  }
}

float HomeController::packTemp() const { return m_packTemp; }

void HomeController::setPackTemp(float packTemp) {
  if (m_packTemp != packTemp) {
    m_packTemp = packTemp;
    emit packTempChanged(packTemp);
  }
}

float HomeController::motorTemp() const { return m_motorTemp; }

void HomeController::setMotorTemp(float motorTemp) {
  if (m_motorTemp != motorTemp) {
    m_motorTemp = motorTemp;
    emit motorTempChanged(motorTemp);
  }
}

float HomeController::stateOfCharge() const { return m_stateOfCharge; }

void HomeController::setStateOfCharge(float stateOfCharge) {
  if (m_stateOfCharge != stateOfCharge) {
    m_stateOfCharge = stateOfCharge;
    emit stateOfChargeChanged(stateOfCharge);
  }
}
