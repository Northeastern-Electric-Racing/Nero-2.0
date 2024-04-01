#include "debuggraphcontroller.h"

DebugGraphController::DebugGraphController(Model *model, QObject *parent)
    : ButtonController(model, 7, parent) {}

DebugGraphController::~DebugGraphController() { m_dataUpdateTimer.stop(); }

QList<QJsonObject> DebugGraphController::graphData() const {
  return m_graphData;
}

void DebugGraphController::setGraphData(QList<QJsonObject> json) {
  if (this->m_graphData != json) {
    this->m_graphData = json;
  }
}
