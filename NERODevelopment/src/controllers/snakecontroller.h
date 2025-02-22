#ifndef SNAKECONTROLLER_H
#define SNAKECONTROLLER_H

#include "buttoncontroller.h"
#include <QObject>

class SnakeController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(bool didStart READ didStart WRITE setDidStart NOTIFY
                 didStartChanged FINAL)
  Q_PROPERTY(int direction READ direction WRITE setDirection NOTIFY
                 directionChanged FINAL)
  Q_PROPERTY(bool gameOver READ gameOver WRITE setGameOver NOTIFY
                 gameOverChanged FINAL)

public:
  explicit SnakeController(Model *model, QObject *parent = nullptr);
  bool didStart() const;
  int direction() const;
  bool gameOver() const;

public slots:
  void enterButtonPressed() override;
  void upButtonPressed() override;
  void downButtonPressed() override;
  void rightButtonPressed() override;
  void leftButtonPressed() override;
  void saveScore(int);
  void setDidStart(bool);
  void setDirection(int);
  void setGameOver(bool);

signals:
  void didStartChanged();
  void directionChanged();
  void gameOverChanged();

private:
  bool isSameOrOppositeDirection(int newDirection);
  bool m_didStart = false;
  int m_direction = 1;
  bool m_gameOver = true;
};

#endif // SNAKECONTROLLER_H
