#include "debug_utils.h"
#include "../../constants/fault_statuses.h"

DebugPlotValue::DebugPlotValue(const QString &name, const QString &unit,
                               const QList<float> &data)
    : data(data), name(name), unit(unit) {
  // Constructor implementation
}

DebugPlotValue::DebugPlotValue() : data(), name(""), unit("") {}

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

/*DEBUG TABLE ROW VALUE*/
DebugTableRowValue::DebugTableRowValue(const QString &name, const float &value,
                                       const QString &unit)
    : m_name(name), m_value(value), m_unit(unit) {}

QString DebugTableRowValue::name() const { return m_name; }

float DebugTableRowValue::value() const { return m_value; }

QString DebugTableRowValue::unit() const { return m_unit; }

QJsonObject DebugTableRowValue::json() const {
  QJsonObject object;
  object["name"] = this->m_name;
  object["value"] = this->m_value;
  object["unit"] = this->m_unit;

  return object;
}

/*DEBUG TABLE ROW TOPICS*/
DebugTableRowTopics::DebugTableRowTopics() { this->m_topics = {}; }
void DebugTableRowTopics::setTopics(const QVector<QString> topics) {
  this->m_topics = topics;
}

QList<QJsonObject> DebugTableRowTopics::json() const {
  QList<QJsonObject> jsonArray;
  for (const QString &topic : this->m_topics) {
    QJsonObject topicObject;
    topicObject["topic"] = topic;
    jsonArray.append(topicObject);
  }

  return jsonArray;
}

QVector<QString> DebugTableRowTopics::topics() const { return this->m_topics; }

/*DEBUG TABLE ROW VALUES*/
DebugTableRowValues::DebugTableRowValues() { this->m_values = {}; }

void DebugTableRowValues::setDebugTableRowValues(
    const QVector<DebugTableRowValue> values) {
  this->m_values = values;
}

QList<QJsonObject> DebugTableRowValues::json() const {
  QList<QJsonObject> jsonArray;
  for (const DebugTableRowValue &row : this->m_values) {
    jsonArray.append(row.json());
  }
  return jsonArray;
}

QVector<DebugTableRowValue> DebugTableRowValues::values() const {
  return this->m_values;
}
