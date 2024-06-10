#include "efficiencycontroller.h"

EfficiencyController::EfficiencyController(Model *model, QObject *parent)
    : ButtonController{model, 3, parent}, m_updateTimer(new QTimer(this)),
      m_timerRunning(false) {
  connect(m_model, &Model::onCurrentDataChange, this,
          &EfficiencyController::currentDataDidChange);
  connect(m_updateTimer, &QTimer::timeout, this,
          &EfficiencyController::updateCurrentTime);
  m_updateTimer->setInterval(1);
  this->m_debounceOffset = 750;
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

int EfficiencyController::currentTime() const { return m_currentTime; }
void EfficiencyController::setCurrentTime(int currentTime) {
  if (currentTime != m_currentTime) {
    m_currentTime = currentTime;
    emit currentTimeChanged(currentTime);
  }
}
int EfficiencyController::fastestTime() const { return m_fastestTime; }
void EfficiencyController::setFastestTime(int fastTime) {
  if (fastTime != m_fastestTime) {
    m_fastestTime = fastTime;
    emit fastestTimeChanged(fastTime);
  }
}
int EfficiencyController::lastTime() const { return m_lastTime; }
void EfficiencyController::setLastTime(int lastTime) {
  if (lastTime != m_lastTime) {
    m_lastTime = lastTime;
    emit lastTimeChanged(lastTime);
  }
}

void EfficiencyController::enterButtonPressed() {
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

void EfficiencyController::updateCurrentTime() {
  if (m_timerRunning) {
    setCurrentTime(static_cast<int>(m_timer.elapsed()));
  }
}
