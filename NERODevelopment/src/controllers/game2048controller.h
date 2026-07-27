#ifndef GAME2048CONTROLLER_H
#define GAME2048CONTROLLER_H

#include "buttoncontroller.h"
#include <QObject>
#include <QVariantList>

class Game2048Controller : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(bool gameOver READ gameOver WRITE setGameOver NOTIFY
                 gameOverChanged FINAL)
  Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged FINAL)
  Q_PROPERTY(int bestScore READ bestScore NOTIFY bestScoreChanged FINAL)

public:
  explicit Game2048Controller(Model *model, QObject *parent = nullptr);

  bool gameOver() const;
  int score() const;
  int bestScore() const;

  Q_INVOKABLE bool hasSavedGame() const;
  Q_INVOKABLE QVariantList savedBoard() const;
  Q_INVOKABLE bool savedHasWon() const;
  Q_INVOKABLE void saveState(const QVariantList &board, bool hasWon);

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
  void bestScoreChanged();
  void moveMade(int direction);
  void restartRequested();

private:
  bool m_gameOver = true;
  int m_score = 0;
  int m_bestScore = 0;
  QVariantList m_savedBoard;
  bool m_savedHasWon = false;
  bool m_hasSavedGame = false;
};

#endif // GAME2048CONTROLLER_H
