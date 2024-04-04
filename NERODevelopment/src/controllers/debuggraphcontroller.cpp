#include "debuggraphcontroller.h"

DebugGraphController::DebugGraphController(Model *model, QObject *parent)
    : ButtonController(model, 7, parent) {}

DebugGraphController::~DebugGraphController() { m_dataUpdateTimer.stop(); }

QList<QJsonObject> DebugGraphController::graphData() const {
  return this->m_graphData;
}

QString DebugGraphController::unit() const { return this->m_unit; }

QString DebugGraphController::title() const { return this->m_title; }

void DebugGraphController::setGraphData(QList<QJsonObject> json) {
  if (this->m_graphData != json) {
    this->m_graphData = json;
  }
}

void DebugGraphController::setUnit(QString unit) { this->m_unit = unit; }

void DebugGraphController::setTitle(QString title) {
  this->m_title = title;
  this->m_model->addPinnedData(title);
}

void DebugGraphController::setValues(QString title, QString unit) {
  setUnit(unit);
  setTitle(title);
}

void DebugGraphController::update() {
  this->m_model->updatePinnedData();

  // setGraphData(pinnedData.map qlist of qjson (x, y))
}
