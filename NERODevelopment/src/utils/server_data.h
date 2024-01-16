#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <QString>
#include <QJsonObject>

struct ServerData {
private:
   void fromJson(const QJsonObject &json);

public:
    QString unit;
    float value;

    ServerData(const QJsonObject &json);
};

struct DataInfo {
   QString topic;
   QString unit;
   float value;

   DataInfo();
   DataInfo(const QString &topic, const QString &unit, const float value);
};

#endif // SERVER_DATA_H
