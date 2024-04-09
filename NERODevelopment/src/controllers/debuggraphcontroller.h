#ifndef DEBUGGRAPHCONTROLLER_H
#define DEBUGGRAPHCONTROLLER_H

#include "QtCore/qvariant.h"
#include "buttoncontroller.h"
#include "src/models/model.h"
#include <QObject>
#include <QTimer>

class DebugGraphController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(QList<QJsonObject> graphData READ graphData WRITE setGraphData
                 NOTIFY graphDataChanged)
  Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(int maxY READ maxY WRITE setMaxY NOTIFY maxYChanged)
  Q_PROPERTY(int minY READ minY WRITE setMinY NOTIFY minYChanged)

public:
  explicit DebugGraphController(Model *model, QObject *parent = nullptr);
  QList<QJsonObject> graphData() const;
  QString unit() const;
  QString title() const;
  int maxY() const;
  int minY() const;
  int getNumPoints();

signals:
  void graphDataChanged();
  void unitChanged();
  void titleChanged();
  void maxYChanged();
  void minYChanged();

public slots:
  void setGraphData(QList<QJsonObject>);
  void setUnit(QString);
  void setTitle(QString);
  void setMaxY(int);
  void setMinY(int);
  void setValues(QString, QString);
  void update();

private:
  QList<QJsonObject> m_graphData = {};
  QString m_unit = "TEMPERATURE (C)";
  QString m_title = "AVERAGE CELL TEMPERATURE";
  int m_maxY = 60;
  int m_minY = 0;
  qint64 m_last_refresh = 0;
  int m_refresh_rate = 500;
  int m_num_points = 21;
};

#endif // DEBUGGRAPHCONTROLLER_H
