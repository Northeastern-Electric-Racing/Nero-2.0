#ifndef CHARGINGCONTROLLER_H
#define CHARGINGCONTROLLER_H

#include <QObject>

class ChargingController : public QObject {
  Q_OBJECT
  Q_PROPERTY(int charge READ charge WRITE setCharge NOTIFY chargeChanged FINAL)
  Q_PROPERTY(
      bool burning READ burning WRITE setBurning NOTIFY burningChanged FINAL)
  Q_PROPERTY(int maxCellVoltage READ maxCellVoltage WRITE setMaxCellVoltage
                 NOTIFY maxCellVoltageChanged FINAL)
  Q_PROPERTY(int maxCellVoltageChipNumber READ maxCellVoltageChipNumber WRITE
                 setMaxCellVoltageChipNumber NOTIFY
                     maxCellVoltageChipNumberChanged FINAL)
  Q_PROPERTY(int maxCellVoltageCellNumber READ maxCellVoltageCellNumber WRITE
                 setMaxCellVoltageCellNumber NOTIFY
                     maxCellVoltageCellNumberChanged FINAL)
  Q_PROPERTY(int minCellVoltage READ minCellVoltage WRITE setMinCellVoltage
                 NOTIFY minCellVoltageChanged FINAL)
  Q_PROPERTY(int minCellVoltageChipNumber READ minCellVoltageChipNumber WRITE
                 setMinCellVoltageChipNumber NOTIFY
                     minCellVoltageChipNumberChanged FINAL)
  Q_PROPERTY(int minCellVoltageCellNumber READ minCellVoltageCellNumber WRITE
                 setMinCellVoltageCellNumber NOTIFY
                     minCellVoltageCellNumberChanged FINAL)
  Q_PROPERTY(int cellDelta READ cellDelta WRITE setCellDelta NOTIFY
                 cellDeltaChanged FINAL)
  Q_PROPERTY(int packVoltage READ packVoltage WRITE setPackVoltage NOTIFY
                 packVoltageChanged FINAL)
  Q_PROPERTY(
      int current READ current WRITE setCurrent NOTIFY currentChanged FINAL)
  Q_PROPERTY(
      int packTemp READ packTemp WRITE setPackTemp NOTIFY packTempChanged FINAL)

public:
  explicit ChargingController(QObject *parent = nullptr);
  int charge() const;
  bool burning() const;
  int maxCellVoltage() const;
  int maxCellVoltageChipNumber() const;
  int maxCellVoltageCellNumber() const;
  int minCellVoltage() const;
  int minCellVoltageChipNumber() const;
  int minCellVoltageCellNumber() const;
  int cellDelta() const;
  int packVoltage() const;
  int current() const;
  int packTemp() const;

signals:
  void chargeChanged(int);
  void burningChanged(bool);
  void maxCellVoltageChanged(int);
  void maxCellVoltageChipNumberChanged(int);
  void maxCellVoltageCellNumberChanged(int);
  void minCellVoltageChanged(int);
  void minCellVoltageChipNumberChanged(int);
  void minCellVoltageCellNumberChanged(int);
  void cellDeltaChanged(int);
  void packVoltageChanged(int);
  void currentChanged(int);
  void packTempChanged(int);

public slots:
  void setCharge(int);
  void setBurning(bool);
  void setMaxCellVoltage(int);
  void setMaxCellVoltageChipNumber(int);
  void setMaxCellVoltageCellNumber(int);
  void setMinCellVoltage(int);
  void setMinCellVoltageChipNumber(int);
  void setMinCellVoltageCellNumber(int);
  void setCellDelta(int);
  void setPackVoltage(int);
  void setCurrent(int);
  void setPackTemp(int);

private:
  int m_charge;                   // charge percentage [0-100]
  bool m_burning;                 // burning cells?
  int m_maxCellVoltage;           // V
  int m_maxCellVoltageChipNumber; // number
  int m_maxCellVoltageCellNumber; // number
  int m_minCellVoltage;           // V
  int m_minCellVoltageChipNumber; // number
  int m_minCellVoltageCellNumber; // number
  int m_cellDelta;                // V
  int m_packVoltage;              // V
  int m_current;                  // A
  int m_packTemp;                 // C
};

#endif // CHARGINGCONTROLLER_H
