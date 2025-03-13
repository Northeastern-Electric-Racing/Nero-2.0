#include "headercontroller.h"

HeaderController::HeaderController(Model *model, QObject *parent)
    : QObject{parent}, m_isTalking(false) {
  this->m_model = model;
  connect(m_model, &Model::onCurrentDataChange, this,
          &HeaderController::currentDataDidChange);
}

QList<QString> HeaderController::criticalFaults() const {
  return m_criticalFaults;
}
void HeaderController::setCriticalFaults(QList<QString> criticalFaults) {
  if (m_criticalFaults.length() != criticalFaults.length()) {
    m_criticalFaults = criticalFaults;
    emit criticalFaultsChanged(criticalFaults);
  }
}

QList<QString> HeaderController::nonCriticalFaults() const {
  return m_nonCriticalFaults;
}
void HeaderController::setNonCriticalFaults(QList<QString> nonCriticalFaults) {
  if (m_nonCriticalFaults.length() != nonCriticalFaults.length()) {
    m_nonCriticalFaults = nonCriticalFaults;
    emit nonCriticalFaultsChanged(nonCriticalFaults);
  }
}

bool HeaderController::isTalking() const { return m_isTalking; }

void HeaderController::setIsTalking(bool isTalking) {
  if (m_isTalking != isTalking) {
    m_isTalking = isTalking;
    emit isTalkingChanged(isTalking);
  }
}

void HeaderController::currentDataDidChange() {
  setCriticalFaults(m_model->getCriticalFaults());
  setNonCriticalFaults(m_model->getNonCriticalFaults());
  setIsTalking(*m_model->getIsTalking());
}
