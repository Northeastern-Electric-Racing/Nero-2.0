#include "model.h"
#include "QVector"

Model::Model() : pageHeight(480), pageWidth(800) {}

void Model::updatePackTempData() {
  if (packTempData.size() >= 600) {
    packTempData.pop_back();
  }
  if (getPackTemp()) {
    packTempData.prepend(*getPackTemp());
  }
}

void Model::addPinnedData(QString id) {
  DataInfo value = this->currentData[id];
  bool success;
  float parsedValue = value.values[0].toFloat(&success);

  if (success) {
    pinnedData.insert(
        id, DebugPlotValue(value.topic, value.unit, QList<float>(parsedValue)));
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

void Model::updateAverageCellTemps() {
  if (averageCellTemps.size() >= 30) {
    averageCellTemps.pop_front();
  }
  averageCellTemps.append(getAveCellTemp() ? *getAveCellTemp() : 0);
}

QList<DebugTableRowValue> Model::getDebugTableValues() {
  QList<DebugTableRowValue> table = {};
  for (auto it = this->currentData.begin(); it != this->currentData.end();
       ++it) {
    const DataInfo &dataInfo = it.value();
    QString name = dataInfo.topic;
    QString units = dataInfo.unit;
    float value = dataInfo.values[0].toFloat();

    DebugTableRowValue row = {name, (std::round(value)), units};

    table.append(row);
  }

  return table;
}

std::optional<float> Model::getById(QString id) {
  bool ok;
  float value = this->currentData.value(id, DataInfo()).values[0].toFloat(&ok);
  if (ok && value != -9999) {
    return value;
  } else {
    return std::nullopt;
  }
}
