#ifndef RASPBERRY_MODEL_H
#define RASPBERRY_MODEL_H
#ifndef _WIN32

#include "model.h"

/**
 * @brief The RaspberryModel class
 * The Production Model that is used when running on the car
 */
class RaspberryModel : public Model {

public:
  RaspberryModel();
  ~RaspberryModel();

  // Implementations of virtual functions from the Model class
  void connectToMQTT() override;
  std::optional<float> getPrecharge() override;
  std::optional<float> getMph() override;
  std::optional<float> getKph() override;
  std::optional<float> getStatus() override;
  std::optional<float> getDir() override;
  std::optional<float> getPackTemp() override;
  std::optional<float> getMotorTemp() override;
  std::optional<float> getStateOfCharge() override;
  std::optional<float> getLvBattery() override;
  std::optional<float> getCurrent() override;
  std::optional<float> getBalancingCells() override;
  std::optional<float> getPackVoltage() override;
  std::optional<float> getMaxCellTemp() override;
  std::optional<float> getMaxCellTempChipNumber() override;
  std::optional<float> getMaxCellTempCellNumber() override;
  std::optional<float> getMaxCellVoltage() override;
  std::optional<float> getMaxCellVoltageChipNumber() override;
  std::optional<float> getMaxCellVoltageCellNumber() override;
  std::optional<float> getMinCellTemp() override;
  std::optional<float> getMinCellTempChipNumber() override;
  std::optional<float> getMinCellTempCellNumber() override;
  std::optional<float> getMinCellVoltage() override;
  std::optional<float> getMinCellVoltageChipNumber() override;
  std::optional<float> getMinCellVoltageCellNumber() override;
  std::optional<float> getAveCellTemp() override;
  std::optional<float> getAveCellVoltage() override;
  std::optional<float> getCellDelta() override;
  std::optional<float> getBurningCells() override;
  std::optional<float> getTractionControl() override;
  std::optional<float> getInverterTemp() override;
  std::optional<float> getMotorPower() override;
  std::optional<float> getFanPower() override;
  std::optional<float> getTorquePower() override;
  std::optional<float> getRegenPower() override;
  std::optional<float> getBmsState() override;
  std::optional<float> getBmsFault() override;
  std::optional<float> getMpuFault() override;
  std::optional<float> getDcl() override;
  std::optional<float> getCcl() override;
  std::optional<float> getPackCurrent() override;
  std::optional<std::string> getForwardButtonPressed() override;
  std::optional<std::string> getBackwardButtonPressed() override;
  std::optional<std::string> getDebugPressed() override;
  std::optional<std::string> getRightButtonPressed() override;
  std::optional<std::string> getEnterButtonPressed() override;
  std::optional<std::string> getUpButtonPressed() override;
  std::optional<std::string> getDownButtonPressed() override;
  std::optional<float> getModeIndex() override;
  std::optional<float> getGForceX() override;
  std::optional<float> getGForceY() override;
  std::optional<float> getGForceZ() override;
  std::optional<float> getSegment1Temp() override;
  std::optional<float> getSegment2Temp() override;
  std::optional<float> getSegment3Temp() override;
  std::optional<float> getSegment4Temp() override;
  std::optional<float> getSdCardStatus() override;
  std::optional<float> getVbat() override;
  std::optional<float> getBmsPrefault() override;

private:
  void processData(const std::string &data);
};
#endif
#endif // RASPBERRY_MODEL_H
