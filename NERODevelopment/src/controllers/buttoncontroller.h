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
  long long m_enterButtonDebounce = 0;
  long long m_upButtonDebounce = 0;
  long long m_downButtonDebounce = 0;
  long long m_rightButtonDebounce = 0;
  long long m_leftButtonDebounce = 0;
  long long m_homeButtonDebounce = 0;
  int m_debounceOffset = 500;
};

#endif // BUTTONCONTROLLER_H
