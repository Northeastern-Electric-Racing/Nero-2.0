#include "buttoncontroller.h"

ButtonController::ButtonController(Model *model, int pageIndex, QObject *parent)
    : ButtonController(model, QList<int>{pageIndex}, parent) {}

ButtonController::ButtonController(Model *model, QList<int> pageIndices,
                                   QObject *parent)
    : QObject{parent} {
    this->m_model = model;
    this->m_pageIndices = pageIndices;
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
    if (this->m_pageIndices.contains(this->m_model->currentPageIndex)) {
        // QDateTime currentDate = QDateTime::currentDateTime();
        // qDebug() << "Button Pressed"
        //          << this->m_model->getBackwardButtonPressed().value_or(false)
        //          << this->m_model->getDownButtonPressed().value_or(false)
        //          << this->m_model->getEnterButtonPressed().value_or(false)
        //          << this->m_model->getRightButtonPressed().value_or(false)
        //          << this->m_model->getUpButtonPressed().value_or(false)
        //          << this->m_model->getHomeButtonPressed().value_or(false)
        //          << this->m_model->getForwardButtonPressed().value_or(false)
        //          << this->m_model->currentPageIndex;

        if (this->m_model->getBackwardButtonPressed() == true) {
            qDebug() << "Back Button Pressed";
            this->leftButtonPressed();
        }

        if (this->m_model->getRightButtonPressed() == true) {
            qDebug() << "Right Button Pressed";

            this->rightButtonPressed();
        }

        if (this->m_model->getEnterButtonPressed() == true) {
            qDebug() << "Enter Button Pressed";

            this->enterButtonPressed();
        }

        if (this->m_model->getUpButtonPressed() == true) {
            qDebug() << "Up button pressed";
            this->upButtonPressed();
        }

        if (this->m_model->getDownButtonPressed() == true) {
            qDebug() << "Down Button Pressed";

            this->downButtonPressed();
        }

        if (this->m_model->getHomeButtonPressed() == true) {
            qDebug() << "Home button pressed";
            this->homeButtonPressed();
        }
    }
}
