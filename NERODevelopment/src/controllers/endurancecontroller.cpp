#include "endurancecontroller.h"

EnduranceController::EnduranceController(Model *model, QObject *parent)
    : ButtonController{model, 4, parent}, m_updateTimer(new QTimer(this)),
      m_timerRunning(false) {
  connect(m_model, &Model::onCurrentDataChange, this,
          &EnduranceController::currentDataDidChange);
  connect(m_updateTimer, &QTimer::timeout, this,
          &EnduranceController::updateCurrentTime);
  m_updateTimer->setInterval(1);
}

int EnduranceController::currentMaxTorque() const {
  return m_currentMaxTorque;
}
void EnduranceController::setCurrentMaxTorque(int torque) {
  if (torque != m_currentMaxTorque) {
    m_currentMaxTorque = torque;
    emit currentMaxTorqueChanged(torque);
  }
}

int EnduranceController::currentRegenStrength() const {
  return m_currentRegenStrength;
}
void EnduranceController::setCurrentRegenStrength(int strength) {
  if (strength != m_currentRegenStrength) {
    m_currentRegenStrength = strength;
    emit currentRegenStrengthChanged(strength);
  }
}

int EnduranceController::stateOfCharge() const { return m_stateOfCharge; }
void EnduranceController::setStateOfCharge(int charge) {
  if (charge != m_stateOfCharge) {
    m_stateOfCharge = charge;
    emit stateOfChargeChanged(charge);
  }
}

int EnduranceController::motorTemp() const { return m_motorTemp; }
void EnduranceController::setMotorTemp(int temp) {
  if (temp != m_motorTemp) {
    m_motorTemp = temp;
    emit motorTempChanged(temp);
  }
}

int EnduranceController::packTemp() const { return m_packTemp; }
void EnduranceController::setPackTemp(int temp) {
  if (temp != m_packTemp) {
    m_packTemp = temp;
    emit packTempChanged(temp);
  }
}

int EnduranceController::speed() const { return m_speed; }
void EnduranceController::setSpeed(int speed) {
  if (speed != m_speed) {
    m_speed = speed;
    emit speedChanged(speed);
  }
}

int EnduranceController::powerDrawPercent() const {
  return m_powerDrawPercent;
}
void EnduranceController::setPowerDrawPercent(int percent) {
  if (percent != m_powerDrawPercent) {
    m_powerDrawPercent = percent;
    emit powerDrawPercentChanged(percent);
  }
}

int EnduranceController::maxDCCurrentTarget() const {
  return m_maxDCCurrentTarget;
}
void EnduranceController::setMaxDCCurrentTarget(int target) {
  if (target != m_maxDCCurrentTarget) {
    m_maxDCCurrentTarget = target;
    emit maxDCCurrentTargetChanged(target);
  }
}

void EnduranceController::currentDataDidChange() {
  std::optional<float> torque = m_model->getTorquePower();
  std::optional<float> regen = m_model->getRegenPower();
  std::optional<float> soc = m_model->getStateOfCharge();
  std::optional<float> motorTemp = m_model->getMotorTemp();
  std::optional<float> packTemp = m_model->getPackTemp();
  std::optional<float> speed = m_model->getMph();
  std::optional<float> dcCurrent = m_model->getDCCurrent();
  std::optional<float> maxDCTarget = m_model->getMaxDCCurrentTarget();

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
  if (speed) {
    setSpeed(*speed);
  }
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

int EnduranceController::currentTime() const { return m_currentTime; }
void EnduranceController::setCurrentTime(int currentTime) {
  if (currentTime != m_currentTime) {
    m_currentTime = currentTime;
    emit currentTimeChanged(currentTime);
  }
}
int EnduranceController::fastestTime() const { return m_fastestTime; }
void EnduranceController::setFastestTime(int fastTime) {
  if (fastTime != m_fastestTime) {
    m_fastestTime = fastTime;
    emit fastestTimeChanged(fastTime);
  }
}
int EnduranceController::lastTime() const { return m_lastTime; }
void EnduranceController::setLastTime(int lastTime) {
  if (lastTime != m_lastTime) {
    m_lastTime = lastTime;
    emit lastTimeChanged(lastTime);
  }
}

void EnduranceController::enterButtonPressed() {
  if (m_timerRunning) {
    int runTime = static_cast<int>(m_timer.elapsed());
    m_timerRunning = false;
    m_updateTimer->stop();

    qDebug() << "Timer stopped. Run time:" << runTime
             << " Last time:" << m_lastTime
             << " Fastest time:" << m_fastestTime;

    setLastTime(runTime);
    setCurrentTime(runTime);

    if (runTime < fastestTime() || fastestTime() == 0) {
      setFastestTime(runTime);
      qDebug() << "Fastest time overridden:" << runTime;
    }

    m_timerRunning = true;
    m_timer.start();
    m_updateTimer->start();
    qDebug() << "Timer started for new lap.";
  } else {
    m_timerRunning = true;
    m_timer.start();
    m_updateTimer->start();
    qDebug() << "Timer started.";
  }
}

void EnduranceController::updateCurrentTime() {
  if (m_timerRunning) {
    setCurrentTime(static_cast<int>(m_timer.elapsed()));
  }
}
