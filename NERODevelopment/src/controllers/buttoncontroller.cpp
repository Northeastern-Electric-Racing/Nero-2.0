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
    if (this->m_model->getBackwardButtonPressed() == 1 &&
        QDateTime::currentDateTime().time().msec() -
                this->m_leftButtonDebounce >
            this->m_debounceOffset) {
      this->leftButtonPressed();
      this->m_leftButtonDebounce = QDateTime::currentDateTime().time().msec();
    }
    if (this->m_model->getDownButtonPressed() == 1 &&
        QDateTime::currentDateTime().time().msec() -
                this->m_downButtonDebounce >
            this->m_debounceOffset) {
      this->downButtonPressed();
      this->m_downButtonDebounce = QDateTime::currentDateTime().time().msec();
    }
    if (this->m_model->getEnterButtonPressed() == 1 &&
        QDateTime::currentDateTime().time().msec() -
                this->m_enterButtonDebounce >
            this->m_debounceOffset) {
      this->enterButtonPressed();
      this->m_enterButtonDebounce = QDateTime::currentDateTime().time().msec();
    }
    if (this->m_model->getRightButtonPressed() == 1 &&
        QDateTime::currentDateTime().time().msec() -
                this->m_rightButtonDebounce >
            this->m_debounceOffset) {
      this->rightButtonPressed();
      this->m_rightButtonDebounce = QDateTime::currentDateTime().time().msec();
    }
    if (this->m_model->getDownButtonPressed() == 1 &&
        QDateTime::currentDateTime().time().msec() -
                this->m_downButtonDebounce >
            this->m_debounceOffset) {
      this->downButtonPressed();
      this->m_downButtonDebounce = QDateTime::currentDateTime().time().msec();
    }
    if (this->m_model->getUpButtonPressed() == 1 &&
        QDateTime::currentDateTime().time().msec() - this->m_upButtonDebounce >
            this->m_debounceOffset) {
      this->upButtonPressed();
      this->m_upButtonDebounce = QDateTime::currentDateTime().time().msec();
    }
    if (this->m_model->getForwardButtonPressed() == 1 &&
        QDateTime::currentDateTime().time().msec() -
                this->m_rightButtonDebounce >
            this->m_debounceOffset) {
      this->rightButtonPressed();
      this->m_rightButtonDebounce = QDateTime::currentDateTime().time().msec();
    }
    if (this->m_model->getHomeButtonPressed() == 1 &&
        QDateTime::currentDateTime().time().msec() -
                this->m_homeButtonDebounce >
            this->m_debounceOffset) {
      this->homeButtonPressed();
      this->m_homeButtonDebounce = QDateTime::currentDateTime().time().msec();
    }
  }
}
