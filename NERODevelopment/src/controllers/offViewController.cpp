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

void OffViewController::setAttributeStatus(const QString &name, AttributeStatus status)
{
    if (m_attributeStatus.value(name) != status)
    {
        m_attributeStatus[name] = status;
        emit attributeStatusChanged(m_attributeStatus);
    }
}

QMap<QString, AttributeStatus> OffViewController::getAttributeStatus() const {
    return m_attributeStatus;
}

void OffViewController::updateAttributesFromModel()
{
    for (const auto& attributeName : {SIDEBRBS, BMS, BSPD, MPU, BOTS, INERTIA, CPBRB, TSMS, HVDINTRLK, HVCNCTR}) {
        setAttributeStatus(attributeName, m_viewModel->getAttributeStatus(attributeName));
    }
}
