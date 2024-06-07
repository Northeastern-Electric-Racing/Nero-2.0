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
  virtual void buttonUpdate();

  virtual void enterButtonPressed();
  virtual void upButtonPressed();
  virtual void downButtonPressed();
  virtual void rightButtonPressed();
  virtual void leftButtonPressed();
  virtual void homeButtonPressed();

protected:
  Model *m_model;
  int m_pageIndex;
  qint64 m_enterButtonDebounce = 0;
  qint64 m_upButtonDebounce = 0;
  qint64 m_downButtonDebounce = 0;
  qint64 m_rightButtonDebounce = 0;
  qint64 m_leftButtonDebounce = 0;
  qint64 m_homeButtonDebounce = 0;
  int m_debounceOffset = 500;
  bool m_downButtonDebounceRunning = false;
  bool m_upButtonDebounceRunning = false;
  bool m_rightButtonDebounceRunning = false;
  bool m_leftButtonDebounceRunning = false;
  bool m_enterButtonDebounceRunning = false;
};

#endif // BUTTONCONTROLLER_H
