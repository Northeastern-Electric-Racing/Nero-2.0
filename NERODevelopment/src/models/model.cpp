#include "model.h"
#include "QVector"

Model::Model() : pageHeight(480), pageWidth(800) {
  connect(this, &Model::onCurrentDataChange, this, &Model::updateStoredValues);
}

void Model::updateStoredValues() {
  std::optional<float> mph = this->getMph();
  if (mph) {
    if (mph.value() > m_maxSpeed) {
      m_maxSpeed = mph.value();
    }
  }
  std::optional<int> time = this->getTime();
  if (time) {
    m_lastTime = time.value();
    if (time.value() < m_fastestTime || m_fastestTime == 0) {
      m_fastestTime = time.value();
    }
  }
  std::optional<float> draw = this->getCurrent();
  if (draw) {
    if (draw.value() > m_maxDraw) {
      m_maxDraw = draw.value();
    }
  }
}

void Model::updatePackTempData() {
  if (packTempData.size() >= 600) {
    packTempData.pop_back();
  }
  if (getPackTemp()) {
    packTempData.prepend(*getPackTemp());
  }
}

void Model::addPinnedData(QString id) {
  bool success;
  DataInfo dataInfo = this->currentData.value(id, DataInfo());
  float value = dataInfo.values[0];

  if (success && value != -9999) {
    pinnedData.insert(
        id, DebugPlotValue(dataInfo.topic, dataInfo.unit, QList<float>(value)));
  }
}

void Model::removePinnedData(QString id) { pinnedData.remove(id); }

void Model::updatePinnedData() {
  for (QString id : pinnedData.keys()) {
    if (pinnedData.find(id).value().data.size() >= 600) {
      pinnedData.find(id).value().data.pop_back();
    }
    pinnedData.find(id).value().data.prepend(*this->getById(id));
  }
}

QMap<QString, DebugPlotValue> Model::getPinnedData() {
  return this->pinnedData;
}

void Model::updateAverageCellTemps() {
  if (averageCellTemps.size() >= 30) {
    averageCellTemps.pop_front();
  }
  averageCellTemps.append(getAveCellTemp() ? *getAveCellTemp() : 0);
}

std::optional<int> Model::getTime() { return m_currentTime; }

std::optional<int> Model::getLastTime() { return m_lastTime; }

std::optional<int> Model::getFastestTime() { return m_fastestTime; }

int Model::getMaxSpeed() { return m_maxSpeed; }

float Model::getMaxDraw() { return m_maxDraw; }

QList<DebugTableRowValue> Model::getDebugTableValues() {
  QList<DebugTableRowValue> table = {};
  for (auto it = this->currentData.begin(); it != this->currentData.end();
       ++it) {
    const DataInfo &dataInfo = it.value();
    QString name = dataInfo.topic;
    QString units = dataInfo.unit;
    float value = dataInfo.values[0];

    DebugTableRowValue row = {name, (std::round(value)), units};

    table.append(row);
  }

  return table;
}

std::optional<float> Model::getById(QString id) {
    auto it = currentData.find(id);
    if (it != currentData.end()) {
        if (it->values.isEmpty()) {
            qWarning() << "Empty values for topic:" << id;
            return std::nullopt;
        }
        return it->values[0];
    }
    return std::nullopt;
}

void Model::setValue(QString topic, float value) {
  this->currentData[topic] = DataInfo(topic, "", {value});
  // emit this->onCurrentDataChange();
}
