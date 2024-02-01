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
    const auto attributeStatusMap = m_model->getAttributeStatus();
    for (const auto& attributeName : {SIDEBRBS, BMS, BSPD, MPU, BOTS, INERTIA, CPBRB, TSMS, HVDINTRLK, HVCNCTR}) {
        AttributeStatus status = attributeStatusMap.value(attributeName);
        setAttributeStatus(attributeName, status);
    }
    setPackTemp(*m_model->getPackTemp());
    setMotorTemp(*m_model->getMotorTemp());
    setStateOfCharge(*m_model->getStateOfCharge());
}

float OffViewController::packTemp() const { return m_packTemp; }
void OffViewController::setPackTemp(float degrees) {
    if (degrees != m_packTemp) {
        m_packTemp = degrees;
        emit packTempChanged(degrees);
    }
}

float OffViewController::motorTemp() const { return m_motorTemp; }
void OffViewController::setMotorTemp(float temp) {
    if (temp != m_motorTemp) {
        m_motorTemp = temp;
        emit motorTempChanged(temp);
    }
}

float OffViewController::stateOfCharge() const { return m_stateOfCharge; }
void OffViewController::setStateOfCharge(float charge) {
    if (charge != m_stateOfCharge) {
        m_stateOfCharge = charge;
        emit stateOfChargeChanged(charge);
    }
}
