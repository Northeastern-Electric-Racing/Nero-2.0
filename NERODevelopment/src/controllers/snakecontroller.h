#ifndef SNAKECONTROLLER_H
#define SNAKECONTROLLER_H

#include "buttoncontroller.h"
#include <QObject>

class SnakeController : public ButtonController {
  Q_OBJECT

public:
  explicit SnakeController(Model *model, QObject *parent = nullptr);

public slots:
  void handleKeyPress(int key);
  void saveScore(int);

signals:
  void directionChanged(int newDirection);

private:
  int m_currentDirection;
  bool isOppositeDirection(int newDirection);
};

#endif // SNAKECONTROLLER_H
