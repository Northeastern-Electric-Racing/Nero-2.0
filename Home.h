#ifndef HOME_H
#define HOME_H
#include "page.h"
#include "ui_home.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QtWidgets>

class Home : public Page {
  Q_OBJECT

public:
  Home(QFrame *parent, Model *model);

  void updateSpeed(int mph, int kph);
  void updateStatus(bool status);
  void updateDirection(bool direction);
  void updatePackTemp(int packTemp);
  void updateMotorTemp(int motorTemp);
  void updateStateCharge(int stateCharge);

public slots:
  // Slot Logic

private:
  Ui::Home *home_ui;
  QLabel *mphValue;
  QLabel *kphValue;
  QLabel *statusValue;
  QLabel *dirValue;
  QLabel *packTempValue;
  QLabel *motorTempValue;
  QLabel *stateChargeValue;
};

#endif // HOME_H
