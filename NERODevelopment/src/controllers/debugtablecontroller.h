#ifndef DEBUGTABLECONTROLLER_H
#define DEBUGTABLECONTROLLER_H

#include "QtCore/qvariant.h"
#include "buttoncontroller.h"
#include "src/models/model.h"
#include <QObject>

class DebugTableController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(QList<QJsonObject> topics READ topics WRITE setTopics NOTIFY
                 topicsChanged FINAL)
  Q_PROPERTY(QList<QJsonObject> selectedValues READ selectedValues WRITE
                 setSelectedValues NOTIFY selectedValuesChanged FINAL)

public:
  explicit DebugTableController(Model *model, QObject *parent = nullptr);
  QList<QJsonObject> topics() const;
  QList<QJsonObject> selectedValues() const;

signals:
  void topicsChanged();
  void selectedValuesChanged();

public slots:
  void setTopics(QList<QJsonObject>);
  void setSelectedValues(QList<QJsonObject>);
  void update();

private:
  void downButtonPressed() override;
  void upButtonPressed() override;
  void leftButtonPressed() override;
  void rightButtonPressed() override;
  void enterButtonPressed() override;

  QList<QJsonObject> m_topics;
  QList<QJsonObject> m_selectedValues;
  int selectedTopicIndex = 0;
  bool scrollingTopics = true;
  int selectedValueIndex = 0;
};

#endif // DEBUGTABLECONTROLLER_H
