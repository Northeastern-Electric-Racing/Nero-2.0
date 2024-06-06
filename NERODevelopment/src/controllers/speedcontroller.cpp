#include "speedcontroller.h"
#include <QElapsedTimer>
#include <QTimer>

SpeedController::SpeedController(Model *model, QObject *parent)
    : ButtonController{model, 2, parent}, m_updateTimer(new QTimer(this)) {
  connect(m_model, &Model::onCurrentDataChange, this, &SpeedController::update);
  connect(m_updateTimer, &QTimer::timeout, this,
          &SpeedController::updateCurrentTime);
  m_updateTimer->setInterval(1);
}

bool SpeedController::tractionControl() const { return m_tractionControl; }
void SpeedController::setTractionControl(bool tractionStatus) {
  if (tractionStatus != m_tractionControl) {
    m_tractionControl = tractionStatus;
    emit tractionControlChanged(tractionStatus);
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
int SpeedController::currentTime() const { return m_currentTime; }
void SpeedController::setCurrentTime(int currentTime) {
  if (currentTime != m_currentTime) {
    m_currentTime = currentTime;
    emit currentTimeChanged(currentTime);
  }
}
int SpeedController::fastestTime() const { return m_fastestTime; }
void SpeedController::setFastestTime(int fastTime) {
  if (fastTime != m_fastestTime) {
    m_fastestTime = fastTime;
    emit fastestTimeChanged(fastTime);
  }
}
int SpeedController::lastTime() const { return m_lastTime; }
void SpeedController::setLastTime(int lastTime) {
  if (lastTime != m_lastTime) {
    m_lastTime = lastTime;
    emit lastTimeChanged(lastTime);
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

void SpeedController::enterButtonPressed() {
  if (m_timerRunning) {
    m_timerRunning = false;
    m_updateTimer->stop();
    int runTime = static_cast<int>(m_timer.elapsed());
    qDebug() << "Timer stopped. Run time:" << runTime
             << " Last time:" << m_lastTime
             << " Fastest time:" << m_fastestTime;
    setCurrentTime(runTime);

    if (runTime < fastestTime() || fastestTime() == 0) {
      setFastestTime(runTime);
      qDebug() << "fastest time overridden" << runTime;
    }
  } else {
    setLastTime(m_currentTime);
    m_timerRunning = true;
    m_timer.start();
    m_updateTimer->start();
    qDebug() << "Timer started.";
  }
}

void SpeedController::updateCurrentTime() {
  if (m_timerRunning) {
    setCurrentTime(static_cast<int>(m_timer.elapsed()));
  }
}

void SpeedController::update() {
  setTractionControl(*m_model->getTractionControl());
  setPackTemp(*m_model->getPackTemp());
  setMotorTemp(*m_model->getMotorTemp());
  setChargeState(*m_model->getStateOfCharge());
  setCurrentSpeed(*m_model->getMph());
  setMaxSpeed(m_model->getMaxSpeed());
  setCurrent(*m_model->getCurrent());
}
