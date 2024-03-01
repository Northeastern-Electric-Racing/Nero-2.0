#include "offviewcontroller.h"
#include "../utils/data_type_names.h"

OffViewController::OffViewController(Model *model, QObject *parent)
    : ButtonController{model, parent}, m_packTemp(0.0), m_motorTemp(0.0),
      m_stateOfCharge(0.0) {
  connect(m_model, &Model::onCurrentDataChange, this,
          &OffViewController::update);
}

QMap<QString, AttributeStatus> OffViewController::attributeStatus() const {
  return m_attributeStatus;
}
void OffViewController::setAttributeStatus(const QString &name,
                                           AttributeStatus status) {
  if (m_attributeStatus.value(name) != status) {
    m_attributeStatus[name] = status;
    emit attributeStatusChanged(m_attributeStatus);
  }
}

void OffViewController::update() {
  for (const auto &attributeName : {SIDEBRBS, BMS, BSPD, MPU, BOTS, INERTIA,
                                    CPBRB, TSMS, HVDINTRLK, HVCNCTR}) {
    std::optional<float> value = this->m_model->getById(attributeName);
    if (value) {
      setAttributeStatus(attributeName,
                         this->mapFloatToAttributeStatus(*value));
    } else {
      setAttributeStatus(attributeName, AttributeStatus::OFF);
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

int OffViewController::selectedFlowIndex() const { return m_selectedFlowIndex; }
void OffViewController::setSelectedFlowIndex(int index) {
  if (index != this->m_selectedFlowIndex) {
    this->m_selectedFlowIndex = index;
    emit selectedFlowIndexChanged();
  }
}

bool OffViewController::didSelect() const { return m_didSelect; }
void OffViewController::setDidSelect(bool didSelect) {
  if (didSelect != this->m_didSelect) {
    qDebug() << "did select";
    this->m_didSelect = didSelect;
    emit didSelectChanged();
  }
}

QString OffViewController::selectedName() const { return m_selectedName; }
void OffViewController::setSelectedName(QString name) {
  if (name != this->m_selectedName) {
    this->m_selectedName = name;
    emit this->selectedNameChanged();
  }
}

QString OffViewController::selectedDescription() const {
  return m_selectedDescription;
}
void OffViewController::setSelectedDescription(QString description) {
  if (description != this->m_selectedDescription) {
    this->m_selectedDescription = description;
    emit this->selectedDescriptionChanged();
  }
}

QString OffViewController::selectedUrl() const { return m_selectedUrl; }
void OffViewController::setSelectedUrl(QString url) {
  if (url != this->m_selectedUrl) {
    this->m_selectedUrl = url;
    emit this->selectedUrlChanged();
  }
}

void OffViewController::enterButtonPressed() {
  DescriptionInfo info =
      this->m_descriptionInfo->value(this->m_selectedFlowIndex);
  this->setSelectedName(info.name());
  this->setSelectedDescription(info.description());
  this->setSelectedUrl(info.url());
  this->setDidSelect(!this->m_didSelect);
}

void OffViewController::downButtonPressed() {
  if (this->m_selectedFlowIndex + 1 <= this->m_attributeStatus.count()) {
    this->setSelectedFlowIndex(this->m_selectedFlowIndex + 1);
  }
}

void OffViewController::upButtonPressed() {
  if (this->m_selectedFlowIndex - 1 >= 0) {
    this->setSelectedFlowIndex(this->m_selectedFlowIndex - 1);
  }
}

void OffViewController::leftButtonPressed() {
  int half = this->m_attributeStatus.count() / 2;
  if (this->m_selectedFlowIndex - half >= 0) {
    this->setSelectedFlowIndex(this->m_selectedFlowIndex - half);
  }
}

void OffViewController::rightButtonPressed() {
  int half = this->m_attributeStatus.count() / 2;
  if (this->m_selectedFlowIndex + half <= this->m_attributeStatus.count()) {
    this->setSelectedFlowIndex(this->m_selectedFlowIndex + half);
  }
}
