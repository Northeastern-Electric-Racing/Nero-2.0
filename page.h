#ifndef PAGE_H
#define PAGE_H

#include "models/model.h"
#include <QFrame>

class Page : public QFrame {
  Q_OBJECT

public:
  Page(QFrame *parent, Model *model, const QString &name);
  virtual ~Page() {}

  void update();
  void enterButtonPressed();
  void upButtonPressed();
  void downButtonPressed();

protected:
  Model *model;
  QString name;
  int height;
  int width;
};

#endif // PAGE_H
