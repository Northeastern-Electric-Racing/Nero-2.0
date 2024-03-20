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
  Q_PROPERTY(int selectedTopicsIndex READ selectedTopicsIndex WRITE
                 setSelectedTopicsIndex NOTIFY selectedTopicsIndexChanged FINAL)
  Q_PROPERTY(int selectedValuesIndex READ selectedValuesIndex WRITE
                 setSelectedValuesIndex NOTIFY selectedValuesIndexChanged FINAL)
  Q_PROPERTY(bool scrollingTopics READ scrollingTopics WRITE setScrollingTopics
                 NOTIFY scrollingTopicsChanged FINAL)

public:
  explicit DebugTableController(Model *model, QObject *parent = nullptr);
  QList<QJsonObject> topics() const;
  QList<QJsonObject> selectedValues() const;
  int selectedValuesIndex() const;
  int selectedTopicsIndex() const;
  bool scrollingTopics() const;

signals:
  void topicsChanged();
  void selectedValuesChanged();
  void selectedValuesIndexChanged();
  void selectedTopicsIndexChanged();
  void scrollingTopicsChanged();

public slots:
  void setTopics(QList<QJsonObject>);
  void setSelectedValues(QList<QJsonObject>);
  void setSelectedValuesIndex(int);
  void setSelectedTopicsIndex(int);
  void setScrollingTopics(bool);

  void update();

  void downButtonPressed() override;
  void upButtonPressed() override;
  void leftButtonPressed() override;
  void rightButtonPressed() override;
  void enterButtonPressed() override;

private:
  QList<QJsonObject> m_topics;
  QList<QJsonObject> m_selectedValues;
  int m_selectedTopicsIndex = 0;
  bool m_scrollingTopics = true;
  int m_selectedValuesIndex = 0;
  int m_refresh_rate = 500;
  int m_last_refresh = 0;
};

#endif // DEBUGTABLECONTROLLER_H
