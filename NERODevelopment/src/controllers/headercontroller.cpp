#include "headercontroller.h"

HeaderController::HeaderController(Model *model, QObject *parent)
    : QObject{parent}, m_numCriticalWarnings(0), m_numNonCriticalWarnings(0),
      m_isTalking(false) {
  this->m_model = model;
  connect(m_model, &Model::onCurrentDataChange, this,
          &HeaderController::currentDataDidChange);
}

int HeaderController::numCriticalWarnings() const {
  return m_numCriticalWarnings;
}

void HeaderController::setNumCriticalWarnings(int numCriticalWarnings) {
  if (m_numCriticalWarnings != numCriticalWarnings) {
    m_numCriticalWarnings = numCriticalWarnings;
    emit numCriticalWarningsChanged(numCriticalWarnings);
  }
}

int HeaderController::numNonCriticalWarnings() const {
  return m_numNonCriticalWarnings;
}

void HeaderController::setNumNonCriticalWarnings(int numNonCriticalWarnings) {
  if (m_numNonCriticalWarnings != numNonCriticalWarnings) {
    m_numNonCriticalWarnings = numNonCriticalWarnings;
    emit numCriticalWarningsChanged(numNonCriticalWarnings);
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
  setNumCriticalWarnings(*m_model->getNumberOfCriticalFaults());
  setNumNonCriticalWarnings(*m_model->getNumberOfNonCriticalFaults());
  setIsTalking(*m_model->getIsTalking());
}
