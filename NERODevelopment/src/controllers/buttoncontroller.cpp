#include "buttoncontroller.h"
#include "../utils/data_type_names.h"

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
    if (!this->m_pageIndices.contains(this->m_model->currentPageIndex)) {
        return;
    }

    // All 6 directional/action buttons share one key: "Wheel/Buttons/button_id".
    // Read it ONCE and dispatch by value to avoid consume-on-read race conditions.
    std::optional<float> buttonValue = this->m_model->getById(ENTERBUTTON);
    if (buttonValue.has_value()) {
        float val = buttonValue.value();

        if (val == 0) {
            qDebug() << "Back Button Pressed";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->leftButtonPressed();
        } else if (val == 1) {
            qDebug() << "Right Button Pressed";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->rightButtonPressed();
        } else if (val == 3) {
            qDebug() << "Down Button Pressed";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->downButtonPressed();
        } else if (val == 4) {
            qDebug() << "Up Button Pressed";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->upButtonPressed();
        } else if (val == 5) {
            qDebug() << "Enter Button Pressed";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->enterButtonPressed();
        }
        // val == 10 = released, ignore
    }

    // Home/exit uses a different topic — still comes via MQTT (client_2)
    std::optional<float> homeValue = this->m_model->getById(HOMEBUTTON);
    if (homeValue.has_value() && homeValue.value() == 1) {
        qDebug() << "Home Button Pressed";
        this->m_model->setValue(HOMEBUTTON, 10);
        this->homeButtonPressed();
    }
}
