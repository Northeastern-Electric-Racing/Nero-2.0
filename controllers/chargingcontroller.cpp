#include "chargingcontroller.h"

ChargingController::ChargingController(QObject *parent)
    : QObject{parent}
    , m_charge(0)
    , m_burning(false)
    , m_maxCellVoltage(0)
    , m_maxCellVoltageChipNumber(0)
    , m_maxCellVoltageCellNumber(0)
    , m_minCellVoltage(0)
    , m_minCellVoltageChipNumber(0)
    , m_minCellVoltageCellNumber(0)
    , m_cellDelta(0)
    , m_packVoltage(0)
    , m_current(0)
    , m_packTemp(0)
{}

int ChargingController::charge() const
{
  return m_charge;
}
void ChargingController::setCharge(int charge)
{
  if (charge != m_charge) {
    m_charge = charge;
    emit chargeChanged(charge);
  }
}

bool ChargingController::burning() const
{
  return m_burning;
}
void ChargingController::setBurning(bool burning)
{
  if (burning != m_burning) {
    m_burning = burning;
    emit burningChanged(burning);
  }
}

qreal ChargingController::maxCellVoltage() const
{
  return m_maxCellVoltage;
}
void ChargingController::setMaxCellVoltage(qreal volts)
{
  if (volts != m_maxCellVoltage) {
    m_maxCellVoltage = volts;
    emit maxCellVoltageChanged(volts);
  }
}

int ChargingController::maxCellVoltageChipNumber() const
{
  return m_maxCellVoltageChipNumber;
}
void ChargingController::setMaxCellVoltageChipNumber(int num)
{
  if (num != m_maxCellVoltageChipNumber) {
    m_maxCellVoltageChipNumber = num;
    emit maxCellVoltageChipNumberChanged(num);
  }
}

int ChargingController::maxCellVoltageCellNumber() const
{
  return m_maxCellVoltageCellNumber;
}
void ChargingController::setMaxCellVoltageCellNumber(int num)
{
  if (num != m_maxCellVoltageCellNumber) {
    m_maxCellVoltageCellNumber = num;
    emit maxCellVoltageCellNumberChanged(num);
  }
}

qreal ChargingController::minCellVoltage() const
{
  return m_minCellVoltage;
}
void ChargingController::setMinCellVoltage(qreal volts)
{
  if (volts != m_minCellVoltage) {
    m_minCellVoltage = volts;
    emit minCellVoltageChanged(volts);
  }
}

int ChargingController::minCellVoltageChipNumber() const
{
  return m_minCellVoltageChipNumber;
}
void ChargingController::setMinCellVoltageChipNumber(int num)
{
  if (num != m_minCellVoltageChipNumber) {
    m_minCellVoltageChipNumber = num;
    emit minCellVoltageChipNumberChanged(num);
  }
}

int ChargingController::minCellVoltageCellNumber() const
{
  return m_minCellVoltageCellNumber;
}
void ChargingController::setMinCellVoltageCellNumber(int num)
{
  if (num != m_minCellVoltageCellNumber) {
    m_minCellVoltageCellNumber = num;
    emit minCellVoltageCellNumberChanged(num);
  }
}

qreal ChargingController::cellDelta() const
{
  return m_cellDelta;
}
void ChargingController::setCellDelta(qreal volts)
{
  if (volts != m_cellDelta) {
    m_cellDelta = volts;
    emit cellDeltaChanged(volts);
  }
}

qreal ChargingController::packVoltage() const
{
  return m_packVoltage;
}
void ChargingController::setPackVoltage(qreal volts)
{
  if (volts != m_packVoltage) {
    m_packVoltage = volts;
    emit packVoltageChanged(volts);
  }
}

qreal ChargingController::current() const
{
  return m_current;
}
void ChargingController::setCurrent(qreal amps)
{
  if (amps != m_current) {
    m_current = amps;
    emit currentChanged(amps);
  }
}

qreal ChargingController::packTemp() const
{
  return m_packTemp;
}
void ChargingController::setPackTemp(qreal degrees)
{
  if (degrees != m_packTemp) {
    m_packTemp = degrees;
    emit packTempChanged(degrees);
  }
}
