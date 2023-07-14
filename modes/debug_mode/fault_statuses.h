#ifndef FAULT_STATUSES_H
#define FAULT_STATUSES_H

#include <vector>
#include <QString>

const std::vector<QString> FAULTS = {
    "Cells_Not_Balancing",
    "Cell_Voltage_Too_High",
    "Cell_Voltage_Too_Low",
    "Pack_Too_High",
    "Open_Wiring_Fault",
    "Internal_Software_Fault",
    "Internal_Thermal_Fault",
    "Internal_Cell_Comm_Fault",
    "Current_Sensor_Fault",
    "Charge_Reading_Mismatch",
    "Low_Cell_Voltage",
    "Weak_Pack_Fault",
    "External_Can_Fault",
    "Discharge_Limit_Enforcement_Fault",
    "Charger_Safety_Relay",
    "Battery_Can_Fault",
    "Charger_Can_Fault",
    "Charge_Limit_Enforcement_Fault"
};

#endif // FAULT_STATUSES_H
