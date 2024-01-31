#include "offviewcontroller.h"

OffViewController::OffViewController(Model *model, QObject *parent)
    : QObject{parent}, m_packTemp(0.0), m_motorTemp(0.0), m_stateOfCharge(0.0) {
    this->m_model = model;
    connect(m_model, &Model::updateAttributesFromModel, this,
            &OffViewController::updateAttributesFromModel);
}

QMap<QString, AttributeStatus> OffViewController::getAttributeStatus() const { return m_attributeStatus; }
void OffViewController::setAttributeStatus(const QString &name, AttributeStatus status)
{
    if (m_attributeStatus.value(name) != status)
    {
        m_attributeStatus[name] = status;
        emit attributeStatusChanged(m_attributeStatus);
    }
}

void OffViewController::updateAttributesFromModel() {
    for (const auto& attributeName : {SIDEBRBS, BMS, BSPD, MPU, BOTS, INERTIA, CPBRB, TSMS, HVDINTRLK, HVCNCTR}) {
        setAttributeStatus(attributeName, m_attributeStatus[attributeName]);
    }
}

int OffViewController::packTemp() const { return m_packTemp; }
void OffViewController::setPackTemp(int degrees) {
    if (degrees != m_packTemp) {
        m_packTemp = degrees;
        emit packTempChanged(degrees);
    }
}

qreal OffViewController::motorTemp() const { return m_motorTemp; }
void OffViewController::setMotorTemp(qreal temp) {
    if (temp != m_motorTemp) {
        m_motorTemp = temp;
        emit motorTempChanged(temp);
    }
}

int OffViewController::stateOfCharge() const { return m_stateOfCharge; }
void OffViewController::setStateOfCharge(int charge) {
    if (charge != m_stateOfCharge) {
        m_stateOfCharge = charge;
        emit stateOfChargeChanged(charge);
    }
}
