#include "efficiencycontroller.h"

EfficiencyController::EfficiencyController(Model *model, QObject *parent)
    : ButtonController{model, 8, parent}, m_currentMaxTorque(0),
      m_currentRegenStrength(0), m_packSegments({0, 0, 0, 0}), m_maxCellTemp(0),
      m_stateOfCharge(0), m_inverterTemp(0), m_motorTemp(0),
      m_averageCellTemp(0), m_stateOfChargeDelta(0) {}

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

QVector<qreal> EfficiencyController::packSegments() const {
  return m_packSegments;
}
void EfficiencyController::setPackSegments(QVector<qreal> packs) {
  // TODO: chek if this comparison and assignment are wise
  // most likely will always result in true and assignment
  if (m_packSegments != packs) {
    m_packSegments = packs;
    emit packSegmentsChanged(packs);
  }
}

qreal EfficiencyController::maxCellTemp() const { return m_maxCellTemp; }
void EfficiencyController::setMaxCellTemp(qreal temp) {
  if (temp != m_maxCellTemp) {
    m_maxCellTemp = temp;
    emit maxCellTempChanged(temp);
  }
}

int EfficiencyController::stateOfCharge() const { return m_stateOfCharge; }
void EfficiencyController::setStateOfCharge(int charge) {
  if (charge != m_stateOfCharge) {
    m_stateOfCharge = charge;
    emit stateOfChargeChanged(charge);
  }
}

qreal EfficiencyController::inverterTemp() const { return m_inverterTemp; }
void EfficiencyController::setInverterTemp(qreal temp) {
  if (temp != m_inverterTemp) {
    m_inverterTemp = temp;
    emit inverterTempChanged(temp);
  }
}

qreal EfficiencyController::motorTemp() const { return m_motorTemp; }
void EfficiencyController::setMotorTemp(qreal temp) {
  if (temp != m_motorTemp) {
    m_motorTemp = temp;
    emit motorTempChanged(temp);
  }
}

qreal EfficiencyController::averageCellTemp() const {
  return m_averageCellTemp;
}
void EfficiencyController::setAverageCellTemp(qreal temp) {
  if (temp != m_averageCellTemp) {
    m_averageCellTemp = temp;
    emit averageCellTempChanged(temp);
  }
}

int EfficiencyController::stateOfChargeDelta() const {
  return m_stateOfChargeDelta;
}
void EfficiencyController::setStateOfChargeDelta(int delta) {
  if (delta != m_stateOfChargeDelta) {
    m_stateOfChargeDelta = delta;
    emit stateOfChargeDeltaChanged(delta);
  }
}
