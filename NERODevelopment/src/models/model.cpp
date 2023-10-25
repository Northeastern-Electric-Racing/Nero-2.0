#include "model.h"

Model::Model() : prevSoc(0), pageHeight(480), pageWidth(800) {
  // Constructor implementation
}

void Model::updatePackTempData() {
  if (packTempData.size() >= 600) {
    packTempData.pop_back();
  }
  if (getPackTemp()) {
    packTempData.prepend(*getPackTemp());
  }
}

void Model::addPinnedData(int id) {
  pinnedData.insert(id,
                    DebugPlotValue(DATA_IDS.find(id)->second.name,
                                   DATA_IDS.find(id)->second.units,
                                   {(currentData[id] ? *currentData[id] : 0)}));
}

void Model::removePinnedData(int id) { pinnedData.remove(id); }

void Model::updatePinnedData() {
  for (int id : pinnedData.keys()) {
    if (pinnedData.find(id).value().data.size() >= 600) {
      pinnedData.find(id).value().data.pop_back();
    }
    if (currentData[id])
      pinnedData.find(id).value().data.prepend(*currentData[id]);
  }
}

void Model::updateAverageCellTemps() {
  if (averageCellTemps.size() >= 30) {
    averageCellTemps.pop_front();
  }
  averageCellTemps.append(getAveCellTemp() ? *getAveCellTemp() : 0);
}

void Model::updateStateOfChargeDeltas() {
  if (stateOfChargeDeltas.size() >= 30) {
    stateOfChargeDeltas.pop_front();
  }
  std::optional<float> soc = getStateOfCharge();
  if (soc) {
    stateOfChargeDeltas.append(*soc - (prevSoc ? *prevSoc : 0));
    prevSoc = soc;
  }
}

QList<DebugTableRowValue> Model::getDebugTableValues() {
  QList<DebugTableRowValue> table;
  for (int i = 0; i < currentData.size(); ++i) {
    std::optional<float> value = currentData[i];
    QString name = DATA_IDS.find(i)->second.name;
    QString units = DATA_IDS.find(i)->second.units;
    DebugTableRowValue row = {
        i, name,
        value ? QString::number(std::round(*value * 1000) / 1000) : "N/A",
        units};

    table.emplace_back(row);
  }
  return table;
}

std::optional<float> Model::getById(int id) { return this->currentData[id]; }

const std::map<int, DataInfo> DATA_IDS = {
    {0, {"Mock Data", ""}},
    {1, {"Pack Inst Voltage", "V"}},
    {2, {"Pack Current", "A"}},
    {3, {"Pack Amphours", "Ah"}},
    {4, {"Pack SOC", "%"}},
    {5, {"Pack Health", "%"}},
    {6, {"Failsafe Statuses", "HEX"}},
    {7, {"DTC Status 1", "HEX"}},
    {8, {"DTC Status 2", "HEX"}},
    {9, {"Current Limits Status", ""}},
    {10, {"Average Temp", "C"}},
    {11, {"Internal Temp", "C"}},
    {12, {"MPE State", "BIN"}},
    {13, {"High Cell Voltage", "V"}},
    {14, {"High Cell Voltage ID", ""}},
    {15, {"Low Cell Voltage", "V"}},
    {16, {"Low Cell Voltage ID", ""}},
    {17, {"Average Cell Voltage", "V"}},
    {18, {"Module A Temperature", "C"}},
    {19, {"Module B Temperature", "C"}},
    {20, {"Module C Temperature", "C"}},
    {21, {"Gate Driver Board Temperature", "C"}},
    {22, {"Control Board Temperature", "C"}},
    {23, {"RTD #1 Temperature", "C"}},
    {24, {"RTD #2 Temperature", "C"}},
    {25, {"RTD #3 Temperature", "C"}},
    {26, {"RTD #4 Temperature", "C"}},
    {27, {"RTD #5 Temperature", "C"}},
    {28, {"Motor Temperature", "C"}},
    {29, {"Torque Shudder", "N-m"}},
    {30, {"Analog Input 1", "V"}},
    {31, {"Analog Input 2", "V"}},
    {32, {"Analog Input 3", "V"}},
    {33, {"Analog Input 4", "V"}},
    {34, {"Analog Input 5", "V"}},
    {35, {"Analog Input 6", "V"}},
    {36, {"Digital Input 1", "BIN"}},
    {37, {"Digital Input 2", "BIN"}},
    {38, {"Digital Input 3", "BIN"}},
    {39, {"Digital Input 4", "BIN"}},
    {40, {"Digital Input 5", "BIN"}},
    {41, {"Digital Input 6", "BIN"}},
    {42, {"Digital Input 7", "BIN"}},
    {43, {"Digital Input 8", "BIN"}},
    {44, {"Motor Angle (Electrical)", "Deg"}},
    {45, {"Motor Speed", "RPM"}},
    {46, {"Electrical Output Frequency", "Hz"}},
    {47, {"Delta Resolver Filtered", "Deg"}},
    {48, {"Phase A Current", "A"}},
    {49, {"Phase B Current", "A"}},
    {50, {"Phase C Current", "A"}},
    {51, {"DC Bus Current", "A"}},
    {52, {"DC Bus Voltage", "V"}},
    {53, {"Output Voltage", "V"}},
    {54, {"VAB_Vd Voltage", "V"}},
    {55, {"VBC_Vq Voltage", "V"}},
    {56, {"Flux Command", "Wb"}},
    {57, {"Flux Feedback", "Wb"}},
    {58, {"Id Feedback", "A"}},
    {59, {"Iq Feedback", "A"}},
    {60, {"1.5V Reference Voltage", "V"}},
    {61, {"2.5V Reference Voltage", "V"}},
    {62, {"5.0V Reference Voltage", "V"}},
    {63, {"12V System Voltage", "V"}},
    {64, {"VSM State", ""}},
    {65, {"Inverter State", ""}},
    {66, {"Relay State", "BIN"}},
    {67, {"Inverter Run Mode", "BIN"}},
    {68, {"Inverter Active Discharge State", "BIN"}},
    {69, {"Inverter Command Mode", "BIN"}},
    {70, {"Inverter Enable State", "BIN"}},
    {71, {"Inverter Enable Lockout", "BIN"}},
    {72, {"Direction Command", "BIN"}},
    {73, {"BMS Active", "BIN"}},
    {74, {"BMS Limiting Torque", "BIN"}},
    {75, {"POST Fault Lo", "BIN"}},
    {76, {"POST Fault Hi", "BIN"}},
    {77, {"Run Fault Lo", "BIN"}},
    {78, {"Run Fault Hi", "BIN"}},
    {79, {"Commanded Torque", "N-m"}},
    {80, {"Torque Feedback", "N-m"}},
    {81, {"Power on Timer", "s"}},
    {82, {"Torque Command", "N-m"}},
    {83, {"Speed Command", "RPM"}},
    {84, {"Direction Command", "BIN"}},
    {85, {"Inverter Enable", "BIN"}},
    {86, {"Inverter Discharge", "BIN"}},
    {87, {"Speed Mode Enable", "BIN"}},
    {88, {"Commanded Torque Limit", "N-m"}},
    {89, {"Pack DCL", "A"}},
    {90, {"Pack CCL", "A"}},
    {91, {"TCU X-Axis Acceleration", "g"}},
    {92, {"TCU Y-Axis Acceleration", "g"}},
    {93, {"TCU Z-Axis Acceleration", "g"}},
    {94, {"TCU Temperature C", "C"}},
    {95, {"TCU Temperature F", "F"}},
    {96, {"Relative Humidity", "%"}},
    {97, {"Cell Voltage Info", ""}},
    {98, {"GLV Current", "A"}},
    {99, {"Strain Gauge Voltage 1", "V"}},
    {100, {"Strain Gauge Voltage 2", "V"}},
    {101, {"Vehicle Speed", "MPH"}},
    {102, {"Wheel Knob 1", ""}},
    {103, {"Wheel Knob 2", ""}},
    {104, {"Wheel Buttons", "BIN"}},
    {105, {"MPU Mode State", ""}},
    {106, {"BMS State", ""}},
    {107, {"BMS Faults", "HEX"}},
    {108, {"Latitude", "Deg"}},
    {109, {"Longitude", "Deg"}},
    {110, {"GPS Fix Status", ""}},
    {111, {"Altitude", "m"}},
    {112, {"Ground Speed", "m/s"}},
    {113, {"Heading Direction", "Deg"}},
    {114, {"High Cell Temp", "C"}},
    {115, {"High Cell Temp Chip Number", ""}},
    {116, {"High Cell Temp Cell Number", ""}},
    {117, {"Low Cell Temp", "C"}},
    {118, {"Low Cell Temp Chip Number", ""}},
    {119, {"Low Cell Temp Cell Number", ""}},
    {120, {"Average Cell Temp", "C"}},
    {121, {"High Cell Voltage Chip Number", ""}},
    {122, {"High Cell Voltage Cell Number", ""}},
    {123, {"Low Cell Voltage Chip Number", ""}},
    {124, {"Low Cell Voltage Cell Number", ""}},
    {125, {"Segment 1 Average Temperature", "C"}},
    {126, {"Segment 2 Average Temperature", "C"}},
    {127, {"Segment 3 Average Temperature", "C"}},
    {128, {"Segment 4 Average Temperature", "C"}},
    {129, {"Logging Status", ""}},
    {130, {"Accumulator Fan Percentage", "%"}},
    {131, {"Motor Fan Percentage", "%"}},
    {132, {"Torque Limit Percentage", "%"}},
    {133, {"Regen Strength Value", ""}},
    {134, {"Charger State", ""}},
    {135, {"Measurement System Valid", ""}},
    {136, {"System Status", ""}},
    {137, {"Charge Status", ""}},
    {138, {"ibat", "A"}},
    {139, {"vbat", "V"}},
    {140, {"vin", "V"}},
    {141, {"vsys", "V"}},
    {142, {"iin", "A"}},
    {143, {"Cell Burning Status", ""}},
    {144, {"Traction Control On", ""}},
    {145, {"Precharge State", ""}},
    {146, {"BMS Prefault Status", ""}}};
