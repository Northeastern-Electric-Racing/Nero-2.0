#ifndef EFFICIENCYCONTROLLER_H
#define EFFICIENCYCONTROLLER_H

#include <QObject>

class EfficiencyController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int currentMaxTorque READ currentMaxTorque WRITE setCurrentMaxTorque NOTIFY currentMaxTorqueChanged FINAL)
  Q_PROPERTY(int currentRegenStrength READ currentRegenStrength WRITE setCurrentRegenStrength NOTIFY currentRegenStrengthChanged FINAL)
  Q_PROPERTY(QVector<qreal> packSegments READ packSegments WRITE setPackSegments NOTIFY packSegmentsChanged FINAL)
  Q_PROPERTY(qreal maxCellTemp READ maxCellTemp WRITE setMaxCellTemp NOTIFY maxCellTempChanged FINAL)
  Q_PROPERTY(int stateOfCharge READ stateOfCharge WRITE setStateOfCharge NOTIFY stateOfChargeChanged FINAL)
  Q_PROPERTY(qreal inverterTemp READ inverterTemp WRITE setInverterTemp NOTIFY inverterTempChanged FINAL)
  Q_PROPERTY(qreal motorTemp READ motorTemp WRITE setMotorTemp NOTIFY motorTempChanged FINAL)
  Q_PROPERTY(qreal averageCellTemp READ averageCellTemp WRITE setAverageCellTemp NOTIFY averageCellTempChanged FINAL)
  Q_PROPERTY(int stateOfChargeDelta READ stateOfChargeDelta WRITE setStateOfChargeDelta NOTIFY stateOfChargeDeltaChanged FINAL)

public:
  explicit EfficiencyController(QObject *parent = nullptr);
  int currentMaxTorque() const;
  int currentRegenStrength() const;
  QVector<qreal> packSegments() const;
  qreal maxCellTemp() const;
  int stateOfCharge() const;
  qreal inverterTemp() const;
  qreal motorTemp() const;
  qreal averageCellTemp() const;
  int stateOfChargeDelta() const;

signals:
  void currentMaxTorqueChanged(int);
  void currentRegenStrengthChanged(int);
  void packSegmentsChanged(QVector<qreal>);
  void maxCellTempChanged(qreal);
  void stateOfChargeChanged(int);
  void inverterTempChanged(qreal);
  void motorTempChanged(qreal);
  void averageCellTempChanged(qreal);
  void stateOfChargeDeltaChanged(int);

public slots:
  void setCurrentMaxTorque(int);
  void setCurrentRegenStrength(int);
  void setPackSegments(QVector<qreal>);
  void setMaxCellTemp(qreal);
  void setStateOfCharge(int);
  void setInverterTemp(qreal);
  void setMotorTemp(qreal);
  void setAverageCellTemp(qreal);
  void setStateOfChargeDelta(int);

private:
  int m_currentMaxTorque; // torque percentage [0,100]
  int m_currentRegenStrength; // regen strength [1,3]
  QVector<qreal> m_packSegments; // pack segments temperatures qreal[4]
  qreal m_maxCellTemp; // Celsius
  int m_stateOfCharge; // charge percentage [0,100]
  qreal m_inverterTemp; // Celsius
  qreal m_motorTemp; // Celsius
  qreal m_averageCellTemp; // Celsius
  int m_stateOfChargeDelta; // charge percentage delta [-100,100]
};

#endif // EFFICIENCYCONTROLLER_H
