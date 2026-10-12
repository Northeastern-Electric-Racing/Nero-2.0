#include "buttoncontroller.h"

ButtonController::ButtonController(Model *model, QObject *parent)
    : QObject{parent}, m_model{model} {}

void ButtonController::press(Buttons::Button button) {
  switch (button) {
  case Buttons::Enter:
    enterButtonPressed();
    break;
  case Buttons::UpRegen:
    upButtonPressed();
    break;
  case Buttons::DownRegen:
    downButtonPressed();
    break;
  case Buttons::Left:
    leftButtonPressed();
    break;
  case Buttons::Right:
    rightButtonPressed();
    break;
  default:
    break;
  }
}

void ButtonController::enterButtonPressed() {}

void ButtonController::upButtonPressed() {}

void ButtonController::downButtonPressed() {}

void ButtonController::rightButtonPressed() {}

void ButtonController::leftButtonPressed() {}
