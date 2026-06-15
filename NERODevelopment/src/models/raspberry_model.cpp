#include "raspberry_model.h"
#include "../utils/data_type_names.h"
#include "mqtt_client.h"
#include <QDebug>
#include <QtMqtt/QMqttClient>
#include <bitset>
#include <cmath>
#include <cstdlib>

#ifndef _WIN32
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#endif

RaspberryModel::RaspberryModel() {}

RaspberryModel::~RaspberryModel() {}

QList<QString> RaspberryModel::getVcuFault() {
  QRegularExpression regex("^VCU/Faults/.*$");

  QList<QString> faults;
  for (auto it = this->currentData.begin(); it != this->currentData.end();
       ++it) {
    if (regex.match(it.key()).hasMatch() && it.value().values[0] == 1) {
      faults.append(it.key());
    }
  }
  return faults;
}

void RaspberryModel::connectToMQTT() {
  // Determine MQTT broker hostname based on environment
  // Priority: 1. MQTT_HOST env var, 2. Default localhost for development
  QString mqttHost =
      getenv("HOST") ? QString(getenv("HOST")) : QString("localhost");

  qInfo() << "RaspberryModel connecting to MQTT broker:" << mqttHost;

  QList<QString> client_1_topics = {
      MPH,
      STATUS,
      PACKTEMP,
      MOTORTEMP,
      STATEOFCHARGE,
      CURRENT,
      BALANCINGCELLS,
      PACKVOLTAGE,
      MAXCELLTEMP,
      MAXCELLTEMPCHIP,
      MAXCELLTEMPCELL,
      MAXCELLVOLTAGE,
      MAXCELLVOLTAGECHIP,
      MAXCELLVOLTAGECELL,
      MINCELLTEMP,
      MINCELLTEMPCHIP,
      MINCELLTEMPCELL,
      MINCELLVOLTAGE,
      MINCELLVOLTAGECHIP,
      MINCELLVOLTAGECELL,
      AVECELLTEMP,
      AVECELLVOLTAGE,
      TRACTIONCONTROL,
      INVERTERTEMP,
      BMSSTATE,
      BMSFAULT,
      VCUFAULT,
      DCL,
      CCL,
      REGENPOWER,
      TORQUEPOWER,
      GFORCE,
      SEGMENTTEMP1,
      SEGMENTTEMP2,
      SEGMENTTEMP3,
      SEGMENTTEMP4,
      MOTORPOWER,
      FANPOWER,
      EFUSE_SHUTDOWN_ENABLED,
      EFUSE_SHUTDOWN_FAULTED,
      BMS,
      BSPD,
      EFUSE_MC_ENABLED,
      EFUSE_MC_FAULTED,
      BOTS,
      INERTIA,
      CPBRB,
      TSMS,
      IMD,
      HVDINTRLK,
      HVCNCTR,
      CRITICALFAULTS,
      NONCRITICALFAULTS,
      LVVOLTAGE,

  };

  const char *client1_port_str = getenv("CLIENT1_PORT");
  const char *client2_port_str = getenv("CLIENT2_PORT");

  int client1_port = client1_port_str ? atoi(client1_port_str) : 1883;
  int client2_port = client2_port_str ? atoi(client2_port_str) : 1882;

  MqttClient *client_1 =
      new MqttClient(nullptr, client1_port, client_1_topics, mqttHost);
  connect(client_1, &MqttClient::emitServerData, this,
          &RaspberryModel::receiveServerData);
  client_1->connectToHost();

  QList<QString> client_2_topics = {
      BUTTONID,
      HOMEBUTTON,
      MODEINDEX,
      DIRECTION,
  };
  MqttClient *client_2 =
      new MqttClient(nullptr, client2_port, client_2_topics, mqttHost);
  connect(client_2, &MqttClient::emitServerData, this,
          &RaspberryModel::receiveServerData);
  client_2->connectToHost();
  this->m_client = client_1;
}

void RaspberryModel::sendMessage(const QString topic, const float value) {
  this->m_client->sendMessage(topic, value);
}

void RaspberryModel::receiveServerData(const serverdata::v2::ServerData data,
                                       const QString topic) {
  // qDebug() << "Topic: " << topic << "Data" << data.values();
  this->currentData[topic] = DataInfo(topic, data.unit(), data.values());
  emit this->onCurrentDataChange();
}

std::optional<float> RaspberryModel::getMph() {
  std::optional<float> mph = this->getById(MPH);
  return mph ? std::optional<float>(std::round(*mph)) : std::nullopt;
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

std::optional<float> RaspberryModel::getMaxRegenCapacity() {
  // CCL ("BMS/Commands/Max_DC_Brake_Current_Target") is the BMS-imposed cap on
  // regen current — i.e. the max regen capacity used to compute regen %.
  std::optional<float> v = this->getById(CCL);
  return v ? std::optional<float>(std::round(*v)) : std::nullopt;
}

std::optional<float> RaspberryModel::getStateOfCharge() {
  std::optional<float> soc = this->getById(STATEOFCHARGE);
  return soc ? std::optional<float>(std::round(*soc * 100)) : std::nullopt;
}

std::optional<float> RaspberryModel::getCurrent() {
  return this->getById(CURRENT);
}

std::optional<float> RaspberryModel::getDCCurrent() {
  return this->getById(CURRENT);
}

std::optional<float> RaspberryModel::getMaxDCCurrentTarget() {
  return this->getById(DCL);
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
  return this->getById(GFORCE, 0);
}

std::optional<float> RaspberryModel::getGForceY() {
  return this->getById(GFORCE, 1);
}

std::optional<float> RaspberryModel::getGForceZ() {
  return this->getById(GFORCE, 2);
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

QList<QString> RaspberryModel::getBmsFault() {
  QRegularExpression regex("^BMS/Faults/Critical/.*$");

  QList<QString> faults;
  for (auto it = this->currentData.begin(); it != this->currentData.end();
       ++it) {
    if (regex.match(it.key()).hasMatch() && it.value().values[0] == 1) {
      faults.append(it.key());
    }
  }
  return faults;
}

bool RaspberryModel::buttonNum0Pressed() {
  return this->getById(BUTTONID) == 0;
}
bool RaspberryModel::buttonNum1Pressed() {
  return this->getById(BUTTONID) == 1;
}
bool RaspberryModel::buttonNum2Pressed() {
  return this->getById(BUTTONID) == 2;
}
bool RaspberryModel::buttonNum3Pressed() {
  return this->getById(BUTTONID) == 3;
}
bool RaspberryModel::buttonNum4Pressed() {
  return this->getById(BUTTONID) == 4;
}
bool RaspberryModel::buttonNum5Pressed() {
  return this->getById(BUTTONID) == 5;
}
bool RaspberryModel::buttonNum6Pressed() {
  return this->getById(BUTTONID) == 6;
}
bool RaspberryModel::buttonNum7Pressed() {
  return this->getById(BUTTONID) == 7;
}
bool RaspberryModel::buttonNum8Pressed() {
  return this->getById(BUTTONID) == 8;
}
bool RaspberryModel::buttonNum9Pressed() {
  return this->getById(BUTTONID) == 9;
}

// Sentinel value written to BUTTONID to latch "this press has been consumed"
// so the next tick doesn't re-fire on stale MQTT data. 10 is outside the
// valid 0..9 ordinal range.
static constexpr float BUTTON_CONSUMED_SENTINEL = 10;

std::optional<bool> RaspberryModel::getEscButtonPressed() {
  if (buttonNum0Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getLeftButtonPressed() {
  if (buttonNum1Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getLaunchControlToggleButtonPressed() {
  if (buttonNum2Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getUpRegenButtonPressed() {
  if (buttonNum3Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getDownRegenButtonPressed() {
  if (buttonNum4Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getEnterButtonPressed() {
  if (buttonNum5Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getRightButtonPressed() {
  if (buttonNum6Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getTractionControlToggleButtonPressed() {
  if (buttonNum7Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getUpTorqueButtonPressed() {
  if (buttonNum8Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getDownTorqueButtonPressed() {
  if (buttonNum9Pressed()) {
    this->setValue(BUTTONID, BUTTON_CONSUMED_SENTINEL);
    return true;
  }
  return std::nullopt;
}

std::optional<bool> RaspberryModel::getHomeButtonPressed() {
  std::optional<float> value = this->getById(HOMEBUTTON);
  if (value) {
    return *value == 1;
  }
  return std::nullopt;
}

std::optional<float> RaspberryModel::getModeIndex() {
  return this->getById(MODEINDEX);
}

void RaspberryModel::updateCurrentData() { emit this->onCurrentDataChange(); }

QList<QString> RaspberryModel::getCriticalFaults() {
  QRegularExpression regex("^(BMS/Faults/Critical/.*|VCU/Faults/Critical/.*)$");

  QList<QString> faults;

  for (auto it = this->currentData.begin(); it != this->currentData.end();
       ++it) {
    if (regex.match(it.key()).hasMatch() && it.value().values[0] == 1) {
      faults.append(it.key());
    }
  }

  return faults;
}

QList<QString> RaspberryModel::getNonCriticalFaults() {
  QRegularExpression regex(
      "^(BMS/Faults/Non-Critical/.*|VCU/Faults/Non-Critical/.*)$");

  QList<QString> faults;

  for (auto it = this->currentData.begin(); it != this->currentData.end();
       ++it) {
    if (regex.match(it.key()).hasMatch() && it.value().values[0] == 1) {
      faults.append(it.key());
    }
  }

  return faults;
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

std::optional<float> RaspberryModel::getLowVoltage() {
  return this->getById(LVVOLTAGE);
}
