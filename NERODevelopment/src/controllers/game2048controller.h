#ifndef GAME2048CONTROLLER_H
#define GAME2048CONTROLLER_H

#include "buttoncontroller.h"
#include <QObject>

class Game2048Controller : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(bool gameOver READ gameOver WRITE setGameOver NOTIFY
                 gameOverChanged FINAL)
  Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged FINAL)

public:
  explicit Game2048Controller(Model *model, QObject *parent = nullptr);

  bool gameOver() const;
  int score() const;

public slots:
  void setGameOver(bool gameOver);
  void setScore(int score);
  void enterButtonPressed() override;
  void upButtonPressed() override;
  void downButtonPressed() override;
  void leftButtonPressed() override;
  void rightButtonPressed() override;
  void saveScore(int score);

signals:
  void gameOverChanged();
  void scoreChanged();
  void moveMade(int direction);
  void restartRequested();

private:
  bool m_gameOver = true;
  int m_score = 0;
};

#endif // GAME2048CONTROLLER_H
