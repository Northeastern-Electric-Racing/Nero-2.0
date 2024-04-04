#ifndef MODEL_H
#define MODEL_H

#include "../modes/debug_mode/debug_utils.h"
#include "../utils/server_data.h"
#include <QList>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QVariant>

/**
 * @brief The Model class
 * Abstract Class For The mock model and raspberry pi model
 * Unimplementable
 */
class Model : public QObject {
  Q_OBJECT

public:
  explicit Model();
  virtual ~Model() {}

  virtual void connectToMQTT() = 0;
  virtual std::optional<float> getMph() = 0;
  virtual std::optional<float> getKph() = 0;
  virtual std::optional<float> getStatus() = 0;
  virtual std::optional<float> getDir() = 0;
  virtual std::optional<float> getPackTemp() = 0;
  virtual std::optional<float> getMotorTemp() = 0;
  virtual std::optional<float> getStateOfCharge() = 0;
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
  virtual std::optional<bool> getForwardButtonPressed() = 0;
  virtual std::optional<bool> getBackwardButtonPressed() = 0;
  virtual std::optional<bool> getRightButtonPressed() = 0;
  virtual std::optional<bool> getEnterButtonPressed() = 0;
  virtual std::optional<bool> getUpButtonPressed() = 0;
  virtual std::optional<bool> getDownButtonPressed() = 0;
  virtual std::optional<bool> getHomeButtonPressed() = 0;
  virtual std::optional<float> getModeIndex() = 0;
  virtual std::optional<float> getGForceX() = 0;
  virtual std::optional<float> getGForceY() = 0;
  virtual std::optional<float> getGForceZ() = 0;
  virtual std::optional<float> getSegment1Temp() = 0;
  virtual std::optional<float> getSegment2Temp() = 0;
  virtual std::optional<float> getSegment3Temp() = 0;
  virtual std::optional<float> getSegment4Temp() = 0;
  virtual std::optional<bool> getIsTalking() = 0;
  virtual std::optional<int> getNumberOfCriticalFaults() = 0;
  virtual std::optional<int> getNumberOfNonCriticalFaults() = 0;
  virtual std::optional<int> getTime() = 0;
  virtual void sendMessage(QString topic, QString message) = 0;



  int getFastestTime();
  int getLastTime();
  int getMaxSpeed();

  QList<DebugTableRowValue> getDebugTableValues();
  void updatePackTempData();
  void addPinnedData(QString id);
  void removePinnedData(QString id);
  void updatePinnedData();
  QMap<QString, DebugPlotValue> getPinnedData();
  void updateAverageCellTemps();
  std::optional<float> getById(QString id);

  int pageHeight;
  int pageWidth;
  int currentPageIndex = -1;

signals:
  void onCurrentDataChange();

private slots:
  virtual void updateCurrentData() = 0;

protected:
  QMap<QString, DataInfo> currentData;
  QMap<QString, DebugPlotValue> pinnedData;
  QList<float> packTempData;
  QList<FaultInstance> faultInstances;
  QList<float> averageCellTemps;
  QList<float> stateOfChargeDeltas;
  int m_fastestTime;
  int m_lastTime;
  int m_maxSpeed;

};

#define ModelInterfaceId "com.ner.model"
Q_DECLARE_INTERFACE(Model, ModelInterfaceId);

#endif // MODEL_H
