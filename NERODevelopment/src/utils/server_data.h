#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <QJsonObject>
#include <QString>

// struct ServerData {
// private:
//    void fromJson(const QJsonObject &json);

// public:
//     QString unit;
//     float value;

//     ServerData(const QJsonObject &json);
// };

struct DataInfo {
  QString topic;
  QString unit;
  QVector<QString> values;

  DataInfo();
  DataInfo(const QString topic, const QString unit,
           const QVector<QString> values);
};

#endif // SERVER_DATA_H
