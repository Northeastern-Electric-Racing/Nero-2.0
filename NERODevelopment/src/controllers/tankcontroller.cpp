#include "tankcontroller.h"
#include <QKeyEvent>

TankController::TankController(Model *model, QObject *parent)
    : ButtonController{model, 8, parent}, m_barrelAngle(0), m_position(0) {}

void TankController::handleKeyPress(int key) {
  switch (key) {
  case Qt::Key_Left:
    m_position--;
    emit positionChanged(m_position);
    break;

  case Qt::Key_Right:
    m_position++;
    emit positionChanged(m_position);
    break;

  case Qt::Key_Up:
    m_barrelAngle++;
    emit barrelAngleChanged(m_barrelAngle);
    break;

  case Qt::Key_Down:
    m_barrelAngle--;
    emit barrelAngleChanged(m_barrelAngle);
    break;

  default:
    return;
  }
}

void TankController::saveScore(int score) {
  QString topic = "TANK_GAME/SCORE";
  m_model->sendMessage(topic, score);
}
