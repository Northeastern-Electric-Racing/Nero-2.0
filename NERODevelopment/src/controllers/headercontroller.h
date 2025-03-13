#ifndef HEADERCONTROLLER_H
#define HEADERCONTROLLER_H

#include "../models/model.h"
#include <QObject>

/**
 * @brief The HeaderController class
 * Controller for the views.
 */
class HeaderController : public QObject {
  Q_OBJECT
  Q_PROPERTY(QList<QString> criticalFaults READ criticalFaults WRITE
                 setCriticalFaults NOTIFY criticalFaultsChanged FINAL)
  Q_PROPERTY(QList<QString> nonCriticalFaults READ nonCriticalFaults WRITE
                 setNonCriticalFaults NOTIFY nonCriticalFaultsChanged FINAL)
  Q_PROPERTY(bool isTalking READ isTalking WRITE setIsTalking NOTIFY
                 isTalkingChanged FINAL)

public:
  explicit HeaderController(Model *model, QObject *parent = nullptr);
  QList<QString> criticalFaults() const;
  QList<QString> nonCriticalFaults() const;
  bool isTalking() const;

signals:
  void criticalFaultsChanged(QList<QString>);
  void nonCriticalFaultsChanged(QList<QString>);
  void isTalkingChanged(bool);

public slots:
  void setCriticalFaults(QList<QString>);
  void setNonCriticalFaults(QList<QString>);
  void setIsTalking(bool);
  void currentDataDidChange();

private:
  Model *m_model;
  QList<QString> m_criticalFaults;
  QList<QString> m_nonCriticalFaults;
  bool m_isTalking = false;
};

#endif // HEADERCONTROLLER_H
