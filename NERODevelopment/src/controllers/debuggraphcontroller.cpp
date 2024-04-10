#include "debuggraphcontroller.h"

DebugGraphController::DebugGraphController(Model *model, QObject *parent)
    : ButtonController(model, 3, parent) {
  connect(model, &Model::onCurrentDataChange, this,
          &DebugGraphController::update);
}

QList<QJsonObject> DebugGraphController::graphData() const {
  return this->m_graphData;
}

QString DebugGraphController::unit() const { return this->m_unit; }

QString DebugGraphController::title() const { return this->m_title; }

int DebugGraphController::maxY() const { return this->m_maxY; }

int DebugGraphController::minY() const { return this->m_minY; }

int DebugGraphController::getNumPoints() { return this->m_num_points; }

void DebugGraphController::setGraphData(QList<QJsonObject> json) {
  if (this->m_graphData != json) {
    this->m_graphData = json;
    emit this->graphDataChanged();
  }
}

void DebugGraphController::setUnit(QString unit) { this->m_unit = unit; }

void DebugGraphController::setTitle(QString title) {
  if (m_title == title) {
    this->m_model->removePinnedData(title);
    return;
  }
  this->m_title = title;
  this->m_model->addPinnedData(title);
  emit this->titleChanged();
}

void DebugGraphController::setMaxY(int max) {
  if (this->m_maxY != max) {
    this->m_maxY = max;
    emit this->maxYChanged();
  }
}

void DebugGraphController::setMinY(int min) {
  if (this->m_minY != min) {
    this->m_minY = min;
    emit this->minYChanged();
  }
}

void DebugGraphController::setValues(QString title, QString unit) {
  setUnit(unit);
  setTitle(title);
}

void DebugGraphController::update() {
  if (this->m_model->currentPageIndex != this->m_pageIndex)
    return;

  if (QDateTime::currentMSecsSinceEpoch() - this->m_last_refresh <
      this->m_refresh_rate) {
    return;
  }
  this->m_last_refresh = QDateTime::currentMSecsSinceEpoch();

  this->m_model->updatePinnedData();

  QList<QJsonObject> allPoints;
  QMap<QString, DebugPlotValue> pinnedData = this->m_model->getPinnedData();

  int globalIndex = this->m_num_points - 1;

  const QList<float> &dataList = pinnedData[this->m_title].data;

  for (int i = 0; i < std::min(this->m_num_points, (int)dataList.length());
       ++i) {
    float value = dataList.at(i);
    if (!std::isnan(value) && std::isfinite(value)) {
      QJsonObject point;
      point["x"] = globalIndex--;
      point["y"] = value;
      allPoints.append(point);
    }
  }

  int maxY = -99999;
  int minY = 99999;

  for (const QJsonObject &point : allPoints) {
    int yValue = point["y"].toInt();
    if (yValue > maxY) {
      maxY = yValue;
    }
    if (yValue < minY) {
      minY = yValue;
    }
  }

  setMaxY(maxY + 5);
  setMinY(minY - 5);

  setGraphData(allPoints);
}
