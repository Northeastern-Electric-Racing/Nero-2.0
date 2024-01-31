#ifndef OFFVIEWCONTROLLER_H
#define OFFVIEWCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "attributenames.h"
#include "attributestatus.h"
#include "../models/model.h"

class OffViewController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QMap<QString, AttributeStatus> attributeStatus READ getAttributeStatus NOTIFY attributeStatusChanged)
    Q_PROPERTY(
        float packTemp READ packTemp WRITE setPackTemp NOTIFY packTempChanged FINAL)
    Q_PROPERTY(float motorTemp READ motorTemp WRITE setMotorTemp NOTIFY
                   motorTempChanged FINAL)
    Q_PROPERTY(float stateOfCharge READ stateOfCharge WRITE setStateOfCharge NOTIFY
                   stateOfChargeChanged FINAL)

public:
    explicit OffViewController(Model *model, QObject *parent = nullptr);

    QMap<QString, AttributeStatus> getAttributeStatus() const;
    float packTemp() const;
    float motorTemp() const;
    float stateOfCharge() const;
    void updateAttributesFromModel();

signals:
    void attributeStatusChanged(const QMap<QString, AttributeStatus> &attributeStatus);
    void packTempChanged(float);
    void motorTempChanged(float);
    void stateOfChargeChanged(float);

public slots:
    void setAttributeStatus(const QString &name, AttributeStatus status);
    void setPackTemp(float);
    void setMotorTemp(float);
    void setStateOfCharge(float);

private:
    Model *m_model;
    QMap<QString, AttributeStatus> m_attributeStatus;
    float m_packTemp;
    float m_motorTemp;
    float m_stateOfCharge;
};

#endif // OFFVIEWCONTROLLER_H
