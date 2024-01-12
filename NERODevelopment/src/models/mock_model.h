#ifndef MOCK_MODEL_H
#define MOCK_MODEL_H

#include "model.h"

/**
 * @brief The MockModel class
 * Mocks the data for development when not connected to Calypso
 */
class MockModel : public Model {
public:
  MockModel();

  void connectToMQTT() override;
  std::optional<float> getMph() override;
  std::optional<float> getKph() override;
  std::optional<float> getStatus() override;
  std::optional<float> getDir() override;
  std::optional<float> getPackTemp() override;
  std::optional<float> getMotorTemp() override;
  std::optional<float> getStateOfCharge() override;
  std::optional<float> getCurrent() override;
  std::optional<float> getBmsFault() override;
  std::optional<float> getMpuFault() override;
  std::optional<float> getModeIndex() override;
  std::optional<float> getMaxCellVoltage() override;
  std::optional<float> getMaxCellVoltageChipNumber() override;
  std::optional<float> getMaxCellVoltageCellNumber() override;
  std::optional<float> getMaxCellTemp() override;
  std::optional<float> getMaxCellTempChipNumber() override;
  std::optional<float> getMaxCellTempCellNumber() override;
  std::optional<float> getMinCellVoltage() override;
  std::optional<float> getMinCellVoltageChipNumber() override;
  std::optional<float> getMinCellVoltageCellNumber() override;
  std::optional<float> getMinCellTemp() override;
  std::optional<float> getMinCellTempChipNumber() override;
  std::optional<float> getMinCellTempCellNumber() override;
  std::optional<float> getAveCellTemp() override;
  std::optional<float> getAveCellVoltage() override;
  std::optional<float> getPackVoltage() override;
  std::optional<float> getBmsState() override;
  std::optional<float> getPackCurrent() override;
  std::optional<float> getDcl() override;
  std::optional<float> getCcl() override;
  std::optional<float> getGForceX() override;
  std::optional<float> getGForceY() override;
  std::optional<float> getGForceZ() override;
  std::optional<float> getSegment1Temp() override;
  std::optional<float> getSegment2Temp() override;
  std::optional<float> getSegment3Temp() override;
  std::optional<float> getSegment4Temp() override;
  std::optional<float> getMotorPower() override;
  std::optional<float> getFanPower() override;
  std::optional<float> getTorquePower() override;
  std::optional<float> getRegenPower() override;
  std::optional<float> getBurningCells() override;
  std::optional<float> getInverterTemp() override;
  std::optional<QString> getForwardButtonPressed() override;
  std::optional<QString> getEnterButtonPressed() override;
  std::optional<QString> getUpButtonPressed() override;
  std::optional<QString> getDownButtonPressed() override;
  std::optional<QString> getBackwardButtonPressed() override;
  std::optional<QString> getRightButtonPressed() override;
  std::optional<float> getBalancingCells() override;
  std::optional<float> getTractionControl() override;
  std::optional<float> getCellDelta() override;

private:
  int mph;
  bool status;
  bool dir;
  int packTemp;
  int motorTemp;
  int stateOfCharge;
  int lvBattery;
  float current;
  int isBurning;
  int isDebug;
  int bmsFaults;
  int mpuFaults;
  float maxCellVoltage;
  int maxCellVoltageChipNumber;
  int maxCellVoltageCellNumber;
  int maxCellTemp;
  int maxCellTempChipNumber;
  int maxCellTempCellNumber;
  float minCellVoltage;
  int minCellVoltageChipNumber;
  int minCellVoltageCellNumber;
  int minCellTemp;
  int minCellTempChipNumber;
  int minCellTempCellNumber;
  float averageCellVoltage;
  int averageCellTemp;
  float packVoltage;
  int bmsState;
  int packCurrent;
  int dcl;
  int ccl;
  float gforceX;
  float gforceY;
  float gforceZ;
  int sdCardStatus;
  int segment1Temp;
  int segment2Temp;
  int segment3Temp;
  int segment4Temp;
  int motorPower;
  int fanPower;
  int torquePower;
  int regenPower;
  int modeIndex;
  int stateOfChargeDelta;
  int inverterTemp;
  int forward;
  int backward;
  int enter;
  int up;
  int down;
  int right;

  void updateData();
};

#endif // MOCK_MODEL_H
