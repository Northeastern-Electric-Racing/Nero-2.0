#include "flappybirdcontroller.h"

FlappyBirdController::FlappyBirdController(Model *model, QObject *parent)
    : ButtonController{model, 6, parent} {
  this->m_debounceOffset = 150;
}

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

void FlappyBirdController::saveScore(int score) {
  QString topic = "NERO/FLAPPYBIRD/SCORE";
  QString message = QString::number(score);
  m_model->sendMessage(topic, message);
}
