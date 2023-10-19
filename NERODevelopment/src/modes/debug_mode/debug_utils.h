#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <QList>
#include <QString>

/**
 * @brief The DebugTableRowValue class
 * @param id the id of the value being displayed
 * @param name the name of the value being displayed
 * @param value the value being displayed
 * @param unit the unit of the value being displayed
 */
class DebugTableRowValue {
public:
  DebugTableRowValue(int id, const QString &name, const QString &value,
                     const QString &unit);

  int id() const;
  QString name() const;
  QString value() const;
  QString unit() const;

private:
  int m_id;
  QString m_name;
  QString m_value;
  QString m_unit;
};

/**
 * @brief DebugPlotValue::DebugPlotValue A container to store the values to
 * display in the debug plot
 * @param name the name of the value to plot
 * @param unit the unit of the value to plot
 * @param data the list of data to display
 */
class DebugPlotValue {
public:
  DebugPlotValue(const QString &name, const QString &unit,
                 const QList<float> &data);
  QList<float> data;
  QString name;
  QString unit;
};

/**
 * @brief FaultInstance::FaultInstance A container to store the values to be
 * displayed on a fault screen
 * @param fault_decimal the decimal value of the fault code
 * @param max_cell_temp the max cell temp at the time of the fault
 * @param max_cell_voltage the max cell voltage at the time of the fault
 * @param average_cell_temp the average cell temp value at the time of the fault
 * @param average_cell_voltage the average cell voltage at the time of the fault
 * @param min_cell_temp the minimum cell temp at the time of the fault
 * @param min_cell_voltage the minimum cell voltage at the time of the fault
 * @param pack_current the pack current at the time of the fault
 * @param dcl the discharge current limit at the time of the fault
 * @param ccl the charge current limit at the time of the fault
 */
class FaultInstance {
public:
  FaultInstance(int fault_decimal, int max_cell_temp, int max_cell_voltage,
                int average_cell_temp, int average_cell_voltage,
                int min_cell_temp, int min_cell_voltage, int pack_current,
                int dcl, int ccl);

  QList<QString> faults;
  QString max_cell_temp;
  QString max_cell_voltage;
  QString average_cell_temp;
  QString average_cell_voltage;
  QString min_cell_temp;
  QString min_cell_voltage;
  QString pack_current;
  QString dcl;
  QString ccl;
};

#endif // DEBUG_UTILS_H
