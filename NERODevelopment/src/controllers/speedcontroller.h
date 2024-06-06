#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#include "../models/model.h"
#include "buttoncontroller.h"
#include <QElapsedTimer>
#include <QObject>
#include <QTimer>

class SpeedController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(bool tractionControl READ tractionControl WRITE setTractionControl
                 NOTIFY tractionControlChanged)
  Q_PROPERTY(
      float packTemp READ packTemp WRITE setPackTemp NOTIFY packTempChanged)
  Q_PROPERTY(
      float motorTemp READ motorTemp WRITE setMotorTemp NOTIFY motorTempChanged)
  Q_PROPERTY(float chargeState READ chargeState WRITE setChargeState NOTIFY
                 chargeStateChanged)
  Q_PROPERTY(int currentTime READ currentTime WRITE setCurrentTime NOTIFY
                 currentTimeChanged)
  Q_PROPERTY(int fastestTime READ fastestTime WRITE setFastestTime NOTIFY
                 fastestTimeChanged)
  Q_PROPERTY(
      int lastTime READ lastTime WRITE setLastTime NOTIFY lastTimeChanged)
  Q_PROPERTY(int currentSpeed READ currentSpeed WRITE setCurrentSpeed NOTIFY
                 currentSpeedChanged)
  Q_PROPERTY(
      int maxSpeed READ maxSpeed WRITE setMaxSpeed NOTIFY maxSpeedChanged)
  Q_PROPERTY(float current READ current WRITE setCurrent NOTIFY currentChanged)
  Q_PROPERTY(float maxCurrent READ maxCurrent WRITE setMaxCurrent NOTIFY
                 maxCurrentChanged)
  Q_PROPERTY(float currentDischarge READ currentDischarge WRITE
                 setCurrentDischarge NOTIFY currentDischargeChanged)
  Q_PROPERTY(float maxCurrentDischarge READ maxCurrentDischarge WRITE
                 setMaxCurrentDischarge NOTIFY maxCurrentDischargeChanged)

public:
  explicit SpeedController(Model *model, QObject *parent = nullptr);

  bool tractionControl() const;
  float packTemp() const;
  float motorTemp() const;
  float chargeState() const;
  int currentTime() const;
  int fastestTime() const;
  int lastTime() const;
  int currentSpeed() const;
  int maxSpeed() const;
  float current() const;
  float maxCurrent() const;
  float currentDischarge() const;
  float maxCurrentDischarge() const;

signals:
  void tractionControlChanged(bool);
  void packTempChanged(float);
  void motorTempChanged(float);
  void chargeStateChanged(float);
  void currentTimeChanged(int);
  void fastestTimeChanged(int);
  void lastTimeChanged(int);
  void currentSpeedChanged(int);
  void maxSpeedChanged(int);
  void currentChanged(float);
  void maxCurrentChanged(float);
  void currentDischargeChanged(float);
  void maxCurrentDischargeChanged(float);

public slots:
  void setTractionControl(bool);
  void setPackTemp(float);
  void setMotorTemp(float);
  void setChargeState(float);
  void setCurrentTime(int);
  void setFastestTime(int);
  void setLastTime(int);
  void setCurrentSpeed(int);
  void setMaxSpeed(int);
  void setCurrent(float);
  void setMaxCurrent(float);
  void setCurrentDischarge(float);
  void setMaxCurrentDischarge(float);

  void enterButtonPressed() override;
  void updateCurrentTime();

  void update();

private:
  bool m_tractionControl = false;
  float m_packTemp = 0;
  float m_motorTemp = 0;
  float m_chargeState = 0;
  int m_currentTime = 0;
  int m_fastestTime = 0;
  int m_lastTime = 0;
  int m_currentSpeed = 0;
  int m_maxSpeed = 0;
  float m_current = 0;
  float m_maxCurrent = 0;
  float m_currentDischarge = 0;
  float m_maxCurrentDischarge = 0;

  bool m_timerRunning = false;
  QElapsedTimer m_timer;
  QTimer *m_updateTimer;
};

#endif // SPEEDCONTROLLER_H
