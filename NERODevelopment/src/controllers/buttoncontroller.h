#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include "../models/model.h"
#include <QObject>

class ButtonController : public QObject {
  Q_OBJECT
public:
  explicit ButtonController(Model *model, int pageIndex,
                            QObject *parent = nullptr);
  explicit ButtonController(Model *model, QList<int> pageIndices,
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
  QList<int> m_pageIndices;
};

#endif // BUTTONCONTROLLER_H
