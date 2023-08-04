#include "mock_model.h"
#include <math.h>

MockModel::MockModel()
    : mph(60), status(true), dir(true), packTemp(30), motorTemp(40),
      stateOfCharge(55), lvBattery(88), current(7.6), isBurning(0), isDebug(0),
      bmsFaults(0), mpuFaults(0), maxCellVoltage(3.5),
      maxCellVoltageChipNumber(1), maxCellVoltageCellNumber(10),
      maxCellTemp(30), maxCellTempChipNumber(5), maxCellTempCellNumber(8),
      minCellVoltage(3.2), minCellVoltageChipNumber(2),
      minCellVoltageCellNumber(3), minCellTemp(25), minCellTempChipNumber(3),
      minCellTempCellNumber(4), averageCellVoltage(3.3), averageCellTemp(27),
      packVoltage(3.4), bmsState(0), packCurrent(4), dcl(280), ccl(300),
      gforceX(0.5), gforceY(-1), gforceZ(0.5), sdCardStatus(4),
      segment1Temp(30), segment2Temp(50), segment3Temp(35), segment4Temp(15),
      motorPower(100), fanPower(100), torquePower(100), regenPower(1),
      modeIndex(0), stateOfChargeDelta(-1), inverterTemp(30), forward(0),
      backward(0), enter(0), up(0), down(0), right(0) {
  this->currentData.resize(45);
  updateData();
}

void MockModel::checkCan() {
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

  if (rng > 480 && rng < 485)
    sdCardStatus = rand() % 4;

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

  currentData = {mph,
                 status,
                 dir,
                 packTemp,
                 motorTemp,
                 stateOfCharge,
                 lvBattery,
                 current,
                 bmsFaults,
                 mpuFaults,
                 modeIndex,
                 maxCellVoltage,
                 maxCellVoltageChipNumber,
                 maxCellVoltageCellNumber,
                 maxCellTemp,
                 maxCellTempChipNumber,
                 maxCellTempCellNumber,
                 minCellVoltage,
                 minCellVoltageChipNumber,
                 minCellVoltageCellNumber,
                 minCellTemp,
                 minCellTempChipNumber,
                 minCellTempCellNumber,
                 averageCellVoltage,
                 averageCellTemp,
                 packVoltage,
                 bmsState,
                 packCurrent,
                 dcl,
                 ccl,
                 gforceX,
                 gforceY,
                 gforceZ,
                 sdCardStatus,
                 segment1Temp,
                 segment2Temp,
                 segment3Temp,
                 segment4Temp,
                 motorPower,
                 fanPower,
                 torquePower,
                 regenPower,
                 isBurning,
                 stateOfChargeDelta,
                 inverterTemp};
}

std::optional<float> MockModel::getMph() { return mph; }

std::optional<float> MockModel::getKph() { return round(mph * 1.609); }

std::optional<float> MockModel::getStatus() { return status; }

std::optional<float> MockModel::getDir() { return dir; }

std::optional<float> MockModel::getPackTemp() { return packTemp; }

std::optional<float> MockModel::getMotorTemp() { return motorTemp; }

std::optional<float> MockModel::getStateOfCharge() { return stateOfCharge; }

std::optional<float> MockModel::getLvBattery() { return lvBattery; }

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

std::optional<float> MockModel::getSdCardStatus() { return sdCardStatus; }

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

std::optional<float> MockModel::getPrecharge() {
  return std::nullopt; // TODO: Implement Mock Precharge
}

std::optional<float> MockModel::getBmsPrefault() {
  return std::nullopt; // TODO: Implement Mock BMS Default
}

std::optional<float> MockModel::getTractionControl() {
  return std::nullopt; // TODO: Implement Mock Traction Control
}

std::optional<float> MockModel::getVbat() {
  return std::nullopt; // TODO: Implement Mock Traction Control
}

std::optional<float> MockModel::getCellDelta() {
  return std::nullopt; // TODO: Implement Mock Cell Delta
}

std::optional<float> MockModel::getBalancingCells() {
  return std::nullopt; // TODO: Implement Mock balancing Cells
}

std::optional<std::string> MockModel::getForwardButtonPressed() {
  return std::to_string(forward);
}

std::optional<std::string> MockModel::getEnterButtonPressed() {
  return std::to_string(enter);
}

std::optional<std::string> MockModel::getUpButtonPressed() {
  return std::to_string(up);
}

std::optional<std::string> MockModel::getDownButtonPressed() {
  return std::to_string(down);
}

std::optional<std::string> MockModel::getBackwardButtonPressed() {
  return std::to_string(backward);
}

std::optional<std::string> MockModel::getRightButtonPressed() {
  return std::to_string(right);
}

std::optional<std::string> MockModel::getDebugPressed() {
  return std::to_string(isDebug);
}

void MockModel::updateData() {
  currentData[0] = mph;
  currentData[1] = status;
  currentData[2] = dir;
  currentData[3] = packTemp;
  currentData[4] = motorTemp;
  currentData[5] = stateOfCharge;
  currentData[6] = lvBattery;
  currentData[7] = current;
  currentData[8] = bmsFaults;
  currentData[9] = mpuFaults;
  currentData[10] = modeIndex;
  currentData[11] = maxCellVoltage;
  currentData[12] = maxCellVoltageChipNumber;
  currentData[13] = maxCellVoltageCellNumber;
  currentData[14] = maxCellTemp;
  currentData[15] = maxCellTempChipNumber;
  currentData[16] = maxCellTempCellNumber;
  currentData[17] = minCellVoltage;
  currentData[18] = minCellVoltageChipNumber;
  currentData[19] = minCellVoltageCellNumber;
  currentData[20] = minCellTemp;
  currentData[21] = minCellTempChipNumber;
  currentData[22] = minCellTempCellNumber;
  currentData[23] = averageCellVoltage;
  currentData[24] = averageCellTemp;
  currentData[25] = packVoltage;
  currentData[26] = bmsState;
  currentData[27] = packCurrent;
  currentData[28] = dcl;
  currentData[29] = ccl;
  currentData[30] = gforceX;
  currentData[31] = gforceY;
  currentData[32] = gforceZ;
  currentData[33] = sdCardStatus;
  currentData[34] = segment1Temp;
  currentData[35] = segment2Temp;
  currentData[36] = segment3Temp;
  currentData[37] = segment4Temp;
  currentData[38] = motorPower;
  currentData[39] = fanPower;
  currentData[40] = torquePower;
  currentData[41] = regenPower;
  currentData[42] = isBurning;
  currentData[43] = inverterTemp;
}
