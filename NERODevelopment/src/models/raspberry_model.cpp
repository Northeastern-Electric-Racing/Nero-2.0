#ifndef _WIN32
#include "raspberry_model.h"
#include "../utils/data_type_names.h"
#include "mqtt_client.h"
#include <QtMqtt/QMqttClient>
#include <bitset>
#include <cmath>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

RaspberryModel::RaspberryModel() {}

RaspberryModel::~RaspberryModel() {}

std::optional<float> RaspberryModel::getMpuFault() {
  return std::nullopt; // TODO: Implement Mpu Faults
}

void RaspberryModel::connectToMQTT() {
  MqttClient *client = new MqttClient();
  connect(client, &MqttClient::emitServerData, this,
          &RaspberryModel::receiveServerData);
  client->connectToHost();
}

void RaspberryModel::receiveServerData(const serverdata::ServerData data,
                                       const QString topic) {
  this->currentData[topic] = DataInfo(topic, data.unit(), data.values());
}

std::optional<float> RaspberryModel::getMph() {
  std::optional<float> mph = this->getById(MPH);
  return mph ? std::optional<float>(std::round(*mph)) : std::nullopt;
}

std::optional<float> RaspberryModel::getKph() {
  std::optional<float> mph = this->getById(KPH);
  return mph ? std::optional<float>(std::round(*mph * 1.601)) : std::nullopt;
}

std::optional<float> RaspberryModel::getStatus() {
  std::optional<float> status = this->getById(STATUS);
  return status ? std::optional<float>((*status == 1)) : std::nullopt;
}

std::optional<float> RaspberryModel::getDir() {
  std::optional<float> dir = this->getById(DIRECTION);
  return dir ? std::optional<float>((*dir == 1)) : std::nullopt;
}

std::optional<float> RaspberryModel::getPackTemp() {
  std::optional<float> packTemp = this->getById(PACKTEMP);
  return packTemp ? std::optional<float>(std::round(*packTemp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getMotorTemp() {
  std::optional<float> motorTemp = this->getById(MOTORTEMP);
  return motorTemp ? std::optional<float>(std::round(*motorTemp))
                   : std::nullopt;
}

std::optional<float> RaspberryModel::getStateOfCharge() {
  return this->getById(STATEOFCHARGE);
}

std::optional<float> RaspberryModel::getCurrent() {
  return this->getById(CURRENT);
}

std::optional<float> RaspberryModel::getMaxCellVoltage() {
  std::optional<float> voltage = this->getById(MAXCELLVOLTAGE);
  return voltage ? std::optional<float>(std::round(*voltage * 1000))
                 : std::nullopt;
}

std::optional<float> RaspberryModel::getMaxCellVoltageChipNumber() {
  return this->getById(MAXCELLVOLTAGECHIP);
}

std::optional<float> RaspberryModel::getMaxCellVoltageCellNumber() {
  return this->getById(MAXCELLVOLTAGECELL);
}

std::optional<float> RaspberryModel::getMaxCellTemp() {
  std::optional<float> temp = this->getById(MAXCELLTEMP);
  return temp ? std::optional<float>(std::round(*temp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getMaxCellTempChipNumber() {
  return this->getById(MAXCELLTEMPCHIP);
}

std::optional<float> RaspberryModel::getMaxCellTempCellNumber() {
  return this->getById(MAXCELLTEMPCELL);
}

std::optional<float> RaspberryModel::getMinCellVoltage() {
  std::optional<float> voltage = this->getById(MINCELLVOLTAGE);
  return voltage ? std::optional<float>(std::round(*voltage * 1000))
                 : std::nullopt;
}

std::optional<float> RaspberryModel::getMinCellVoltageChipNumber() {
  return this->getById(MINCELLVOLTAGECHIP);
}

std::optional<float> RaspberryModel::getMinCellVoltageCellNumber() {
  return this->getById(MINCELLVOLTAGECELL);
}

std::optional<float> RaspberryModel::getMinCellTemp() {
  std::optional<float> temp = this->getById(MINCELLTEMP);
  return temp ? std::optional<float>(std::round(*temp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getMinCellTempChipNumber() {
  return this->getById(MINCELLTEMPCHIP);
}

std::optional<float> RaspberryModel::getMinCellTempCellNumber() {
  return this->getById(MINCELLTEMPCELL);
}

std::optional<float> RaspberryModel::getAveCellTemp() {
  std::optional<float> temp = this->getById(AVECELLTEMP);
  return temp ? std::optional<float>(std::round(*temp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getAveCellVoltage() {
  std::optional<float> voltage = this->getById(AVECELLVOLTAGE);
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
  return this->getById(PACKVOLTAGE);
}

std::optional<float> RaspberryModel::getBmsState() {
  return this->getById(BMSSTATE);
}

std::optional<float> RaspberryModel::getPackCurrent() {
  return this->getById(CURRENT);
}

std::optional<float> RaspberryModel::getDcl() { return this->getById(DCL); }

std::optional<float> RaspberryModel::getCcl() { return this->getById(CCL); }

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
  return this->getById(GFORCEX);
}

std::optional<float> RaspberryModel::getGForceY() {
  return this->getById(GFORCEY);
}

std::optional<float> RaspberryModel::getGForceZ() {
  return this->getById(GFORCEZ);
}

std::optional<float> RaspberryModel::getBalancingCells() {
  return this->getById(BALANCINGCELLS);
}

std::optional<float> RaspberryModel::getSegment1Temp() {
  return this->getById(SEGMENTTEMP1);
}

std::optional<float> RaspberryModel::getSegment2Temp() {
  return this->getById(SEGMENTTEMP2);
}

std::optional<float> RaspberryModel::getSegment3Temp() {
  return this->getById(SEGMENTTEMP3);
}

std::optional<float> RaspberryModel::getSegment4Temp() {
  return this->getById(SEGMENTTEMP4);
}

std::optional<float> RaspberryModel::getMotorPower() {
  return this->getById(MOTORPOWER);
}

std::optional<float> RaspberryModel::getFanPower() {
  return this->getById(FANPOWER);
}

std::optional<float> RaspberryModel::getTorquePower() {
  return this->getById(TORQUEPOWER);
}

std::optional<float> RaspberryModel::getRegenPower() {
  return this->getById(REGENPOWER);
}

std::optional<float> RaspberryModel::getTractionControl() {
  return this->getById(TRACTIONCONTROL);
}

std::optional<float> RaspberryModel::getBmsFault() {
  std::optional<float> faultStatus = this->getById(BMSFAULT);
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

std::optional<bool> RaspberryModel::getForwardButtonPressed() {
  std::optional<float> value = this->getById(FORWARDBUTTON);
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 7 ? binary[6] : false;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getBackwardButtonPressed() {
  std::optional<float> value = this->getById(BACKWARDBUTTON);
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 8 ? binary[7] == 1 : false;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getRightButtonPressed() {
  std::optional<float> value = this->getById(RIGHTBUTTON);
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 2 ? binary[1] == 1 : false;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getEnterButtonPressed() {
  std::optional<float> value = this->getById(ENTERBUTTON);
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary[0] == 1;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getUpButtonPressed() {
  std::optional<float> value = this->getById(UPBUTTON);
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 6 ? binary[5] == 1 : false;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getDownButtonPressed() {
  std::optional<float> value = this->getById(DOWNBUTTON);
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 5 ? binary[4] == 1 : false;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getHomeButtonPressed() {
  std::optional<float> value = this->getById(HOMEBUTTON);
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 1 ? binary[1] == 1 : false;
  }
  return std::nullopt;
}

std::optional<float> RaspberryModel::getModeIndex() {
  return this->getById(MODEINDEX);
}

std::optional<float> RaspberryModel::getBurningCells() {
  return std::nullopt; // TODO: Implement Burning Cells
}

void RaspberryModel::updateCurrentData() { emit this->onCurrentDataChange(); }

std::optional<bool> RaspberryModel::getIsTalking() {
  std::optional<float> value = this->getById(MICROPHONE);
  if (value && value > 0) {
    return true;
  }
  return false;
}

std::optional<int> RaspberryModel::getNumberOfCriticalFaults() {
  std::optional<float> value = this->getById(CRITICALFAULTS);
  if (value) {
    return this->totalNumberOfOnesIn(*value);
  }
  return std::nullopt;
}

std::optional<int> RaspberryModel::getNumberOfNonCriticalFaults() {
  std::optional<float> value = this->getById(NONCRITICALFAULTS);
  if (value) {
    return this->totalNumberOfOnesIn(*value);
  }
  return std::nullopt;
}

int RaspberryModel::totalNumberOfOnesIn(float value) {
  int total = 0;
  std::string binary = std::bitset<8>(static_cast<int>(value)).to_string();
  for (int num : binary) {
    if (num == 1) {
      total++;
    }
  }
  return total;
}

#endif
