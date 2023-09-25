#ifndef HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <QObject>

class homeController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
    Q_PROPERTY(boolean status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(boolean direction READ direction WRITE setDirection NOTIFY directionChanged FINAL)
    Q_PROPERTY(int packTemp READ packTemp WRITE setPackTemp NOTIFY packTempChanged FINAL)
    Q_PROPERTY(int motorTemp READ motorTemp WRITE setMotorTemp NOTIFY motorTempChanged FINAL)

public:
    explicit homeController(QObject *parent = nullptr);
    int speed() const;
    bool status() const;
    bool direction() const;
    int packTemp() const;
    int motorTemp() const;

signals:
    void speedChanged(int);
    void statusChanged(bool);
    void directionChanged(bool);
    void packTempChanged(int);
    void motorTempChanged(int);

public slots:
    void setSpeed(int);
    void setStatus(bool);
    void setDirection(bool);
    void setPackTemp(int);
    void setMotorTemp(int);

private:
    int m_speed;
    bool m_status;
    bool m_direction;
    int m_packTemp;
    int m_motorTemp;
};

#endif // HOMECONTROLLER_H
