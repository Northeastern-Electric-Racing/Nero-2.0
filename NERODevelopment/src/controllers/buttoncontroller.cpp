#include "buttoncontroller.h"
#include "src/utils/data_type_names.h"

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

void ButtonController::homeButtonPressed() {
  this->m_model->currentPageIndex = -1;
}

void ButtonController::buttonUpdate() {
  if (this->m_pageIndex == this->m_model->currentPageIndex) {
    qDebug() << "Button Pressed"
             << this->m_model->getBackwardButtonPressed().value_or(-1)
             << this->m_model->getDownButtonPressed().value_or(-1)
             << this->m_model->getEnterButtonPressed().value_or(-1)
             << this->m_model->getRightButtonPressed().value_or(-1)
             << this->m_model->getUpButtonPressed().value_or(-1)
             << this->m_model->getHomeButtonPressed().value_or(-1)
             << this->m_model->getForwardButtonPressed().value_or(-1)
             << this->m_model->currentPageIndex;
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
    if (this->m_model->getUpButtonPressed()) {
      this->upButtonPressed();
    }
    if (this->m_model->getForwardButtonPressed()) {
      this->rightButtonPressed();
    }
    if (this->m_model->getHomeButtonPressed()) {
      this->homeButtonPressed();
    }
  }
}
