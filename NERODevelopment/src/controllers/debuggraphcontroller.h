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

public:
  explicit DebugGraphController(Model *model, QObject *parent = nullptr);
  ~DebugGraphController();
  QList<QJsonObject> graphData() const;
  QString unit() const;
  QString title() const;

signals:
  void graphDataChanged();
  void unitChanged();
  void titleChanged();

public slots:
  void setGraphData(QList<QJsonObject>);
  void setUnit(QString);
  void setTitle(QString);
  void setValues(QString, QString);
  void update();

private:
  QList<QJsonObject> m_graphData;
  QString m_unit = "TEMPERATURE (C)";
  QString m_title = "AVERAGE CELL TEMPERATURE";
  QTimer m_dataUpdateTimer;
};

#endif // DEBUGGRAPHCONTROLLER_H
