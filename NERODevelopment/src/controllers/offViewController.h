#ifndef OFFVIEWCONTROLLER_H
#define OFFVIEWCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "attributeNames.h"
#include "attributeStatus.h"
#include "offviewmodel.h"
#include "chargingcontroller.h"
#include "efficiencycontroller.h"

class OffViewController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QMap<QString, AttributeStatus> attributeStatus READ getAttributeStatus NOTIFY attributeStatusChanged)

public:
    explicit OffViewController(OffViewModel *viewModel, QObject *parent = nullptr);

    QMap<QString, AttributeStatus> getAttributeStatus() const;
    void updateAttributesFromModel();

signals:
    void attributeStatusChanged(const QMap<QString, AttributeStatus> &attributeStatus);

public slots:
    void setAttributeStatus(const QString &name, AttributeStatus status);

private:
    OffViewModel *m_viewModel;
    ChargingController *m_chargingInfo;               //pointer to access packTemp and stateOfCharge
    EfficiencyController *m_efficiencyInfo;           //pointer to access motorTemp
    QMap<QString, AttributeStatus> m_attributeStatus;
};

#endif // OFFVIEWCONTROLLER_H
