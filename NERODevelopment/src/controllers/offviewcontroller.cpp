#include "offviewcontroller.h"
#include "../utils/data_type_names.h"

OffViewController::OffViewController(Model *model, QObject *parent)
    : ButtonController{model, parent}, m_packTemp(0.0), m_motorTemp(0.0),
      m_stateOfCharge(0.0) {
  connect(m_model, &Model::onCurrentDataChange, this,
          &OffViewController::updateAttributesFromModel);
}

QMap<QString, AttributeStatus> OffViewController::getAttributeStatus() const {
  return m_attributeStatus;
}
void OffViewController::setAttributeStatus(const QString &name,
                                           AttributeStatus status) {
  if (m_attributeStatus.value(name) != status) {
    m_attributeStatus[name] = status;
    emit attributeStatusChanged(m_attributeStatus);
  }
}

void OffViewController::updateAttributesFromModel() {
  for (const auto &attributeName : {SIDEBRBS, BMS, BSPD, MPU, BOTS, INERTIA,
                                    CPBRB, TSMS, HVDINTRLK, HVCNCTR}) {
    std::optional<float> value = this->m_model->getById(attributeName);
    if (value) {
      setAttributeStatus(attributeName,
                         this->mapFloatToAttributeStatus(*value));
    }
  }
  setPackTemp(*m_model->getPackTemp());
  setMotorTemp(*m_model->getMotorTemp());
  setStateOfCharge(*m_model->getStateOfCharge());
}

AttributeStatus OffViewController::mapFloatToAttributeStatus(float value) {
  return AttributeStatus(int(value));
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
