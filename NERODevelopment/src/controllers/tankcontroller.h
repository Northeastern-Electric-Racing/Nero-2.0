#ifndef TANKCONTROLLER_H
#define TANKCONTROLLER_H

#include "buttoncontroller.h"
#include <QObject>

class TankController : public ButtonController {
  Q_OBJECT

public:
  explicit TankController(Model *model, QObject *parent = nullptr);

public slots:
  void handleKeyPress(int key);
  void saveScore(int);

signals:
  void barrelAngleChanged(int newBarrelAngle);
  void positionChanged(int newPosition);

private:
  int m_barrelAngle = 0;
  int m_position;
};

#endif // TANKCONTROLLER_H
