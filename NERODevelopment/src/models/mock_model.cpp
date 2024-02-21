#include "mock_model.h"
#include "../utils/data_type_names.h"
#include <QThread>
#include <math.h>

MockModel::MockModel()
    : mph(3), status(true), dir(true), packTemp(30), motorTemp(40),
      stateOfCharge(55), lvBattery(88), current(7.6), isBurning(0), isDebug(0),
      bmsFaults(0), mpuFaults(0), maxCellVoltage(3.5),
      maxCellVoltageChipNumber(1), maxCellVoltageCellNumber(10),
      maxCellTemp(30), maxCellTempChipNumber(5), maxCellTempCellNumber(8),
      minCellVoltage(3.2), minCellVoltageChipNumber(2),
      minCellVoltageCellNumber(3), minCellTemp(25), minCellTempChipNumber(3),
      minCellTempCellNumber(4), averageCellVoltage(3.3), averageCellTemp(27),
      packVoltage(3.4), bmsState(0), packCurrent(4), dcl(280), ccl(300),
      gforceX(0.5), gforceY(-1), gforceZ(0.5), segment1Temp(30),
      segment2Temp(50), segment3Temp(35), segment4Temp(15), motorPower(100),
      fanPower(100), torquePower(100), regenPower(1), modeIndex(0),
      stateOfChargeDelta(-1), inverterTemp(30), forward(0), backward(0),
      enter(0), up(0), down(0), right(0) {
  this->updateTimer = new QTimer(this);
  connect(updateTimer, &QTimer::timeout, this, &MockModel::connectToMQTT);
  this->updateTimer->start(10);
}

MockModel::~MockModel() {}

void MockModel::connectToMQTT() {
  int rng = rand() % 10001;

  if (rng < 5 && rng >= 0)
    mph += 1;
  else if (rng >= 5 && rng <= 10)
    mph -= 1;

  if (rng < 3)
    motorTemp += 1;
  else if (rng >= 985 && rng < 988)
    motorTemp -= 1;

  if (rng < 2)
    packTemp += 1;
  else if (rng >= 990 && rng < 992)
    packTemp -= 1;

  if (rng < 5)
    stateOfCharge += 1;
  else if (rng >= 100 && rng < 105)
    stateOfCharge -= 1;

  if (rng == 100)
    status = false;
  else if (rng < 5)
    status = true;

  if (rng == 200)
    dir = false;
  if (rng > 40 && rng < 45)
    dir = true;

  if (rng == 400)
    isBurning = 0;
  if (rng > 50 && rng < 55)
    isBurning = 1;

  if (rng > 70 && rng < 75)
    lvBattery += 1;
  if (rng > 60 && rng < 65)
    lvBattery -= 1;

  if (rng > 80 && rng < 85)
    current += 0.2;
  if (rng > 90 && rng < 95)
    current -= 0.3;

  if (rng > 100 && rng < 105)
    bmsFaults = rand() % 65536;
  if (rng > 110 && rng < 115)
    bmsFaults = 0;

  if (rng > 120 && rng < 125)
    mpuFaults = rand() % 65536;
  if (rng > 130 && rng < 135)
    mpuFaults = 0;

  if (rng > 140 && rng < 145)
    maxCellVoltage += 0.4;
  if (rng > 150 && rng < 155)
    maxCellVoltage -= 0.4;

  if (rng > 160 && rng < 165)
    maxCellVoltageChipNumber = rand() % 9; // 8 chips

  if (rng > 170 && rng < 175)
    maxCellVoltageCellNumber = rand() % 23; // 22 thermistors

  if (rng > 180 && rng < 185)
    maxCellTemp += 1;
  if (rng > 190 && rng < 195)
    maxCellTemp -= 1;

  if (rng > 200 && rng < 205)
    maxCellTempChipNumber = rand() % 9;

  if (rng > 210 && rng < 215)
    maxCellTempCellNumber = rand() % 23;

  if (rng > 220 && rng < 225)
    minCellVoltage += 0.4;
  if (rng > 230 && rng < 235)
    minCellVoltage -= 0.4;

  if (rng > 240 && rng < 245)
    minCellVoltageChipNumber = rand() % 9;

  if (rng > 250 && rng < 255)
    minCellVoltageCellNumber = rand() % 23;

  if (rng > 260 && rng < 265)
    minCellTemp += 1;
  if (rng > 270 && rng < 275)
    minCellTemp -= 1;

  if (rng > 280 && rng < 285)
    minCellTempChipNumber = rand() % 9;

  if (rng > 290 && rng < 295)
    minCellTempCellNumber = rand() % 23;

  if (rng > 300 && rng < 305)
    averageCellVoltage += 0.4;
  if (rng > 310 && rng < 315)
    averageCellVoltage -= 0.4;

  if (rng > 320 && rng < 325)
    averageCellTemp += 1;
  if (rng > 330 && rng < 335)
    averageCellTemp -= 1;

  if (rng > 340 && rng < 345)
    packVoltage += 0.4;
  if (rng > 350 && rng < 355)
    packVoltage -= 0.4;

  if (rng > 360 && rng < 365)
    packCurrent += 0.2;
  if (rng > 370 && rng < 375)
    packCurrent -= 0.2;

  if (rng > 380 && rng < 385)
    dcl += 10;
  if (rng > 390 && rng < 395)
    dcl -= 10;

  if (rng > 400 && rng < 405)
    ccl += 10;
  if (rng > 410 && rng < 415)
    ccl -= 10;

  if (rng > 420 && rng < 425)
    gforceX += 0.1;
  if (rng > 430 && rng < 435)
    gforceX -= 0.1;

  if (rng > 440 && rng < 445)
    gforceY += 0.1;
  if (rng > 450 && rng < 455)
    gforceY -= 0.1;

  if (rng > 460 && rng < 465)
    gforceZ += 0.1;
  if (rng > 470 && rng < 475)
    gforceZ -= 0.1;

  if (rng > 490 && rng < 495)
    segment1Temp += 1;
  if (rng > 500 && rng < 505)
    segment1Temp -= 1;

  if (rng > 510 && rng < 515)
    segment2Temp += 1;
  if (rng > 520 && rng < 525)
    segment2Temp -= 1;

  if (rng > 530 && rng < 535)
    segment3Temp += 1;
  if (rng > 540 && rng < 545)
    segment3Temp -= 1;

  if (rng > 550 && rng < 555)
    segment4Temp += 1;
  if (rng > 560 && rng < 565)
    segment4Temp -= 1;

  if (rng > 570 && rng < 575)
    stateOfChargeDelta += 1;
  if (rng > 580 && rng < 585)
    stateOfChargeDelta -= 1;

  if (rng > 590 && rng < 595)
    inverterTemp += 1;
  if (rng > 600 && rng < 605)
    inverterTemp -= 1;

  this->updateCurrentData();
}

std::optional<float> MockModel::getMph() { return mph; }

std::optional<float> MockModel::getKph() { return round(mph * 1.609); }

std::optional<float> MockModel::getStatus() { return status; }

std::optional<float> MockModel::getDir() { return dir; }

std::optional<float> MockModel::getPackTemp() { return packTemp; }

std::optional<float> MockModel::getMotorTemp() { return motorTemp; }

std::optional<float> MockModel::getStateOfCharge() { return stateOfCharge; }

std::optional<float> MockModel::getCurrent() {
  return round(current * 10) / 10;
}

std::optional<float> MockModel::getBmsFault() { return bmsFaults; }

std::optional<float> MockModel::getMpuFault() { return mpuFaults; }

std::optional<float> MockModel::getModeIndex() { return modeIndex; }

std::optional<float> MockModel::getMaxCellVoltage() {
  return round(maxCellVoltage * 10) / 10;
}

std::optional<float> MockModel::getMaxCellVoltageChipNumber() {
  return maxCellVoltageChipNumber;
}

std::optional<float> MockModel::getMaxCellVoltageCellNumber() {
  return maxCellVoltageCellNumber;
}

std::optional<float> MockModel::getMaxCellTemp() { return maxCellTemp; }

std::optional<float> MockModel::getMaxCellTempChipNumber() {
  return maxCellTempChipNumber;
}

std::optional<float> MockModel::getMaxCellTempCellNumber() {
  return maxCellTempCellNumber;
}

std::optional<float> MockModel::getMinCellVoltage() {
  return round(minCellVoltage * 10) / 10;
}

std::optional<float> MockModel::getMinCellVoltageChipNumber() {
  return minCellVoltageChipNumber;
}

std::optional<float> MockModel::getMinCellVoltageCellNumber() {
  return minCellVoltageCellNumber;
}

std::optional<float> MockModel::getMinCellTemp() { return minCellTemp; }

std::optional<float> MockModel::getMinCellTempChipNumber() {
  return minCellTempChipNumber;
}

std::optional<float> MockModel::getMinCellTempCellNumber() {
  return minCellTempCellNumber;
}

std::optional<float> MockModel::getAveCellTemp() { return averageCellTemp; }

std::optional<float> MockModel::getAveCellVoltage() {
  return round(averageCellVoltage * 10) / 10;
}

std::optional<float> MockModel::getPackVoltage() {
  return round(packVoltage * 10) / 10;
}

std::optional<float> MockModel::getBmsState() { return bmsState; }

std::optional<float> MockModel::getPackCurrent() { return packCurrent; }

std::optional<float> MockModel::getDcl() { return dcl; }

std::optional<float> MockModel::getCcl() { return ccl; }

std::optional<float> MockModel::getGForceX() {
  return round(gforceX * 10) / 10;
}

std::optional<float> MockModel::getGForceY() {
  return round(gforceY * 10) / 10;
}

std::optional<float> MockModel::getGForceZ() {
  return round(gforceZ * 10) / 10;
}

std::optional<float> MockModel::getSegment1Temp() { return segment1Temp; }

std::optional<float> MockModel::getSegment2Temp() { return segment2Temp; }

std::optional<float> MockModel::getSegment3Temp() { return segment3Temp; }

std::optional<float> MockModel::getSegment4Temp() { return segment4Temp; }

std::optional<float> MockModel::getMotorPower() { return motorPower; }

std::optional<float> MockModel::getFanPower() { return fanPower; }

std::optional<float> MockModel::getTorquePower() { return torquePower; }

std::optional<float> MockModel::getRegenPower() { return regenPower; }

std::optional<float> MockModel::getBurningCells() { return isBurning; }

std::optional<float> MockModel::getInverterTemp() { return inverterTemp; }

std::optional<float> MockModel::getTractionControl() {
  return std::nullopt; // TODO: Implement Mock Traction Control
}

std::optional<float> MockModel::getCellDelta() {
  return std::nullopt; // TODO: Implement Mock Cell Delta
}

std::optional<float> MockModel::getBalancingCells() {
  return std::nullopt; // TODO: Implement Mock balancing Cells
}

std::optional<bool> MockModel::getForwardButtonPressed() { return 0; }

std::optional<bool> MockModel::getEnterButtonPressed() { return 0; }

std::optional<bool> MockModel::getUpButtonPressed() { return 0; }

std::optional<bool> MockModel::getDownButtonPressed() { return 0; }

std::optional<bool> MockModel::getBackwardButtonPressed() { return 0; }

std::optional<bool> MockModel::getRightButtonPressed() { return 0; }

std::optional<bool> MockModel::getIsTalking() { return 0; }

std::optional<int> MockModel::getNumberOfCriticalFaults() { return 0; }

std::optional<int> MockModel::getNumberOfNonCriticalFaults() { return 0; }

QVector<QString> MockModel::convertNumberToDataInfoValue(float value) {
  return {QString::number(value)};
}
void MockModel::updateCurrentData() {
  this->currentData.insert(
      MPH, DataInfo(MPH, "mph", convertNumberToDataInfoValue(mph)));
  this->currentData.insert(
      STATUS, DataInfo(STATUS, "", convertNumberToDataInfoValue(status)));
  this->currentData.insert(
      DIRECTION, DataInfo(DIRECTION, "", convertNumberToDataInfoValue(dir)));
  this->currentData.insert(
      PACKTEMP,
      DataInfo(PACKTEMP, "C", convertNumberToDataInfoValue(packTemp)));
  this->currentData.insert(
      MOTORTEMP,
      DataInfo(MOTORTEMP, "", convertNumberToDataInfoValue(motorTemp)));
  this->currentData.insert(
      STATEOFCHARGE, DataInfo(STATEOFCHARGE, "%",
                              convertNumberToDataInfoValue(stateOfCharge)));
  this->currentData.insert(
      CURRENT, DataInfo(CURRENT, "A", convertNumberToDataInfoValue(current)));
  this->currentData.insert(
      BMSFAULT,
      DataInfo(BMSFAULT, "", convertNumberToDataInfoValue(bmsFaults)));

  this->currentData.insert(
      MPUFAULT,
      DataInfo(MPUFAULT, "", convertNumberToDataInfoValue(mpuFaults)));
  this->currentData.insert(
      MODEINDEX,
      DataInfo(MODEINDEX, "", convertNumberToDataInfoValue(modeIndex)));
  this->currentData.insert(
      MAXCELLVOLTAGE, DataInfo(MAXCELLVOLTAGE, "",
                               convertNumberToDataInfoValue(maxCellVoltage)));
  this->currentData.insert(
      MAXCELLVOLTAGECHIP,
      DataInfo(MAXCELLVOLTAGECHIP, "",
               convertNumberToDataInfoValue(maxCellVoltageCellNumber)));
  this->currentData.insert(
      MAXCELLVOLTAGECELL,
      DataInfo(MAXCELLVOLTAGECELL, "",
               convertNumberToDataInfoValue(maxCellVoltageChipNumber)));
  this->currentData.insert(
      MAXCELLTEMP,
      DataInfo(MAXCELLTEMP, "", convertNumberToDataInfoValue(maxCellTemp)));
  this->currentData.insert(
      MAXCELLTEMPCHIP,
      DataInfo(MAXCELLTEMPCHIP, "",
               convertNumberToDataInfoValue(maxCellTempChipNumber)));
  this->currentData.insert(
      MAXCELLTEMPCELL,
      DataInfo(MAXCELLTEMPCELL, "",
               convertNumberToDataInfoValue(maxCellTempCellNumber)));
  this->currentData.insert(
      MINCELLVOLTAGE, DataInfo(MINCELLVOLTAGE, "",
                               convertNumberToDataInfoValue(minCellVoltage)));
  this->currentData.insert(
      MINCELLVOLTAGECHIP,
      DataInfo(MINCELLVOLTAGECHIP, "",
               convertNumberToDataInfoValue(minCellVoltageChipNumber)));
  this->currentData.insert(
      MINCELLVOLTAGECELL,
      DataInfo(MINCELLVOLTAGECELL, "",
               convertNumberToDataInfoValue(minCellVoltageCellNumber)));
  this->currentData.insert(
      MINCELLTEMP,
      DataInfo(MINCELLTEMP, "", convertNumberToDataInfoValue(minCellTemp)));
  this->currentData.insert(
      MINCELLTEMPCHIP,
      DataInfo(MINCELLTEMPCHIP, "",
               convertNumberToDataInfoValue(minCellTempChipNumber)));
  this->currentData.insert(
      MINCELLTEMPCELL,
      DataInfo(MINCELLTEMPCELL, "",
               convertNumberToDataInfoValue(minCellTempCellNumber)));
  this->currentData.insert(
      AVECELLVOLTAGE,
      DataInfo(AVECELLVOLTAGE, "",
               convertNumberToDataInfoValue(averageCellVoltage)));
  this->currentData.insert(
      AVECELLTEMP,
      DataInfo(AVECELLTEMP, "", convertNumberToDataInfoValue(averageCellTemp)));
  this->currentData.insert(
      PACKVOLTAGE,
      DataInfo(PACKVOLTAGE, "", convertNumberToDataInfoValue(packVoltage)));
  this->currentData.insert(
      BMSSTATE, DataInfo(BMSSTATE, "", convertNumberToDataInfoValue(bmsState)));
  this->currentData.insert(
      CURRENT,
      DataInfo(CURRENT, "", convertNumberToDataInfoValue(packCurrent)));
  this->currentData.insert(
      DCL, DataInfo(DCL, "", convertNumberToDataInfoValue(dcl)));
  this->currentData.insert(
      CCL, DataInfo(CCL, "", convertNumberToDataInfoValue(ccl)));
  this->currentData.insert(
      GFORCEX, DataInfo(GFORCEX, "", convertNumberToDataInfoValue(gforceX)));
  this->currentData.insert(
      GFORCEY, DataInfo(GFORCEY, "", convertNumberToDataInfoValue(gforceY)));
  this->currentData.insert(
      GFORCEZ, DataInfo(GFORCEZ, "", convertNumberToDataInfoValue(gforceZ)));
  this->currentData.insert(
      SEGMENTTEMP1,
      DataInfo(SEGMENTTEMP1, "", convertNumberToDataInfoValue(segment1Temp)));
  this->currentData.insert(
      SEGMENTTEMP2,
      DataInfo(SEGMENTTEMP2, "", convertNumberToDataInfoValue(segment2Temp)));
  this->currentData.insert(
      SEGMENTTEMP3,
      DataInfo(SEGMENTTEMP4, "", convertNumberToDataInfoValue(segment3Temp)));
  this->currentData.insert(
      SEGMENTTEMP4,
      DataInfo(SEGMENTTEMP4, "", convertNumberToDataInfoValue(segment4Temp)));
  this->currentData.insert(
      MOTORPOWER,
      DataInfo(MOTORPOWER, "", convertNumberToDataInfoValue(motorPower)));
  this->currentData.insert(
      FANPOWER, DataInfo(FANPOWER, "", convertNumberToDataInfoValue(fanPower)));
  this->currentData.insert(
      TORQUEPOWER,
      DataInfo(TORQUEPOWER, "", convertNumberToDataInfoValue(torquePower)));
  this->currentData.insert(
      REGENPOWER,
      DataInfo(REGENPOWER, "", convertNumberToDataInfoValue(regenPower)));
  this->currentData.insert(
      BURNINGCELLS,
      DataInfo(BURNINGCELLS, "", convertNumberToDataInfoValue(isBurning)));
  this->currentData.insert(
      INVERTERTEMP,
      DataInfo(INVERTERTEMP, "", convertNumberToDataInfoValue(inverterTemp)));
  emit onCurrentDataChange();
}
