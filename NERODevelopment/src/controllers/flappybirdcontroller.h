#ifndef FLAPPYBIRDCONTROLLER_H
#define FLAPPYBIRDCONTROLLER_H

#include "buttoncontroller.h"
#include <QObject>

class FlappyBirdController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(
      bool didJump READ didJump WRITE setDidJump NOTIFY didJumpChanged FINAL)
  Q_PROPERTY(int bestScore READ bestScore NOTIFY bestScoreChanged FINAL)
public:
  explicit FlappyBirdController(Model *model, QObject *parent = nullptr);

  bool didJump() const;
  int bestScore() const;

public slots:
  void setDidJump(bool);
  void enterButtonPressed() override;
  void saveScore(int);

signals:
  void didJumpChanged();
  void bestScoreChanged();

private:
  bool m_didJump = false;
  int m_bestScore = 0;
};

#endif // FLAPPYBIRDCONTROLLER_H
