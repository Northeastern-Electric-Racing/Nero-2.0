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
  virtual std::optional<float> getTractionControl() = 0;
  virtual std::optional<float> getInverterTemp() = 0;
  virtual std::optional<float> getMotorPower() = 0;
  virtual std::optional<float> getFanPower() = 0;
  virtual std::optional<float> getTorquePower() = 0;
  virtual std::optional<float> getRegenPower() = 0;
  virtual std::optional<float> getBmsState() = 0;
  virtual QList<QString> getBmsFault() = 0;
  virtual QList<QString> getVcuFault() = 0;
  virtual std::optional<float> getDcl() = 0;
  virtual std::optional<float> getCcl() = 0;
  virtual std::optional<float> getPackCurrent() = 0;
  // Non-consuming primitives: true iff the current Wheel/Buttons/button_id
  // payload equals the given 0-indexed ordinal. See raspberry_model.h for
  // the layout and VCU counterpart.
  virtual bool buttonNum0Pressed() = 0;
  virtual bool buttonNum1Pressed() = 0;
  virtual bool buttonNum2Pressed() = 0;
  virtual bool buttonNum3Pressed() = 0;
  virtual bool buttonNum4Pressed() = 0;
  virtual bool buttonNum5Pressed() = 0;
  virtual bool buttonNum6Pressed() = 0;
  virtual bool buttonNum7Pressed() = 0;
  virtual bool buttonNum8Pressed() = 0;
  virtual bool buttonNum9Pressed() = 0;

  // Semantic wrappers, consume-on-read: return true once per press then clear
  // the latched value so ButtonController's fire-and-forget pattern keeps
  // working. Each wraps one primitive.
  virtual std::optional<bool> getEscButtonPressed() = 0;
  virtual std::optional<bool> getLeftButtonPressed() = 0;
  virtual std::optional<bool> getLaunchControlToggleButtonPressed() = 0;
  virtual std::optional<bool> getUpRegenButtonPressed() = 0;
  virtual std::optional<bool> getDownRegenButtonPressed() = 0;
  virtual std::optional<bool> getEnterButtonPressed() = 0;
  virtual std::optional<bool> getRightButtonPressed() = 0;
  virtual std::optional<bool> getTractionControlToggleButtonPressed() = 0;
  virtual std::optional<bool> getUpTorqueButtonPressed() = 0;
  virtual std::optional<bool> getDownTorqueButtonPressed() = 0;

  virtual std::optional<bool> getHomeButtonPressed() = 0;
  virtual std::optional<float> getModeIndex() = 0;
  virtual std::optional<float> getGForceX() = 0;
  virtual std::optional<float> getGForceY() = 0;
  virtual std::optional<float> getGForceZ() = 0;
  virtual std::optional<float> getSegment1Temp() = 0;
  virtual std::optional<float> getSegment2Temp() = 0;
  virtual std::optional<float> getSegment3Temp() = 0;
  virtual std::optional<float> getSegment4Temp() = 0;
  virtual QList<QString> getCriticalFaults() = 0;
  virtual QList<QString> getNonCriticalFaults() = 0;
  virtual void sendMessage(QString topic, float value) = 0;
  virtual void sendMessage(QString topic, QList<float> values) = 0;
  virtual std::optional<float> getLowVoltage() = 0;

  std::optional<int> getTime();
  std::optional<int> getFastestTime();
  std::optional<int> getLastTime();
  int getMaxSpeed();
  float getMaxDraw();

  QList<DebugTableRowValue> getDebugTableValues();
  void updatePackTempData();
  void addPinnedData(QString id);
  void removePinnedData(QString id);
  void updatePinnedData();
  QMap<QString, DebugPlotValue> getPinnedData();
  void updateAverageCellTemps();
  std::optional<float> getById(QString id, int valueIndex = 0);

  int pageHeight;
  int pageWidth;
  int currentPageIndex = -1;

signals:
  void onCurrentDataChange();

private slots:
  virtual void updateCurrentData() = 0;
  void updateStoredValues();

protected slots:
  void setValue(QString topic, float value);

protected:
  QMap<QString, DataInfo> currentData;
  QMap<QString, DebugPlotValue> pinnedData;
  QList<float> packTempData;
  QList<FaultInstance> faultInstances;
  QList<float> averageCellTemps;
  QList<float> stateOfChargeDeltas;
  int m_currentTime = 0;
  int m_fastestTime = 0;
  int m_lastTime = 0;
  int m_maxSpeed = 0;
  float m_maxDraw = 0;
};

#define ModelInterfaceId "com.ner.model"
Q_DECLARE_INTERFACE(Model, ModelInterfaceId);

#endif // MODEL_H
