#ifndef CHARGINGCONTROLLER_H
#define CHARGINGCONTROLLER_H

#include <QObject>

class ChargingController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int charge READ charge WRITE setCharge NOTIFY chargeChanged FINAL)
  Q_PROPERTY(bool burning READ burning WRITE setBurning NOTIFY burningChanged FINAL)
  Q_PROPERTY(qreal maxCellVoltage READ maxCellVoltage WRITE setMaxCellVoltage NOTIFY
		 maxCellVoltageChanged FINAL)
  Q_PROPERTY(int maxCellVoltageChipNumber READ maxCellVoltageChipNumber WRITE
		 setMaxCellVoltageChipNumber NOTIFY maxCellVoltageChipNumberChanged FINAL)
  Q_PROPERTY(int maxCellVoltageCellNumber READ maxCellVoltageCellNumber WRITE
		 setMaxCellVoltageCellNumber NOTIFY maxCellVoltageCellNumberChanged FINAL)
  Q_PROPERTY(qreal minCellVoltage READ minCellVoltage WRITE setMinCellVoltage NOTIFY
		 minCellVoltageChanged FINAL)
  Q_PROPERTY(int minCellVoltageChipNumber READ minCellVoltageChipNumber WRITE
		 setMinCellVoltageChipNumber NOTIFY minCellVoltageChipNumberChanged FINAL)
  Q_PROPERTY(int minCellVoltageCellNumber READ minCellVoltageCellNumber WRITE
		 setMinCellVoltageCellNumber NOTIFY minCellVoltageCellNumberChanged FINAL)
  Q_PROPERTY(qreal cellDelta READ cellDelta WRITE setCellDelta NOTIFY cellDeltaChanged FINAL)
  Q_PROPERTY(qreal packVoltage READ packVoltage WRITE setPackVoltage NOTIFY packVoltageChanged FINAL)
  Q_PROPERTY(qreal current READ current WRITE setCurrent NOTIFY currentChanged FINAL)
  Q_PROPERTY(qreal packTemp READ packTemp WRITE setPackTemp NOTIFY packTempChanged FINAL)

public:
  explicit ChargingController(QObject *parent = nullptr);
  int charge() const;
  bool burning() const;
  qreal maxCellVoltage() const;
  int maxCellVoltageChipNumber() const;
  int maxCellVoltageCellNumber() const;
  qreal minCellVoltage() const;
  int minCellVoltageChipNumber() const;
  int minCellVoltageCellNumber() const;
  qreal cellDelta() const;
  qreal packVoltage() const;
  qreal current() const;
  qreal packTemp() const;

signals:
  void chargeChanged(int);
  void burningChanged(bool);
  void maxCellVoltageChanged(qreal);
  void maxCellVoltageChipNumberChanged(int);
  void maxCellVoltageCellNumberChanged(int);
  void minCellVoltageChanged(qreal);
  void minCellVoltageChipNumberChanged(int);
  void minCellVoltageCellNumberChanged(int);
  void cellDeltaChanged(qreal);
  void packVoltageChanged(qreal);
  void currentChanged(qreal);
  void packTempChanged(qreal);

public slots:
  void setCharge(int);
  void setBurning(bool);
  void setMaxCellVoltage(qreal);
  void setMaxCellVoltageChipNumber(int);
  void setMaxCellVoltageCellNumber(int);
  void setMinCellVoltage(qreal);
  void setMinCellVoltageChipNumber(int);
  void setMinCellVoltageCellNumber(int);
  void setCellDelta(qreal);
  void setPackVoltage(qreal);
  void setCurrent(qreal);
  void setPackTemp(qreal);

private:
  int m_charge;                   // charge percentage [0-100]
  bool m_burning;                 // burning cells?
  qreal m_maxCellVoltage;           // V
  int m_maxCellVoltageChipNumber; // number
  int m_maxCellVoltageCellNumber; // number
  qreal m_minCellVoltage;           // V
  int m_minCellVoltageChipNumber; // number
  int m_minCellVoltageCellNumber; // number
  qreal m_cellDelta;                // V
  qreal m_packVoltage;              // V
  qreal m_current;                  // A
  qreal m_packTemp;                 // C
};

#endif // CHARGINGCONTROLLER_H
