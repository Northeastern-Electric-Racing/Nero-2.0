#include "buttoncontroller.h"

ButtonController::ButtonController(Model *model, QObject *parent)
    : QObject{parent} {
  this->m_model = model;
  connect(model, &Model::onCurrentDataChange, this,
          &ButtonController::buttonUpdate);
}

void ButtonController::enterButtonPressed() {}

void ButtonController::downButtonPressed() {}

void ButtonController::upButtonPressed() {}

void ButtonController::leftButtonPressed() {}

void ButtonController::rightButtonPressed() {}

void ButtonController::buttonUpdate() {
  if (this->m_model->getBackwardButtonPressed()) {
    this->leftButtonPressed();
  }
  if (this->m_model->getDownButtonPressed()) {
    this->downButtonPressed();
  }
  if (this->m_model->getEnterButtonPressed()) {
    this->enterButtonPressed();
  }
  if (this->m_model->getRightButtonPressed()) {
    this->rightButtonPressed();
  }
  if (this->m_model->getDownButtonPressed()) {
    this->downButtonPressed();
  }
}
