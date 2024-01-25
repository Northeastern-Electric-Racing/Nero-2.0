#include "signalscontroler.h"

signalsController::signalsController(QObject *parent)
{
    // Inital mockup of statuses, Strings represent
    // the name and status of each attribute
    attributeStatus["SideBRBs"] = "unchecked";
    attributeStatus["BMS"] = "unchecked";
    attributeStatus["IMD"] = "unchecked";
    attributeStatus["BSPD"] = "unchecked";
    attributeStatus["MPU"] = "unchecked";
    attributeStatus["BOTS"] = "unchecked";
    attributeStatus["INERTIA"] = "unchecked";
    attributeStatus["CP BRB"] = "unchecked";
    attributeStatus["TSMS"] = "unchecked";
    attributeStatus["HVD INTRLK"] = "unchecked";
    attributeStatus["HV CNCTR"] = "unchecked";

}

QMap<QString, QString> SignalsController::getAttributeStatus(const QString &attribute) const
{
    return attributeStatus.value(attribute);
}

void SignalsController::setAttributeStatus(const QString &attribute, const QString &status)
{
    if (attributeStatus.value(attribute) != status)
    {
        attributeStatus[attribute] = status;
        emit attributeStatusChanged(attributeStatus);
    }
}
