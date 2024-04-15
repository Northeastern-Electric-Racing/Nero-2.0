#include "keyboardcontroller.h"

KeyboardController::KeyboardController(Model *model, QObject *parent)
    : ButtonController{model, 5, parent} {}

int KeyboardController::selectedIndex() const { return this->m_selectedIndex; }

bool KeyboardController::selectedKey() const { return this->m_selectedKey; }

void KeyboardController::setSelectedIndex(int index) {
  if (this->m_selectedIndex != index) {
    this->m_selectedIndex = index;
    emit selectedIndexChanged();
  }
}

void KeyboardController::setSelectedKey(bool selected) {
  if (this->m_selectedKey != selected) {
    this->m_selectedKey = selected;
    emit selectedKeyChanged();
  }
}

void KeyboardController::downButtonPressed() {
  qDebug() << "down Button Pressed Keyboard";

  this->setSelectedIndex(
      std::min(this->m_numKeys - 1, this->m_selectedIndex + this->m_columns));
}

void KeyboardController::upButtonPressed() {
  qDebug() << "up Button Pressed Keyboard";

  this->setSelectedIndex(std::max(0, this->m_selectedIndex - this->m_columns));
}

void KeyboardController::leftButtonPressed() {
  this->setSelectedIndex(std::max(0, this->m_selectedIndex - 1));
}

void KeyboardController::rightButtonPressed() {
  this->setSelectedIndex(
      std::min(this->m_numKeys - 1, this->m_selectedIndex + 1));
}

void KeyboardController::setExit(bool exit) {
  this->m_exit = exit;
  emit exitChanged();
}
bool KeyboardController::exit() const { return this->m_exit; }

void KeyboardController::homeButtonPressed() {
  qDebug() << "Home Button Pressed Keyboard";
  this->setExit(true);
  this->m_model->currentPageIndex = -1;
}

void KeyboardController::enterButtonPressed() {
  this->setSelectedKey(true);
  this->setSelectedKey(false);
}
