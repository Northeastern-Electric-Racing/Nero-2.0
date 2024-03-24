#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#include <QObject>
#include "../models/model.h"
#include "buttoncontroller.h"

class SpeedController : public ButtonController {
    Q_OBJECT
    Q_PROPERTY(bool tractionControl READ tractionControlStatus WRITE setTractionControl
               NOTIFY tractionControlChanged);

    Q_PROPERTY(float packTemp READ packTemp WRITE setPackTemp NOTIFY packTempChanged);

    Q_PROPERTY(float motorTemp READ motorTemp WRITE setMotorTemp NOTIFY motorTempChanged);

    Q_PROPERTY(float chargeState READ chargeState WRITE setChargeState NOTIFY chargeStateChanged);

    Q_PROPERTY(float currentTime READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged);

    Q_PROPERTY(float fastestTime READ fastestTime WRITE setFastestTime NOTIFY fastestTimeChanged);

    Q_PROPERTY(float lastTime READ lastTime WRITE setLastTime NOTIFY lastTimeChanged);

    Q_PROPERTY(float currentSpeed READ currentSpeed WRITE setCurrentSpeed NOTIFY currentSpeedChanged);

    Q_PROPERTY(float maxSpeed READ maxSpeed WRITE setMaxSpeed NOTIFY maxSpeedChanged);

    Q_PROPERTY(float current READ current WRITE setCurrent NOTIFY currentChanged);

    Q_PROPERTY(float maxCurrent READ maxCurrent WRITE setMaxCurrent NOTIFY maxCurrentChanged);

    Q_PROPERTY(float currentDischarge READ currentDischarge WRITE setCurrentDischarge NOTIFY currentDischargeChanged);

    Q_PROPERTY(float maxCurrentDischarge READ maxCurrentDischarge WRITE setMaxCurrentDischarge
               NOTIFY maxcurrentDischargeChanged);




public:
    explicit SpeedController(Model *model ,QObject *parent = nullptr);

    bool tractionControl() const;
    float packTemp() const;
    float motorTemp() const;
    float chargeState() const;
    float currentTime() const;
    float fastestTime() const;
    float lastTime() const;
    float currentSpeed() const;
    float maxSpeed() const;
    float current() const;
    float maxCurrent() const;
    float currentDischarge() const;
    float maxCurrentDischarge() const;

signals:
    void tractionControlChanged(bool);
    void packTempChanged(float);
    void motorTempChanged(float);
    void chargeStateChanged(float);
    void currentTimeChanged(float);
    void fastestTimeChanged(float);
    void lastTimeChanged(float);
    void currentSpeedChanged(float);
    void maxSpeedChanged(float);
    void currentChanged(float);
    void maxCurrentChanged(float);
    void currentDischargeChanged(float);
    void maxCurrentDischargeChanged(float);

public slots:
    void setTractionControl(bool);
    void setPackTemp(float);
    void setMotorTemp(float);
    void setChargeState(float);
    void setCurrentTime(float);
    void setFastestTime(float);
    void setLastTime(float);
    void setCurrentSpeed(float);
    void setMaxSpeed(float);
    void setCurrent(float);
    void setMaxCurrent(float);
    void setCurrentDischarge(float);
    void setMaxCurrentDischarge(float);

    void update();

private:
    bool m_tractionControl;
    float m_packTemp;
    float m_motorTemp;
    float m_chargeState;
    float m_currentTime;
    float m_fastestTime;
    float m_lastTime;
    float m_currentSpeed;
    float m_maxSpeed;
    float m_current;
    float m_maxCurrent;
    float m_currentDischarge;
    float m_maxCurrentDischarge;

};

#endif // SPEEDCONTROLLER_H
