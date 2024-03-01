#include "flappybirdcontroller.h"

FlappyBirdController::FlappyBirdController(Model *model, QObject *parent)
    : ButtonController{model, parent} {}

bool FlappyBirdController::didJump() const { return m_didJump; }
void FlappyBirdController::setDidJump(bool didJump) {
  if (didJump != this->m_didJump) {
    this->m_didJump = didJump;
    emit didJumpChanged();
  }
}

void FlappyBirdController::enterButtonPressed() {
  this->setDidJump(true);
  this->setDidJump(false);
}
