#include "speedcontroller.h"

SpeedController::SpeedController(Model *model, QObject *parent)
    : ButtonController{model, 3, parent} {
  connect(m_model, &Model::onCurrentDataChange, this, &SpeedController::update);
}

bool SpeedController::tractionControl() const { return m_tractionControl; }
void SpeedController::setTractionControl(bool tractionStatus) {
  if (tractionStatus != m_tractionControl) {
    m_tractionControl = tractionStatus;
    emit tractionControlChanged(tractionStatus);
  }
}

float SpeedController::regen() const { return m_regen; }
void SpeedController::setRegen(float regen) {
  if (regen != m_regen) {
    m_regen = regen;
    emit regenChanged(regen);
  }
}

float SpeedController::packTemp() const { return m_packTemp; }
void SpeedController::setPackTemp(float packTemp) {
  if (packTemp != m_packTemp) {
    m_packTemp = packTemp;
    emit packTempChanged(packTemp);
  }
}
float SpeedController::motorTemp() const { return m_motorTemp; }
void SpeedController::setMotorTemp(float motorTemp) {
  if (motorTemp != m_motorTemp) {
    m_motorTemp = motorTemp;
    emit motorTempChanged(motorTemp);
  }
}
float SpeedController::chargeState() const { return m_chargeState; }
void SpeedController::setChargeState(float chargeState) {
  if (chargeState != m_chargeState) {
    m_chargeState = chargeState;
    emit chargeStateChanged(chargeState);
  }
}
int SpeedController::currentSpeed() const { return m_currentSpeed; }
void SpeedController::setCurrentSpeed(int currentSpeed) {
  if (currentSpeed != m_currentSpeed) {
    m_currentSpeed = currentSpeed;
    emit currentSpeedChanged(currentSpeed);
  }
}
int SpeedController::maxSpeed() const { return m_maxSpeed; }
void SpeedController::setMaxSpeed(int maxSpeed) {
  if (maxSpeed != m_maxSpeed) {
    m_maxSpeed = maxSpeed;
    emit maxSpeedChanged(maxSpeed);
  }
}
float SpeedController::current() const { return m_current; }
void SpeedController::setCurrent(float current) {
  if (current != m_current) {
    m_current = current;
    emit currentChanged(current);
  }
}
float SpeedController::maxCurrent() const { return m_maxCurrent; }
void SpeedController::setMaxCurrent(float maxCurrent) {
  if (maxCurrent != m_maxCurrent) {
    m_maxCurrent = maxCurrent;
    emit maxCurrentChanged(maxCurrent);
  }
}
float SpeedController::currentDischarge() const { return m_currentDischarge; }
void SpeedController::setCurrentDischarge(float currentDischarge) {
  if (currentDischarge != m_currentDischarge) {
    m_currentDischarge = currentDischarge;
    emit currentDischargeChanged(currentDischarge);
  }
}
float SpeedController::maxCurrentDischarge() const {
  return m_maxCurrentDischarge;
}
void SpeedController::setMaxCurrentDischarge(float maxCurrentDischarge) {
  if (maxCurrentDischarge != m_maxCurrentDischarge) {
    m_maxCurrentDischarge = maxCurrentDischarge;
    emit maxCurrentDischargeChanged(maxCurrentDischarge);
  }
}

int SpeedController::powerDrawPercent() const { return m_powerDrawPercent; }
void SpeedController::setPowerDrawPercent(int percent) {
  if (percent != m_powerDrawPercent) {
    m_powerDrawPercent = percent;
    emit powerDrawPercentChanged(percent);
  }
}

int SpeedController::maxDCCurrentTarget() const { return m_maxDCCurrentTarget; }
void SpeedController::setMaxDCCurrentTarget(int target) {
  if (target != m_maxDCCurrentTarget) {
    m_maxDCCurrentTarget = target;
    emit maxDCCurrentTargetChanged(target);
  }
}

void SpeedController::rightButtonPressed() {
  emit toggleFaultAlertsRequested();
}

void SpeedController::update() {
  setTractionControl(*m_model->getTractionControl());
  setPackTemp(*m_model->getPackTemp());
  setMotorTemp(*m_model->getMotorTemp());
  setChargeState(*m_model->getStateOfCharge());
  setCurrentSpeed(*m_model->getMph());
  setMaxSpeed(m_model->getMaxSpeed());
  setCurrent(*m_model->getCurrent());
  setMaxCurrent(m_model->getMaxDraw());
  setCurrentDischarge(*m_model->getDcl());
  setRegen(*m_model->getRegenPower());

  std::optional<float> dcCurrent = m_model->getDCCurrent();
  std::optional<float> maxDCTarget = m_model->getMaxDCCurrentTarget();

  if (maxDCTarget) {
    setMaxDCCurrentTarget(static_cast<int>(std::round(std::abs(*maxDCTarget))));
  }
  if (dcCurrent && maxDCTarget && std::abs(*maxDCTarget) > 0) {
    int percent = static_cast<int>(
        std::round(std::abs(*dcCurrent) / std::abs(*maxDCTarget) * 100.0f));
    if (percent > 100)
      percent = 100;
    if (percent < 0)
      percent = 0;
    setPowerDrawPercent(percent);
  }
}
