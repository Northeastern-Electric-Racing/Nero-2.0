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

public:
  explicit DebugGraphController(Model *model, QObject *parent = nullptr);
  ~DebugGraphController();
  QList<QJsonObject> graphData() const;

signals:
  void graphDataChanged();

public slots:
  void setGraphData(QList<QJsonObject>);

private:
  QList<QJsonObject> m_graphData;
  QTimer m_dataUpdateTimer;
};

#endif // DEBUGGRAPHCONTROLLER_H
