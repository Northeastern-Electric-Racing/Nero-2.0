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
  Q_PROPERTY(bool faultAlertsEnabled READ faultAlertsEnabled NOTIFY
                 faultAlertsEnabledChanged FINAL)

public:
  explicit HeaderController(Model *model, QObject *parent = nullptr);
  QList<QString> criticalFaults() const;
  QList<QString> nonCriticalFaults() const;
  bool faultAlertsEnabled() const;

signals:
  void criticalFaultsChanged(QList<QString>);
  void nonCriticalFaultsChanged(QList<QString>);
  void faultAlertsEnabledChanged(bool);

public slots:
  void setCriticalFaults(QList<QString>);
  void setNonCriticalFaults(QList<QString>);
  void currentDataDidChange();
  void toggleFaultAlerts();

private:
  Model *m_model;
  QList<QString> m_criticalFaults;
  QList<QString> m_nonCriticalFaults;
  bool m_faultAlertsEnabled = true;
};

#endif // HEADERCONTROLLER_H
