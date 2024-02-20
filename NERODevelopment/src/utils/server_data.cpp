#include "../utils/server_data.h"

// ServerData::ServerData(const QJsonObject &json) { fromJson(json); }

// void ServerData::fromJson(const QJsonObject &json) {
//   this->unit = json["unit"].toString();
//   this->value = json["value"].toDouble();
// }

DataInfo::DataInfo(const QString topic, const QString unit,
                   const QVector<QString> values) {
  this->topic = topic;
  this->unit = unit;
  this->values = values;
}

DataInfo::DataInfo() {
  this->topic = nullptr;
  this->unit = nullptr;
  this->values = {"-9999"};
}
