#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include "../models/model.h"
#include "buttons.h"
#include <QObject>

class ButtonController : public QObject {
  Q_OBJECT
public:
  explicit ButtonController(Model *model, QObject *parent = nullptr);

  Q_INVOKABLE void press(Buttons::Button button);

public slots:
  virtual void enterButtonPressed();
  virtual void upButtonPressed();
  virtual void downButtonPressed();
  virtual void rightButtonPressed();
  virtual void leftButtonPressed();

protected:
  Model *m_model;
};

#endif // BUTTONCONTROLLER_H
