#include "debug_utils.h"
#include "fault_statuses.h"

DebugPlotValue::DebugPlotValue(const QString &name, const QString &unit,
                               const QList<float> &data)
    : name(name), unit(unit), data(data) {
  // Constructor implementation
}

FaultInstance::FaultInstance(int fault_decimal, int max_cell_temp,
                             int max_cell_voltage, int average_cell_temp,
                             int average_cell_voltage, int min_cell_temp,
                             int min_cell_voltage, int pack_current, int dcl,
                             int ccl)
    : max_cell_temp(QString::number(max_cell_temp)),
      max_cell_voltage(QString::number(max_cell_voltage)),
      average_cell_temp(QString::number(average_cell_temp)),
      average_cell_voltage(QString::number(average_cell_voltage)),
      min_cell_temp(QString::number(min_cell_temp)),
      min_cell_voltage(QString::number(min_cell_voltage)),
      pack_current(QString::number(pack_current)), dcl(QString::number(dcl)),
      ccl(QString::number(ccl)) {
  int i = 0;
  while (fault_decimal > 0) {
    if (fault_decimal % 2 == 1) {
      faults.append(FAULTS[i]);
    }
    fault_decimal /= 2;
    i++;
  }
}

DebugTableRowValue::DebugTableRowValue(int id, const QString &name,
                                       const QString &value,
                                       const QString &unit)
    : m_id(id), m_name(name), m_value(value), m_unit(unit) {}

int DebugTableRowValue::id() const { return m_id; }

QString DebugTableRowValue::name() const { return m_name; }

QString DebugTableRowValue::value() const { return m_value; }

QString DebugTableRowValue::unit() const { return m_unit; }
