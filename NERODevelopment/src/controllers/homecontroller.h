#ifndef HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include "../models/model.h"
#include "buttoncontroller.h"
#include <QObject>

/**
 * @brief The HomeController class
 * Controller for the home screen, aka pit and reverse.
 */
class HomeController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
  Q_PROPERTY(bool status READ status WRITE setStatus NOTIFY statusChanged FINAL)
  Q_PROPERTY(bool direction READ direction WRITE setDirection NOTIFY
                 directionChanged FINAL)
  Q_PROPERTY(float packTemp READ packTemp WRITE setPackTemp NOTIFY
                 packTempChanged FINAL)
  Q_PROPERTY(float motorTemp READ motorTemp WRITE setMotorTemp NOTIFY
                 motorTempChanged FINAL)
  Q_PROPERTY(float stateOfCharge READ stateOfCharge WRITE setStateOfCharge
                 NOTIFY stateOfChargeChanged FINAL)
  Q_PROPERTY(bool dialogVisible READ dialogVisible WRITE setDialogVisible NOTIFY
                 dialogVisibleChanged FINAL)

public:
  explicit HomeController(Model *model, QObject *parent = nullptr);
  int speed() const;
  bool status() const;
  bool direction() const;
  float packTemp() const;
  float motorTemp() const;
  float stateOfCharge() const;
  bool dialogVisible() const;

signals:
  void speedChanged(int);
  void statusChanged(bool);
  void directionChanged(bool);
  void packTempChanged(float);
  void motorTempChanged(float);
  void stateOfChargeChanged(float);
  void dialogVisibleChanged();

public slots:
  void setSpeed(int);
  void setStatus(bool);
  void setDirection(bool);
  void setPackTemp(float);
  void setMotorTemp(float);
  void setStateOfCharge(float);
  void currentDataDidChange();
  void setDialogVisible(bool);
  void enterButtonPressed();

private:
  int m_speed;
  bool m_status;
  bool m_direction;
  float m_packTemp;
  float m_motorTemp;
  float m_stateOfCharge;
  bool m_dialogVisible;
};

#endif // HOMECONTROLLER_H
