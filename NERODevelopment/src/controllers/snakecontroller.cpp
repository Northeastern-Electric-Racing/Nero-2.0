#include "snakecontroller.h"

SnakeController::SnakeController(Model *model, QObject *parent)
    : ButtonController{model, 7, parent} {}

bool SnakeController::didStart() const { return m_didStart; }
void SnakeController::setDidStart(bool didStart) {
  if (didStart != this->m_didStart) {
    this->m_didStart = didStart;
    emit didStartChanged();
  }
}

int SnakeController::direction() const { return m_direction; }
void SnakeController::setDirection(int direction) {
  if (!isSameOrOppositeDirection(direction)) {
    this->m_direction = direction;
    emit directionChanged();
  }
}

bool SnakeController::gameOver() const { return m_gameOver; }
void SnakeController::setGameOver(bool gameOver) {
  if (gameOver != this->m_gameOver) {
    this->m_gameOver = gameOver;
    emit gameOverChanged();
  }
}

void SnakeController::enterButtonPressed() {
  if (this->m_gameOver) {
    this->setGameOver(false);
    this->setDirection(1);
    this->setDidStart(true);
    this->setDidStart(false);
  }
}

void SnakeController::upButtonPressed() { this->setDirection(0); }

void SnakeController::downButtonPressed() { this->setDirection(2); }

void SnakeController::leftButtonPressed() { this->setDirection(3); }

void SnakeController::rightButtonPressed() { this->setDirection(1); }

bool SnakeController::isSameOrOppositeDirection(int newDirection) {
  return (m_direction + 2) % 4 == newDirection || m_direction == newDirection;
}

void SnakeController::resetGame() {
  m_gameOver = true;
  m_direction = 1;
  m_didStart = false;
  emit gameOverChanged();
  emit directionChanged();
  emit didStartChanged();
}

void SnakeController::saveScore(int score) {
  QString topic = "SNAKE/SCORE";
  this->m_model->sendMessage(topic, score);
}
