#ifndef OFFVIEWCONTROLLER_H
#define OFFVIEWCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "attributenames.h"
#include "attributestatus.h"
#include "offviewmodel.h"
#include "chargingcontroller.h"
#include "efficiencycontroller.h"

class OffViewController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QMap<QString, AttributeStatus> attributeStatus READ getAttributeStatus NOTIFY attributeStatusChanged)
    Q_PROPERTY(
        int packTemp READ packTemp WRITE setPackTemp NOTIFY packTempChanged FINAL)          //QProperty for pack temp
    Q_PROPERTY(qreal motorTemp READ motorTemp WRITE setMotorTemp NOTIFY                     //QProperty for motor temp
                   motorTempChanged FINAL)
    Q_PROPERTY(int stateOfChargeDelta READ stateOfChargeDelta WRITE
                   setStateOfChargeDelta NOTIFY stateOfChargeDeltaChanged FINAL)            //QProperty for state of charge

public:
    explicit OffViewController(OffViewModel *viewModel, QObject *parent = nullptr);

    QMap<QString, AttributeStatus> getAttributeStatus() const;
    void updateAttributesFromModel();

signals:
    void attributeStatusChanged(const QMap<QString, AttributeStatus> &attributeStatus);

public slots:
    void setAttributeStatus(const QString &name, AttributeStatus status);

private:
    QMap<QString, AttributeStatus> m_attributeStatus;
};

#endif // OFFVIEWCONTROLLER_H
