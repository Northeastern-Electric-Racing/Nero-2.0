#ifndef RASPBERRY_MODEL_H
#define RASPBERRY_MODEL_H

#include "model.h"
#include "mqtt_client.h"
#include <serverdata.qpb.h>
/**
 * @brief The RaspberryModel class
 * The Production Model that is used when running on the car
 *
 * Wheel button layout — published to "Wheel/Buttons/button_id" as a 0-indexed
 * ordinal (0..9). The order mirrors the `button_t` enum in
 * Cerberus-2.0/Core/Inc/u_buttons.h; the two must stay in sync. See
 * readme.md "Button Layout" for the full table and the Confluence link.
 */
class RaspberryModel : public Model {
  Q_OBJECT
  Q_INTERFACES(Model)

public:
  RaspberryModel();
  ~RaspberryModel();

  // Implementations of virtual functions from the Model class
  void connectToMQTT() override;
  std::optional<float> getMph() override;
  std::optional<float> getStatus() override;
  std::optional<float> getDir() override;
  std::optional<float> getPackTemp() override;
  std::optional<float> getMotorTemp() override;
  std::optional<float> getMaxRegenCapacity() override;
  std::optional<float> getStateOfCharge() override;
  std::optional<float> getCurrent() override;
  std::optional<float> getDCCurrent() override;
  std::optional<float> getMaxDCCurrentTarget() override;
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
  std::optional<float> getTractionControl() override;
  std::optional<float> getInverterTemp() override;
  std::optional<float> getMotorPower() override;
  std::optional<float> getFanPower() override;
  std::optional<float> getTorquePower() override;
  std::optional<float> getRegenPower() override;
  std::optional<float> getBmsState() override;
  QList<QString> getBmsFault() override;
  QList<QString> getVcuFault() override;
  std::optional<float> getDcl() override;
  std::optional<float> getCcl() override;
  std::optional<float> getPackCurrent() override;
  bool buttonNum0Pressed() override;
  bool buttonNum1Pressed() override;
  bool buttonNum2Pressed() override;
  bool buttonNum3Pressed() override;
  bool buttonNum4Pressed() override;
  bool buttonNum5Pressed() override;
  bool buttonNum6Pressed() override;
  bool buttonNum7Pressed() override;
  bool buttonNum8Pressed() override;
  bool buttonNum9Pressed() override;

  std::optional<bool> getEscButtonPressed() override;
  std::optional<bool> getLeftButtonPressed() override;
  std::optional<bool> getLaunchControlToggleButtonPressed() override;
  std::optional<bool> getUpRegenButtonPressed() override;
  std::optional<bool> getDownRegenButtonPressed() override;
  std::optional<bool> getEnterButtonPressed() override;
  std::optional<bool> getRightButtonPressed() override;
  std::optional<bool> getTractionControlToggleButtonPressed() override;
  std::optional<bool> getUpTorqueButtonPressed() override;
  std::optional<bool> getDownTorqueButtonPressed() override;

  std::optional<bool> getHomeButtonPressed() override;
  std::optional<float> getModeIndex() override;
  std::optional<float> getFunctionalState() override;
  std::optional<float> getStateRejectionError() override;
  std::optional<float> getGForceX() override;
  std::optional<float> getGForceY() override;
  std::optional<float> getGForceZ() override;
  std::optional<float> getSegment1Temp() override;
  std::optional<float> getSegment2Temp() override;
  std::optional<float> getSegment3Temp() override;
  std::optional<float> getSegment4Temp() override;
  QList<QString> getCriticalFaults() override;
  QList<QString> getNonCriticalFaults() override;
  std::optional<float> getLowVoltage() override;

  void sendMessage(const QString topic, const float value) override;
  void sendMessage(const QString topic, const QList<float> values) override;

private slots:
  void updateCurrentData() override;
  void receiveServerData(const serverdata::v2::ServerData, const QString topic);

private:
  void processData(const std::string &data);
  int totalNumberOfOnesIn(float value);
  MqttClient *m_client;
};
#endif
