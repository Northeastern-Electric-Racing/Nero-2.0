#include "debuggraphcontroller.h"

DebugGraphController::DebugGraphController(Model *model, QObject *parent)
    : ButtonController(model, parent) {
  connect(&m_dataUpdateTimer, &QTimer::timeout, this,
          &DebugGraphController::updateGraphData);
  m_dataUpdateTimer.start(1000);
}

DebugGraphController::~DebugGraphController() { m_dataUpdateTimer.stop(); }

QList<QVariant> DebugGraphController::graphData() const { return m_graphData; }

void DebugGraphController::updateGraphData() {
  QList<QVariant> newData;
  auto pinnedDataMap = m_model->getPinnedData();

  for (const auto &key : pinnedDataMap.keys()) {
    const auto &value = pinnedDataMap.value(key);
    newData.append(QVariant::fromValue(value));
  }

  m_graphData = newData;

  emit graphDataChanged();
}
