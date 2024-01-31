#include "offviewcontroller.h"

OffViewController::OffViewController(OffViewModel *viewModel, QObject *parent)
    : QObject(parent), m_viewModel(viewModel)
{
    updateAttributesFromModel();
}

QMap<QString, AttributeStatus> OffViewController::getAttributeStatus() const
{
    return m_attributeStatus;
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
        setAttributeStatus(attributeName, *m_model->getAttributeStatus());
    }
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
