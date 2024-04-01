#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include "src/models/model.h"
#include <QObject>

class ButtonController : public QObject {
  Q_OBJECT
public:
  explicit ButtonController(Model *model, int pageIndex,
                            QObject *parent = nullptr);

public slots:
  void buttonUpdate();

  virtual void enterButtonPressed();
  virtual void upButtonPressed();
  virtual void downButtonPressed();
  virtual void rightButtonPressed();
  virtual void leftButtonPressed();
  virtual void homeButtonPressed();

protected:
  Model *m_model;
  int m_pageIndex;
};

#endif // BUTTONCONTROLLER_H
