#include "efficiencycontroller.h"

EfficiencyController::EfficiencyController(Model *model, QObject *parent)
    : ButtonController{model, 2, parent} {
  connect(m_model, &Model::onCurrentDataChange, this,
          &EfficiencyController::currentDataDidChange);
}

int EfficiencyController::currentMaxTorque() const {
  return m_currentMaxTorque;
}
void EfficiencyController::setCurrentMaxTorque(int torque) {
  if (torque != m_currentMaxTorque) {
    m_currentMaxTorque = torque;
    emit currentMaxTorqueChanged(torque);
  }
}

int EfficiencyController::currentRegenStrength() const {
  return m_currentRegenStrength;
}
void EfficiencyController::setCurrentRegenStrength(int strength) {
  if (strength != m_currentRegenStrength) {
    m_currentRegenStrength = strength;
    emit currentRegenStrengthChanged(strength);
  }
}

int EfficiencyController::stateOfCharge() const { return m_stateOfCharge; }
void EfficiencyController::setStateOfCharge(int charge) {
  if (charge != m_stateOfCharge) {
    m_stateOfCharge = charge;
    emit stateOfChargeChanged(charge);
  }
}

int EfficiencyController::motorTemp() const { return m_motorTemp; }
void EfficiencyController::setMotorTemp(int temp) {
  if (temp != m_motorTemp) {
    m_motorTemp = temp;
    emit motorTempChanged(temp);
  }
}

int EfficiencyController::packTemp() const { return m_packTemp; }
void EfficiencyController::setPackTemp(int temp) {
  if (temp != m_packTemp) {
    m_packTemp = temp;
    emit packTempChanged(temp);
  }
}

int EfficiencyController::lowVoltageStateOfCharge() const {
  return m_lowVoltageStateOfCharge;
}
void EfficiencyController::setLowVoltageStateOfCharge(int charge) {
  if (charge != m_lowVoltageStateOfCharge) {
    m_lowVoltageStateOfCharge = charge;
    emit lowVoltageStateOfChargeChanged(charge);
  }
}

int EfficiencyController::speed() const { return m_speed; }
void EfficiencyController::setSpeed(int speed) {
  if (speed != m_speed) {
    m_speed = speed;
    emit speedChanged(speed);
  }
}

void EfficiencyController::currentDataDidChange() {
  std::optional<float> torque = m_model->getTorquePower();
  std::optional<float> regen = m_model->getRegenPower();
  std::optional<float> soc = m_model->getStateOfCharge();
  std::optional<float> motorTemp = m_model->getMotorTemp();
  std::optional<float> packTemp = m_model->getPackTemp();
  std::optional<float> lowVoltageSoc = m_model->getLowVoltageStateOfCharge();
  std::optional<float> speed = m_model->getMph();

  if (torque) {
    setCurrentMaxTorque(*torque);
  }
  if (regen) {
    setCurrentRegenStrength(*regen);
  }
  if (soc) {
    setStateOfCharge(*soc);
  }
  if (motorTemp) {
    setMotorTemp(*motorTemp);
  }
  if (packTemp) {
    setPackTemp(*packTemp);
  }
  if (lowVoltageSoc) {
    setLowVoltageStateOfCharge(*lowVoltageSoc);
  }
  if (speed) {
    setSpeed(*speed);
  }
}
