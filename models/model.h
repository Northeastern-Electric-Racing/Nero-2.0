#ifndef MODEL_H
#define MODEL_H

#include "../modes/debug_mode/debug_utils.h"
#include <QList>
#include <QMap>
#include <QObject>
#include <QVariant>

struct DataInfo {
  QString name;
  QString units;
};

extern const std::map<int, DataInfo> DATA_IDS;

class Model {

public:
  explicit Model();
  virtual ~Model() {}

  virtual void checkCan() = 0;
  virtual std::optional<float> getPrecharge() = 0;
  virtual std::optional<float> getMph() = 0;
  virtual std::optional<float> getKph() = 0;
  virtual std::optional<float> getStatus() = 0;
  virtual std::optional<float> getDir() = 0;
  virtual std::optional<float> getPackTemp() = 0;
  virtual std::optional<float> getMotorTemp() = 0;
  virtual std::optional<float> getStateOfCharge() = 0;
  virtual std::optional<float> getLvBattery() = 0;
  virtual std::optional<float> getCurrent() = 0;
  virtual std::optional<float> getBalancingCells() = 0;
  virtual std::optional<float> getPackVoltage() = 0;
  virtual std::optional<float> getMaxCellTemp() = 0;
  virtual std::optional<float> getMaxCellTempChipNumber() = 0;
  virtual std::optional<float> getMaxCellTempCellNumber() = 0;
  virtual std::optional<float> getMaxCellVoltage() = 0;
  virtual std::optional<float> getMaxCellVoltageChipNumber() = 0;
  virtual std::optional<float> getMaxCellVoltageCellNumber() = 0;
  virtual std::optional<float> getMinCellTemp() = 0;
  virtual std::optional<float> getMinCellTempChipNumber() = 0;
  virtual std::optional<float> getMinCellTempCellNumber() = 0;
  virtual std::optional<float> getMinCellVoltage() = 0;
  virtual std::optional<float> getMinCellVoltageChipNumber() = 0;
  virtual std::optional<float> getMinCellVoltageCellNumber() = 0;
  virtual std::optional<float> getAveCellTemp() = 0;
  virtual std::optional<float> getAveCellVoltage() = 0;
  virtual std::optional<float> getCellDelta() = 0;
  virtual std::optional<float> getBurningCells() = 0;
  virtual std::optional<float> getTractionControl() = 0;
  virtual std::optional<float> getInverterTemp() = 0;
  virtual std::optional<float> getMotorPower() = 0;
  virtual std::optional<float> getFanPower() = 0;
  virtual std::optional<float> getTorquePower() = 0;
  virtual std::optional<float> getRegenPower() = 0;
  virtual std::optional<float> getBmsState() = 0;
  virtual std::optional<float> getBmsFault() = 0;
  virtual std::optional<float> getMpuFault() = 0;
  virtual std::optional<float> getDcl() = 0;
  virtual std::optional<float> getCcl() = 0;
  virtual std::optional<float> getPackCurrent() = 0;
  virtual std::optional<std::string> getForwardButtonPressed() = 0;
  virtual std::optional<std::string> getBackwardButtonPressed() = 0;
  virtual std::optional<std::string> getDebugPressed() = 0;
  virtual std::optional<std::string> getRightButtonPressed() = 0;
  virtual std::optional<std::string> getEnterButtonPressed() = 0;
  virtual std::optional<std::string> getUpButtonPressed() = 0;
  virtual std::optional<std::string> getDownButtonPressed() = 0;
  virtual std::optional<float> getModeIndex() = 0;
  virtual std::optional<float> getGForceX() = 0;
  virtual std::optional<float> getGForceY() = 0;
  virtual std::optional<float> getGForceZ() = 0;
  virtual std::optional<float> getSegment1Temp() = 0;
  virtual std::optional<float> getSegment2Temp() = 0;
  virtual std::optional<float> getSegment3Temp() = 0;
  virtual std::optional<float> getSegment4Temp() = 0;
  virtual std::optional<float> getSdCardStatus() = 0;
  virtual std::optional<float> getVbat() = 0;
  virtual std::optional<float> getBmsPrefault() = 0;

  QList<DebugTableRowValue> getDebugTableValues();
  void updatePackTempData();
  void addPinnedData(int id);
  void removePinnedData(int id);
  void updatePinnedData();
  void updateAverageCellTemps();
  void updateStateOfChargeDeltas();
  std::optional<float> getById(int id);

  int pageHeight;
  int pageWidth;

protected:
  QMap<int, DebugPlotValue> pinnedData;
  QList<std::optional<float>> currentData;
  QList<int> packTempData;
  QList<FaultInstance> faultInstances;
  QList<int> averageCellTemps;
  QList<int> stateOfChargeDeltas;
  std::optional<float> prevSoc;
};

#endif // MODEL_H
