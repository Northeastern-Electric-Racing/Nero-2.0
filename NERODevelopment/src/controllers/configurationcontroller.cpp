#include "configurationcontroller.h"

ConfigurationController::ConfigurationController(Model *model, QObject *parent)
    : ButtonController{model, 3, parent} {}

int ConfigurationController::selectedConfigurationIndex() const {
  return this->m_selectedConfigurationIndex;
}
void ConfigurationController::setSelectedConfigurationIndex(int index) {
  if (this->m_selectedConfigurationIndex != index) {
    this->m_selectedConfigurationIndex = index;
    emit this->selectedConfigurationIndexChanged();
  }
}

bool ConfigurationController::isKeyboardSelected() const {
  return this->m_isKeyboardSelected;
}
void ConfigurationController::setIsKeyboardSelected(bool selected) {
  this->m_isKeyboardSelected = selected;
  emit this->isKeyboardSelectedChanged();
}

void ConfigurationController::setDriverName(QString driverName) {
  if (this->m_driverName != driverName) {
    this->m_driverName = driverName;
  }
}

void ConfigurationController::setSystemName(QString systemName) {
  if (this->m_systemName != systemName) {
    this->m_systemName = systemName;
  }
}

void ConfigurationController::setLocationName(QString locationName) {
  if (this->m_locationName != locationName) {
    this->m_locationName = locationName;
  }
}

void ConfigurationController::downButtonPressed() {
  if (!this->m_isKeyboardSelected) {
    this->setSelectedConfigurationIndex(std::min(
        this->m_numConfigs - 1, this->m_selectedConfigurationIndex + 1));
  }
}

void ConfigurationController::upButtonPressed() {
  if (!this->m_isKeyboardSelected) {
    this->setSelectedConfigurationIndex(
        std::max(0, this->m_selectedConfigurationIndex - 1));
  }
}

void ConfigurationController::enterButtonPressed() {
  if (!this->m_isKeyboardSelected) {
    if (this->m_selectedConfigurationIndex == 3) {
      this->m_model->sendMessage("driver", this->m_driverName);
      this->m_model->sendMessage("system", this->m_systemName);
      this->m_model->sendMessage("location", this->m_locationName);
    } else {
      this->setIsKeyboardSelected(true);
    }
  }
}
