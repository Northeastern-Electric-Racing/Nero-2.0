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
    if (!this->m_model->lastButtonFromSocket) {
        return;
    }

    // Button mapping (from hardware proposal):
    //   1 = Escape (MQTT only — ignored here)
    //   2 = Left
    //   4 = Up
    //   5 = Down
    //   6 = Enter
    //   7 = Right
    std::optional<float> buttonValue = this->m_model->getById(SOCKETBUTTON);
    if (buttonValue.has_value()) {
        float val = buttonValue.value();

        if (val == 2) {
            qDebug() << "Left Button Pressed (socket)";
            this->m_model->setValue(SOCKETBUTTON, -1);
            this->leftButtonPressed();
        } else if (val == 7) {
            qDebug() << "Right Button Pressed (socket)";
            this->m_model->setValue(SOCKETBUTTON, -1);
            this->rightButtonPressed();
        } else if (val == 4) {
            qDebug() << "Up Button Pressed (socket)";
            this->m_model->setValue(SOCKETBUTTON, -1);
            this->upButtonPressed();
        } else if (val == 5) {
            qDebug() << "Down Button Pressed (socket)";
            this->m_model->setValue(SOCKETBUTTON, -1);
            this->downButtonPressed();
        } else if (val == 6) {
            qDebug() << "Enter Button Pressed (socket)";
            this->m_model->setValue(SOCKETBUTTON, -1);
            this->enterButtonPressed();
        }
        // 1 = escape (MQTT only, ignored here)
        // -1 = cleared/released, ignore
    }
}
