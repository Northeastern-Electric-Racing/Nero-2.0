#ifndef MOCK_MODEL_H
#define MOCK_MODEL_H

#include "QTimer"
#include "model.h"

/**
 * @brief The MockModel class
 * Mocks the data for development when not connected to Calypso
 */
class MockModel : public Model {
  Q_OBJECT
  Q_INTERFACES(Model)

public:
  MockModel();
  ~MockModel();

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
  std::optional<bool> getForwardButtonPressed() override;
  std::optional<bool> getEnterButtonPressed() override;
  std::optional<bool> getUpButtonPressed() override;
  std::optional<bool> getDownButtonPressed() override;
  std::optional<bool> getBackwardButtonPressed() override;
  std::optional<bool> getRightButtonPressed() override;
  std::optional<float> getBalancingCells() override;
  std::optional<float> getTractionControl() override;
  std::optional<float> getCellDelta() override;
  std::optional<bool> getIsTalking() override;
  std::optional<int> getNumberOfCriticalFaults() override;
  std::optional<int> getNumberOfNonCriticalFaults() override;

private slots:
  void updateCurrentData() override;

private:
  QTimer *updateTimer;
  float mph;
  bool status;
  bool dir;
  float packTemp;
  float motorTemp;
  float stateOfCharge;
  float lvBattery;
  float current;
  float isBurning;
  float isDebug;
  float bmsFaults;
  float mpuFaults;
  float maxCellVoltage;
  float maxCellVoltageChipNumber;
  float maxCellVoltageCellNumber;
  float maxCellTemp;
  float maxCellTempChipNumber;
  float maxCellTempCellNumber;
  float minCellVoltage;
  float minCellVoltageChipNumber;
  float minCellVoltageCellNumber;
  float minCellTemp;
  float minCellTempChipNumber;
  float minCellTempCellNumber;
  float averageCellVoltage;
  float averageCellTemp;
  float packVoltage;
  float bmsState;
  float packCurrent;
  float dcl;
  float ccl;
  float gforceX;
  float gforceY;
  float gforceZ;
  float sdCardStatus;
  float segment1Temp;
  float segment2Temp;
  float segment3Temp;
  float segment4Temp;
  float motorPower;
  float fanPower;
  float torquePower;
  float regenPower;
  float modeIndex;
  float stateOfChargeDelta;
  float inverterTemp;
  float forward;
  float backward;
  float enter;
  float up;
  float down;
  float right;

  QVector<QString> convertNumberToDataInfoValue(float value);
};

#endif // MOCK_MODEL_H
