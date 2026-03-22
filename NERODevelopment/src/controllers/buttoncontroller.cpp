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

    // Inside tabs (page >= 0): directional buttons from SOCKET only
    // Ignore MQTT-sourced directional buttons when inside a tab
    if (!this->m_model->lastButtonFromSocket) {
        return;
    }

    // Read shared button key ONCE, dispatch by value
    std::optional<float> buttonValue = this->m_model->getById(ENTERBUTTON);
    if (buttonValue.has_value()) {
        float val = buttonValue.value();

        if (val == 0) {
            qDebug() << "Back Button Pressed (socket)";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->leftButtonPressed();
        } else if (val == 1) {
            qDebug() << "Right Button Pressed (socket)";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->rightButtonPressed();
        } else if (val == 3) {
            qDebug() << "Down Button Pressed (socket)";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->downButtonPressed();
        } else if (val == 4) {
            qDebug() << "Up Button Pressed (socket)";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->upButtonPressed();
        } else if (val == 5) {
            qDebug() << "Enter Button Pressed (socket)";
            this->m_model->setValue(ENTERBUTTON, 10);
            this->enterButtonPressed();
        }
    }
}
