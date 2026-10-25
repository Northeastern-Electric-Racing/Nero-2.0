#include "homecontroller.h"

HomeController::HomeController(Model *model, QObject *parent)
    : ButtonController{model, parent}, m_speed(0), m_status(false),
      m_direction(true), m_packTemp(0.0), m_motorTemp(0.0),
      m_stateOfCharge(0.0) {
  connect(m_model, &Model::onCurrentDataChange, this,
          &HomeController::currentDataDidChange);
}

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

int HomeController::stateOfCharge() const { return m_stateOfCharge; }

void HomeController::setStateOfCharge(int charge) {
  if (charge != m_stateOfCharge) {
    m_stateOfCharge = charge;
    emit stateOfChargeChanged(charge);
  }
}

float HomeController::lowVoltage() const { return m_lowVoltage; }

void HomeController::setLowVoltage(float voltage) {
  if (voltage != m_lowVoltage) {
    m_lowVoltage = voltage;
    emit lowVoltageChanged(voltage);
  }
}

void HomeController::rightButtonPressed() { emit toggleFaultAlertsRequested(); }

void HomeController::currentDataDidChange() {
  setPackTemp(m_model->getPackTemp().value_or(0));
  setMotorTemp(m_model->getMotorTemp().value_or(0));
  setDirection(m_model->getDir().value_or(1));
  setStateOfCharge(m_model->getStateOfCharge().value_or(0));
  setSpeed(m_model->getMph().value_or(0));
  setStatus(m_model->getStatus().value_or(0));
  setLowVoltage(m_model->getLowVoltage().value_or(0));
}
