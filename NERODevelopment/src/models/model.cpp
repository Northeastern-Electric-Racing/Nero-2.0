#include "model.h"

Model::Model() : prevSoc(0), pageHeight(480), pageWidth(800) {}

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

  pinnedData.insert(
      id, DebugPlotValue(value.topic, value.unit, QList<float>(value.value)));
}

void Model::removePinnedData(QString id) { pinnedData.remove(id); }

void Model::updatePinnedData() {
  for (QString id : pinnedData.keys()) {
    if (pinnedData.find(id).value().data.size() >= 600) {
      pinnedData.find(id).value().data.pop_back();
    }
    pinnedData.find(id).value().data.prepend(this->currentData[id].value);
  }
}

void Model::updateAverageCellTemps() {
  if (averageCellTemps.size() >= 30) {
    averageCellTemps.pop_front();
  }
  averageCellTemps.append(getAveCellTemp() ? *getAveCellTemp() : 0);
}

void Model::updateStateOfChargeDeltas() {
  if (stateOfChargeDeltas.size() >= 30) {
    stateOfChargeDeltas.pop_front();
  }
  std::optional<float> soc = this->getStateOfCharge();
  if (soc) {
    stateOfChargeDeltas.append(*soc - prevSoc);
    prevSoc = *soc;
  }
}

QList<DebugTableRowValue> Model::getDebugTableValues() {
  QList<DebugTableRowValue> table;
  for (DataInfo dataInfo : this->currentData.values()) {
    QString name = dataInfo.topic;
    QString units = dataInfo.unit;
    float value = dataInfo.value;
    DebugTableRowValue row = {name, (std::round(value * 1000) / 1000), units};

    table.emplace_back(row);
  }
  return table;
}

std::optional<float> Model::getById(QString id) {
  return this->currentData[id].value;
}
