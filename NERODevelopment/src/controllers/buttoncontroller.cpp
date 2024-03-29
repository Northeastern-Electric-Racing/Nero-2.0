#include "buttoncontroller.h"

ButtonController::ButtonController(Model *model, int pageIndex, QObject *parent)
    : QObject{parent} {
  this->m_model = model;
  this->m_pageIndex = pageIndex;
  connect(model, &Model::onCurrentDataChange, this,
          &ButtonController::buttonUpdate);
}

void ButtonController::enterButtonPressed() {}

void ButtonController::downButtonPressed() {}

void ButtonController::upButtonPressed() {}

void ButtonController::leftButtonPressed() {}

void ButtonController::rightButtonPressed() {}

void ButtonController::homeButtonPressed() {}

void ButtonController::buttonUpdate() {
  if (this->m_pageIndex == this->m_model->currentPageIndex) {
    if (this->m_model->getBackwardButtonPressed() == 1) {
      this->leftButtonPressed();
    }
    if (this->m_model->getDownButtonPressed() == 1) {
      this->downButtonPressed();
    }
    if (this->m_model->getEnterButtonPressed() == 1) {
      this->enterButtonPressed();
    }
    if (this->m_model->getRightButtonPressed() == 1) {
      this->rightButtonPressed();
    }
    if (this->m_model->getDownButtonPressed() == 1) {
      this->downButtonPressed();
    }
    if (this->m_model->getHomeButtonPressed() == 1) {
      this->homeButtonPressed();
    }
  }
}
