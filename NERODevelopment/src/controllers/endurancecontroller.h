#ifndef ENDURANCECONTROLLER_H
#define ENDURANCECONTROLLER_H

#include "buttoncontroller.h"
#include <QElapsedTimer>
#include <QObject>
#include <QTimer>

/**
 * @brief The EnduranceController class
 * Controller for the endurance view
 */
class EnduranceController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(int currentMaxTorque READ currentMaxTorque WRITE
                 setCurrentMaxTorque NOTIFY currentMaxTorqueChanged FINAL)
  Q_PROPERTY(
      int currentRegenStrength READ currentRegenStrength WRITE
          setCurrentRegenStrength NOTIFY currentRegenStrengthChanged FINAL)
  Q_PROPERTY(int stateOfCharge READ stateOfCharge WRITE setStateOfCharge NOTIFY
                 stateOfChargeChanged FINAL)
  Q_PROPERTY(int motorTemp READ motorTemp WRITE setMotorTemp NOTIFY
                 motorTempChanged FINAL)
  Q_PROPERTY(
      int packTemp READ packTemp WRITE setPackTemp NOTIFY packTempChanged FINAL)
  Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
  Q_PROPERTY(int currentTime READ currentTime WRITE setCurrentTime NOTIFY
                 currentTimeChanged)
  Q_PROPERTY(int fastestTime READ fastestTime WRITE setFastestTime NOTIFY
                 fastestTimeChanged)
  Q_PROPERTY(
      int lastTime READ lastTime WRITE setLastTime NOTIFY lastTimeChanged)
  Q_PROPERTY(int powerDrawPercent READ powerDrawPercent WRITE
                 setPowerDrawPercent NOTIFY powerDrawPercentChanged FINAL)
  Q_PROPERTY(int maxDCCurrentTarget READ maxDCCurrentTarget WRITE
                 setMaxDCCurrentTarget NOTIFY maxDCCurrentTargetChanged FINAL)
  Q_PROPERTY(float currentRegenPercentage READ currentRegenPercentage NOTIFY
                 currentRegenPercentageChanged)
  Q_PROPERTY(float maxRegenCapacity READ maxRegenCapacity WRITE
                 setMaxRegenCapacity NOTIFY maxRegenCapacityChanged)

public:
  explicit EnduranceController(Model *model, QObject *parent = nullptr);
  int currentMaxTorque() const;
  int currentRegenStrength() const;
  int stateOfCharge() const;
  int motorTemp() const;
  int packTemp() const;
  int speed() const;
  int currentTime() const;
  int fastestTime() const;
  int lastTime() const;
  int powerDrawPercent() const;
  int maxDCCurrentTarget() const;
  float currentRegenPercentage() const;
  float maxRegenCapacity() const;

signals:
  void currentMaxTorqueChanged(int);
  void currentRegenStrengthChanged(int);
  void stateOfChargeChanged(int);
  void motorTempChanged(int);
  void packTempChanged(int);
  void speedChanged(int);
  void currentTimeChanged(int);
  void fastestTimeChanged(int);
  void lastTimeChanged(int);
  void powerDrawPercentChanged(int);
  void maxDCCurrentTargetChanged(int);
  void currentRegenPercentageChanged(float);
  void maxRegenCapacityChanged(float);
  void toggleFaultAlertsRequested();

public slots:
  void setCurrentMaxTorque(int);
  void setCurrentRegenStrength(int);
  void setStateOfCharge(int);
  void setMotorTemp(int);
  void setPackTemp(int);
  void setSpeed(int);
  void currentDataDidChange();
  void setCurrentTime(int);
  void setFastestTime(int);
  void setLastTime(int);
  void setPowerDrawPercent(int);
  void setMaxDCCurrentTarget(int);
  void setMaxRegenCapacity(float);

  void enterButtonPressed() override;
  void rightButtonPressed() override;
  void updateCurrentTime();

private:
  int m_currentMaxTorque = 0;     // torque percentage [0,100]
  int m_currentRegenStrength = 0; // regen strength [1,3]
  int m_stateOfCharge = 0;        // charge percentage [0,100]
  int m_motorTemp = 0;            // Celsius
  int m_packTemp = 0;             // Celsius
  int m_speed = 0;                // speed in mph
  int m_currentTime = 0;
  int m_fastestTime = 0;
  int m_lastTime = 0;
  int m_powerDrawPercent = 0;
  int m_maxDCCurrentTarget = 0;
  float m_maxRegenCapacity = 0;
  bool m_timerRunning = false;
  QElapsedTimer m_timer;
  QTimer *m_updateTimer;
};

#endif // ENDURANCECONTROLLER_H
