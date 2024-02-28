#ifndef FLAPPYBIRDCONTROLLER_H
#define FLAPPYBIRDCONTROLLER_H

#include "buttoncontroller.h"
#include <QObject>

class FlappyBirdController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(
      bool didJump READ didJump WRITE setDidJump NOTIFY didJumpChanged FINAL)
public:
  explicit FlappyBirdController(Model *model, QObject *parent = nullptr);

  bool didJump() const;

public slots:
  void setDidJump(bool);

  void enterButtonPressed() override;
signals:
  void didJumpChanged();

private:
  bool m_didJump = false;
};

#endif // FLAPPYBIRDCONTROLLER_H
