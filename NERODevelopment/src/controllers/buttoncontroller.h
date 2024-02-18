#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include "src/models/model.h"
#include <QObject>

class ButtonController : public QObject {
  Q_OBJECT
public:
  explicit ButtonController(Model *model, QObject *parent = nullptr);

public slots:
  void buttonUpdate();

protected:
  virtual void enterButtonPressed();
  virtual void upButtonPressed();
  virtual void downButtonPressed();
  virtual void rightButtonPressed();
  virtual void leftButtonPressed();
  Model *m_model;
};

#endif // BUTTONCONTROLLER_H
