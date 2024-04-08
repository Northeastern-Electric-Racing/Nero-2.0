#ifndef EFFICIENCYCONTROLLER_H
#define EFFICIENCYCONTROLLER_H

#include "buttoncontroller.h"
#include <QObject>

/**
 * @brief The EfficiencyController class
 * Controller for the efficiency view
 */
class EfficiencyController : public ButtonController {
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
  Q_PROPERTY(int lowVoltageStateOfCharge READ lowVoltageStateOfCharge WRITE
                 setLowVoltageStateOfCharge NOTIFY
                     lowVoltageStateOfChargeChanged FINAL)
  Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)

public:
  explicit EfficiencyController(Model *model, QObject *parent = nullptr);
  int currentMaxTorque() const;
  int currentRegenStrength() const;
  int stateOfCharge() const;
  int motorTemp() const;
  int packTemp() const;
  int lowVoltageStateOfCharge() const;
  int speed() const;

signals:
  void currentMaxTorqueChanged(int);
  void currentRegenStrengthChanged(int);
  void stateOfChargeChanged(int);
  void motorTempChanged(int);
  void packTempChanged(int);
  void lowVoltageStateOfChargeChanged(int);
  void speedChanged(int);

public slots:
  void setCurrentMaxTorque(int);
  void setCurrentRegenStrength(int);
  void setStateOfCharge(int);
  void setMotorTemp(int);
  void setPackTemp(int);
  void setLowVoltageStateOfCharge(int);
  void setSpeed(int);
  void currentDataDidChange();

private:
  int m_currentMaxTorque;        // torque percentage [0,100]
  int m_currentRegenStrength;    // regen strength [1,3]
  int m_stateOfCharge;           // charge percentage [0,100]
  int m_motorTemp;               // Celsius
  int m_packTemp;                // Celsius
  int m_lowVoltageStateOfCharge; // charge percentage [0,100]
  int m_speed;                   // speed in mph
};

#endif // EFFICIENCYCONTROLLER_H
