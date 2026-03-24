#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <QString>
#include <QVector>

struct DataInfo {
  QString topic;
  QString unit;
  QVector<float> values;

  DataInfo();
  DataInfo(const QString topic, const QString unit,
           const QVector<float> values);
};

#endif // SERVER_DATA_H
