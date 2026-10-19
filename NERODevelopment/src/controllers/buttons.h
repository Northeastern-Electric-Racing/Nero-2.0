#ifndef BUTTONS_H
#define BUTTONS_H

#include "../models/model.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

/**
 * @brief Wheel button presses. Each button_id message is one press.
 */
class Buttons : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Use the buttons context property")

public:
  // Same order as button_t in Cerberus-2.0 Core/Inc/u_buttons.h. These are the
  // 0-indexed wire values; the wheel silkscreen is 1-indexed, so silkscreen
  // n = wire n - 1. Layout: readme "Button Layout" and Confluence "Button IO
  // 25", https://nerdocs.atlassian.net/wiki/spaces/NER/pages/1526988828
  enum Button {
    Esc,
    Left,
    LaunchControl,
    UpRegen,
    DownRegen,
    Enter,
    Right,
    TractionControl,
    UpTorque,
    DownTorque
  };
  Q_ENUM(Button)

  explicit Buttons(Model *model, QObject *parent = nullptr);

  Q_INVOKABLE void press(Buttons::Button button);

signals:
  void pressed(Buttons::Button button);

private:
  Model *m_model;
};

#endif // BUTTONS_H
