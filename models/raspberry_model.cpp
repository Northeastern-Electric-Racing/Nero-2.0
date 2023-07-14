#include "raspberry_model.h"
#include <bitset>
#include <cmath>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>

RaspberryModel::RaspberryModel() {
  currentData.resize(DATA_IDS.size());
  chdir("/home/ner/Desktop/Nero/");
  setenv("DISPLAY", ":0.0", 1);
  std::thread(&RaspberryModel::connectToIPC, this).detach();
}

RaspberryModel::~RaspberryModel() {}

void RaspberryModel::checkCan() {}

std::optional<float> RaspberryModel::getMpuFault() {
  return std::nullopt; // TODO: Implement Mpu Faults
}

void RaspberryModel::connectToIPC() {
  std::string socketPath = "/tmp/ipc.sock";
  unlink(socketPath.c_str());

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("Failed to create socket");
    return;
  }

  sockaddr_un addr{};
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path) - 1);

  if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Failed to bind socket");
    close(fd);
    return;
  }

  if (listen(fd, 0) < 0) {
    perror("Failed to listen on socket");
    close(fd);
    return;
  }

  system("/home/ner/Desktop/Ner_Processing/target/release/ner_processing &");

  while (true) {
    int conn = accept(fd, nullptr, nullptr);
    if (conn < 0) {
      perror("Failed to accept connection");
      close(fd);
      return;
    }

    char buffer[1024];
    ssize_t bytesRead = recv(conn, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
      buffer[bytesRead] = '\0';
      std::string data(buffer);
      processData(data);
    }
    close(conn);
  }

  close(fd);
}

void RaspberryModel::processData(const std::string &data) {
  size_t start = 0;
  size_t pos;
  while ((pos = data.find("index:", start)) != std::string::npos) {
    std::string value = data.substr(start, pos - start);
    start = pos + 6;

    size_t commaPos = value.find(',');
    if (commaPos != std::string::npos) {
      int index = std::stoi(value.substr(0, commaPos));
      float floatValue = std::stof(value.substr(commaPos + 1));
      if (index >= 0 && index < currentData.size()) {
        currentData[index] = floatValue;
      }
    }
  }
}

std::optional<float> RaspberryModel::getMph() {
  std::optional<float> mph = currentData[101];
  return mph ? std::optional<float>(std::round(*mph)) : std::nullopt;
}

std::optional<float> RaspberryModel::getKph() {
  std::optional<float> kph = currentData[101];
  return kph ? std::optional<float>(std::round(*kph * 1.601)) : std::nullopt;
}

std::optional<float> RaspberryModel::getStatus() {
  std::optional<float> status = currentData[85];
  return status ? std::optional<float>((*status == 1)) : std::nullopt;
}

std::optional<float> RaspberryModel::getDir() {
  std::optional<float> dir = currentData[84];
  return dir ? std::optional<float>((*dir == 1)) : std::nullopt;
}

std::optional<float> RaspberryModel::getPackTemp() {
  std::optional<float> packTemp = currentData[10];
  return packTemp ? std::optional<float>(std::round(*packTemp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getMotorTemp() {
  std::optional<float> motorTemp = currentData[28];
  return motorTemp ? std::optional<float>(std::round(*motorTemp))
                   : std::nullopt;
}

std::optional<float> RaspberryModel::getStateOfCharge() {
  return currentData[4];
}

std::optional<float> RaspberryModel::getPrecharge() { return currentData[145]; }

std::optional<float> RaspberryModel::getLvBattery() { return currentData[63]; }

std::optional<float> RaspberryModel::getCurrent() { return currentData[2]; }

std::optional<float> RaspberryModel::getMaxCellVoltage() {
  std::optional<float> voltage = currentData[13];
  return voltage ? std::optional<float>(std::round(*voltage * 1000))
                 : std::nullopt;
}

std::optional<float> RaspberryModel::getMaxCellVoltageChipNumber() {
  return currentData[121];
}

std::optional<float> RaspberryModel::getMaxCellVoltageCellNumber() {
  return currentData[122];
}

std::optional<float> RaspberryModel::getMaxCellTemp() {
  std::optional<float> temp = currentData[114];
  return temp ? std::optional<float>(std::round(*temp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getMaxCellTempChipNumber() {
  return currentData[115];
}

std::optional<float> RaspberryModel::getMaxCellTempCellNumber() {
  return currentData[116];
}

std::optional<float> RaspberryModel::getMinCellVoltage() {
  std::optional<float> voltage = currentData[15];
  return voltage ? std::optional<float>(std::round(*voltage * 1000))
                 : std::nullopt;
}

std::optional<float> RaspberryModel::getMinCellVoltageChipNumber() {
  return currentData[123];
}

std::optional<float> RaspberryModel::getMinCellVoltageCellNumber() {
  return currentData[124];
}

std::optional<float> RaspberryModel::getMinCellTemp() {
  std::optional<float> temp = currentData[117];
  return temp ? std::optional<float>(std::round(*temp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getMinCellTempChipNumber() {
  return currentData[118];
}

std::optional<float> RaspberryModel::getMinCellTempCellNumber() {
  return currentData[119];
}

std::optional<float> RaspberryModel::getAveCellTemp() {
  std::optional<float> temp = currentData[120];
  return temp ? std::optional<float>(std::round(*temp)) : std::nullopt;
}

std::optional<float> RaspberryModel::getAveCellVoltage() {
  std::optional<float> voltage = currentData[17];
  return voltage ? std::optional<float>(std::round(*voltage * 1000))
                 : std::nullopt;
}

std::optional<float> RaspberryModel::getCellDelta() {
  std::optional<float> max = getMaxCellVoltage();
  std::optional<float> min = getMinCellVoltage();
  return max && min ? std::optional<float>(std::round(*max - *min))
                    : std::nullopt;
}

std::optional<float> RaspberryModel::getPackVoltage() { return currentData[1]; }

std::optional<float> RaspberryModel::getBmsState() { return currentData[106]; }

std::optional<float> RaspberryModel::getPackCurrent() { return currentData[2]; }

std::optional<float> RaspberryModel::getDcl() { return currentData[89]; }

std::optional<float> RaspberryModel::getCcl() { return currentData[90]; }

std::optional<float> RaspberryModel::getInverterTemp() {
  std::optional<float> moduleATemp = currentData[18];
  std::optional<float> moduleBTemp = currentData[19];
  std::optional<float> moduleCTemp = currentData[20];
  if (moduleATemp && moduleBTemp && moduleCTemp) {
    float averageTemp = (*moduleATemp + *moduleBTemp + *moduleCTemp) / 3;
    return std::optional<float>(std::round(averageTemp));
  }
  return std::nullopt;
}

std::optional<float> RaspberryModel::getGForceX() { return currentData[91]; }

std::optional<float> RaspberryModel::getGForceY() { return currentData[92]; }

std::optional<float> RaspberryModel::getGForceZ() { return currentData[93]; }

std::optional<float> RaspberryModel::getVbat() { return currentData[139]; }

std::optional<float> RaspberryModel::getBalancingCells() {
  return currentData[143];
}

std::optional<float> RaspberryModel::getSdCardStatus() {
  return currentData[129];
}

std::optional<float> RaspberryModel::getSegment1Temp() {
  return currentData[125];
}

std::optional<float> RaspberryModel::getSegment2Temp() {
  return currentData[126];
}

std::optional<float> RaspberryModel::getSegment3Temp() {
  return currentData[127];
}

std::optional<float> RaspberryModel::getSegment4Temp() {
  return currentData[128];
}

std::optional<float> RaspberryModel::getMotorPower() {
  return currentData[131];
}

std::optional<float> RaspberryModel::getFanPower() { return currentData[130]; }

std::optional<float> RaspberryModel::getTorquePower() {
  return currentData[132];
}

std::optional<float> RaspberryModel::getRegenPower() {
  return currentData[133];
}

std::optional<float> RaspberryModel::getTractionControl() {
  return currentData[144];
}

std::optional<float> RaspberryModel::getBmsPrefault() {
  return currentData[146];
}

std::optional<float> RaspberryModel::getBmsFault() {
  std::optional<float> faultStatus = currentData[107];
  if (faultStatus && *faultStatus > 0) {
    FaultInstance faultInstance(
        *faultStatus, getMaxCellTemp().value_or(0),
        getMaxCellVoltage().value_or(0), getAveCellTemp().value_or(0),
        getAveCellVoltage().value_or(0), getMinCellTemp().value_or(0),
        getMinCellVoltage().value_or(0), getPackCurrent().value_or(0),
        getDcl().value_or(0), getCcl().value_or(0));
    faultInstances.push_back(faultInstance);
  }
  return currentData[107];
}

std::optional<std::string> RaspberryModel::getForwardButtonPressed() {
  std::optional<float> value = currentData[104];
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 7
               ? std::optional<std::string>(std::string(1, binary[6]))
               : std::nullopt;
  }
  return std::nullopt;
}

std::optional<std::string> RaspberryModel::getBackwardButtonPressed() {
  std::optional<float> value = currentData[104];
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 8
               ? std::optional<std::string>(std::string(1, binary[7]))
               : std::nullopt;
  }
  return std::nullopt;
}

std::optional<std::string> RaspberryModel::getDebugPressed() {
  std::optional<float> value = currentData[104];
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 3
               ? std::optional<std::string>(std::string(1, binary[2]))
               : std::nullopt;
  }
  return std::nullopt;
}

std::optional<std::string> RaspberryModel::getRightButtonPressed() {
  std::optional<float> value = currentData[104];
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 2
               ? std::optional<std::string>(std::string(1, binary[1]))
               : std::nullopt;
  }
  return std::nullopt;
}

std::optional<std::string> RaspberryModel::getEnterButtonPressed() {
  std::optional<float> value = currentData[104];
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return std::optional<std::string>(std::string(1, binary[0]));
  }
  return std::nullopt;
}

std::optional<std::string> RaspberryModel::getUpButtonPressed() {
  std::optional<float> value = currentData[104];
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 6
               ? std::optional<std::string>(std::string(1, binary[5]))
               : std::nullopt;
  }
  return std::nullopt;
}

std::optional<std::string> RaspberryModel::getDownButtonPressed() {
  std::optional<float> value = currentData[104];
  if (value) {
    std::string binary = std::bitset<8>(static_cast<int>(*value)).to_string();
    return binary.length() >= 5
               ? std::optional<std::string>(std::string(1, binary[4]))
               : std::nullopt;
  }
  return std::nullopt;
}

std::optional<float> RaspberryModel::getModeIndex() { return currentData[105]; }

std::optional<float> RaspberryModel::getBurningCells() {
  return std::nullopt; // TODO: Implement Burning Cells
}
