#include "snakecontroller.h"

SnakeController::SnakeController(Model *model, QObject *parent)
    : ButtonController{model, 5, parent} {}

void SnakeController::handleKeyPress(int key) {
  int newDirection = m_currentDirection;

  switch (key) {
  case Qt::Key_Up:
    newDirection = 0;
    break;

  case Qt::Key_Right:
    newDirection = 1;
    break;

  case Qt::Key_Down:
    newDirection = 2;
    break;

  case Qt::Key_Left:
    newDirection = 3;
    break;

  default:
    return;
  }

  if (!isSameOrOppositeDirection(newDirection)) {
    m_currentDirection = newDirection;
    emit directionChanged(m_currentDirection);
  }
}

bool SnakeController::isSameOrOppositeDirection(int newDirection) {
  return (m_currentDirection + 2) % 4 == newDirection ||
         m_currentDirection == newDirection;
}

void SnakeController::saveScore(int score) {
  QString topic = "NERO/SNAKE/SCORE";
  m_model->sendMessage(topic, score);
}
