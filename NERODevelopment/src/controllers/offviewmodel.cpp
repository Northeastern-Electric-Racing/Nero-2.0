#include "OffViewModel.h"

OffViewModel::OffViewModel(QObject *parent) : QObject(parent)
{
    for (const auto& attributeName : {SIDEBRBS, BMS, BSPD, MPU, BOTS, INERTIA, CPBRB, TSMS, HVDINTRLK, HVCNCTR}) {
        m_attributeStatusMap[attributeName] = AttributeStatus::OFF;
    }
}

AttributeStatus OffViewModel::getAttributeStatus(AttributeName attributeName) const
{
    return m_attributeStatusMap.value(attributeName, AttributeStatus::UNCHECKED);
}
