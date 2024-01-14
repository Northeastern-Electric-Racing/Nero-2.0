#ifndef _WIN32
#include "raspberry_model.h"
#include <../utils/data_type_names.h>
#include <QtMqtt/QMqttClient>
#include <bitset>
#include <cmath>
#include <mqtt_client.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

RaspberryModel::RaspberryModel() {
  chdir("/home/ner/Desktop/Nero/");
  setenv("DISPLAY", ":0.0", 1);
}

RaspberryModel::~RaspberryModel() {}

std::optional<float> RaspberryModel::getMpuFault() {
  return std::nullopt; // TODO: Implement Mpu Faults
}

void RaspberryModel::connectToMQTT() {
  MqttClient *client = new MqttClient();
  client->connectToHost();
}

std::optional<float> RaspberryModel::getMph() {
  std::optional<float> mph = currentData[MPH].value;
  return mph ? std::optional<float>(std::round(*mph)) : std::nullopt;
}

std::optional<float> RaspberryModel::getKph() {
  std::optional<float> mph = currentData[KPH].value;
  return mph ? std::optional<float>(std::round(*mph * 1.601)) : std::nullopt;
}

std::optional<float> RaspberryModel::getStatus() {
  std::optional<float> status = currentData[STATUS].value;
  return status ? std::optional<float>((*status == 1)) : std::nullopt;
}

std::optional<float> RaspberryModel::getDir() {
  std::optional<float> dir = currentData[DIRECTION].value;
  return dir ? std::optional<float>((*dir == 1)) : std::nullopt;
}

std::optional<float> RaspberryModel::getPackTemp() {
  std::optional<float> packTemp = currentData[PACKTEMP].value;
  return packTemp ? std::optional<float>(std::round(*packTemp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getMotorTemp() {
  std::optional<float> motorTemp = currentData[MOTORTEMP].value;
  return motorTemp ? std::optional<float>(std::round(*motorTemp))
                   : std::nullopt;
}

std::optional<float> RaspberryModel::getStateOfCharge() {
  return currentData[STATEOFCHARGE].value;
}

std::optional<float> RaspberryModel::getCurrent() {
  return currentData[CURRENT].value;
}

std::optional<float> RaspberryModel::getMaxCellVoltage() {
  std::optional<float> voltage = currentData[MAXCELLVOLTAGE].value;
  return voltage ? std::optional<float>(std::round(*voltage * 1000))
                 : std::nullopt;
}

std::optional<float> RaspberryModel::getMaxCellVoltageChipNumber() {
  return currentData[MAXCELLVOLTAGECHIP].value;
}

std::optional<float> RaspberryModel::getMaxCellVoltageCellNumber() {
  return currentData[MAXCELLVOLTAGECELL].value;
}

std::optional<float> RaspberryModel::getMaxCellTemp() {
  std::optional<float> temp = currentData[MAXCELLTEMP].value;
  return temp ? std::optional<float>(std::round(*temp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getMaxCellTempChipNumber() {
  return currentData[MAXCELLTEMPCHIP].value;
}

std::optional<float> RaspberryModel::getMaxCellTempCellNumber() {
  return currentData[MAXCELLTEMPCELL].value;
}

std::optional<float> RaspberryModel::getMinCellVoltage() {
  std::optional<float> voltage = currentData[MINCELLVOLTAGE].value;
  return voltage ? std::optional<float>(std::round(*voltage * 1000))
                 : std::nullopt;
}

std::optional<float> RaspberryModel::getMinCellVoltageChipNumber() {
  return currentData[MINCELLVOLTAGECHIP].value;
}

std::optional<float> RaspberryModel::getMinCellVoltageCellNumber() {
  return currentData[MINCELLVOLTAGECELL].value;
}

std::optional<float> RaspberryModel::getMinCellTemp() {
  std::optional<float> temp = currentData[MINCELLTEMP].value;
  return temp ? std::optional<float>(std::round(*temp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getMinCellTempChipNumber() {
  return currentData[MINCELLTEMPCHIP].value;
}

std::optional<float> RaspberryModel::getMinCellTempCellNumber() {
  return currentData[MINCELLTEMPCELL].value;
}

std::optional<float> RaspberryModel::getAveCellTemp() {
  std::optional<float> temp = currentData[AVECELLTEMP].value;
  return temp ? std::optional<float>(std::round(*temp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getAveCellVoltage() {
  std::optional<float> voltage = currentData[AVECELLVOLTAGE].value;
  return voltage ? std::optional<float>(std::round(*voltage * 1000))
                 : std::nullopt;
}

std::optional<float> RaspberryModel::getCellDelta() {
  std::optional<float> max = getMaxCellVoltage();
  std::optional<float> min = getMinCellVoltage();
  return max && min ? std::optional<float>(std::round(*max - *min))
                    : std::nullopt;
}

std::optional<float> RaspberryModel::getPackVoltage() {
  return currentData[PACKVOLTAGE].value;
}

std::optional<float> RaspberryModel::getBmsState() {
  return currentData[BMSSTATE].value;
}

std::optional<float> RaspberryModel::getPackCurrent() {
  return currentData[CURRENT].value;
}

std::optional<float> RaspberryModel::getDcl() { return currentData[DCL].value; }

std::optional<float> RaspberryModel::getCcl() { return currentData[CCL].value; }

std::optional<float> RaspberryModel::getInverterTemp() {
  // std::optional<float> moduleATemp = currentData[18];
  // std::optional<float> moduleBTemp = currentData[19];
  // std::optional<float> moduleCTemp = currentData[20];
  // if (moduleATemp && moduleBTemp && moduleCTemp) {
  //   float averageTemp = (*moduleATemp + *moduleBTemp + *moduleCTemp) / 3;
  //   return std::optional<float>(std::round(averageTemp));
  // }
  // TODO: WILL CHANGE WITH NEW INVERTER
  return std::nullopt;
}

std::optional<float> RaspberryModel::getGForceX() {
  return currentData[GFORCEX].value;
}

std::optional<float> RaspberryModel::getGForceY() {
  return currentData[GFORCEY].value;
}

std::optional<float> RaspberryModel::getGForceZ() {
  return currentData[GFORCEZ].value;
}

std::optional<float> RaspberryModel::getBalancingCells() {
  return currentData[BALANCINGCELLS].value;
}

std::optional<float> RaspberryModel::getSegment1Temp() {
  return currentData[SEGMENTTEMP1].value;
}

std::optional<float> RaspberryModel::getSegment2Temp() {
  return currentData[SEGMENTTEMP2].value;
}

std::optional<float> RaspberryModel::getSegment3Temp() {
  return currentData[SEGMENTTEMP3].value;
}

std::optional<float> RaspberryModel::getSegment4Temp() {
  return currentData[SEGMENTTEMP4].value;
}

std::optional<float> RaspberryModel::getMotorPower() {
  return currentData[MOTORPOWER].value;
}

std::optional<float> RaspberryModel::getFanPower() {
  return currentData[FANPOWER].value;
}

std::optional<float> RaspberryModel::getTorquePower() {
  return currentData[TORQUEPOWER].value;
}

std::optional<float> RaspberryModel::getRegenPower() {
  return currentData[REGENPOWER].value;
}

std::optional<float> RaspberryModel::getTractionControl() {
  return currentData[TRACTIONCONTROL].value;
}

std::optional<float> RaspberryModel::getBmsFault() {
  std::optional<float> faultStatus = currentData[BMSFAULT].value;
  if (faultStatus && *faultStatus > 0) {
    FaultInstance faultInstance(
        *faultStatus, getMaxCellTemp().value_or(0),
        getMaxCellVoltage().value_or(0), getAveCellTemp().value_or(0),
        getAveCellVoltage().value_or(0), getMinCellTemp().value_or(0),
        getMinCellVoltage().value_or(0), getPackCurrent().value_or(0),
        getDcl().value_or(0), getCcl().value_or(0));
    faultInstances.push_back(faultInstance);
  }
  return faultStatus;
}

std::optional<QString> RaspberryModel::getForwardButtonPressed() {
  std::optional<float> value = currentData[FORWARDBUTTON].value;
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 7 ? std::optional<QString>(QString(1, binary[6]))
                                : std::nullopt;
  }
  return std::nullopt;
}

std::optional<QString> RaspberryModel::getBackwardButtonPressed() {
  std::optional<float> value = currentData[BACKWARDBUTTON].value;
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 8 ? std::optional<QString>(QString(1, binary[7]))
                                : std::nullopt;
  }
  return std::nullopt;
}

std::optional<QString> RaspberryModel::getRightButtonPressed() {
  std::optional<float> value = currentData[RIGHTBUTTON].value;
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 2 ? std::optional<QString>(QString(1, binary[1]))
                                : std::nullopt;
  }
  return std::nullopt;
}

std::optional<QString> RaspberryModel::getEnterButtonPressed() {
  std::optional<float> value = currentData[ENTERBUTTON].value;
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return std::optional<QString>(QString(1, binary[0]));
  }
  return std::nullopt;
}

std::optional<QString> RaspberryModel::getUpButtonPressed() {
  std::optional<float> value = currentData[UPBUTTON].value;
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 6 ? std::optional<QString>(QString(1, binary[5]))
                                : std::nullopt;
  }
  return std::nullopt;
}

std::optional<QString> RaspberryModel::getDownButtonPressed() {
  std::optional<float> value = currentData[DOWNBUTTON].value;
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 5 ? std::optional<QString>(QString(1, binary[4]))
                                : std::nullopt;
  }
  return std::nullopt;
}

std::optional<float> RaspberryModel::getModeIndex() {
  return currentData[MODEINDEX].value;
}

std::optional<float> RaspberryModel::getBurningCells() {
  return std::nullopt; // TODO: Implement Burning Cells
}

void RaspberryModel::updateCurrentData() { emit this->onCurrentDataChange(); }

#endif
