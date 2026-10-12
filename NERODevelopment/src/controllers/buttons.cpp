#include "buttons.h"
#include "../utils/data_type_names.h"

Buttons::Buttons(Model *model, QObject *parent)
    : QObject{parent}, m_model{model} {
  connect(model, &Model::topicReceived, this,
          [this](const QString &topic, float value) {
            // Range check first, so NaN never reaches the int conversion
            if (topic != BUTTONID || !(value >= Esc && value <= DownTorque))
              return;
            const int id = value;
            if (id == value)
              emit pressed(static_cast<Button>(id));
          });
}

void Buttons::press(Buttons::Button button) {
  m_model->sendMessage(BUTTONID, button);
}
