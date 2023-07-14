#include "home.h"

Home::Home(QFrame* parent, Model* model) : Page(parent, model, "Home")
{
    // Create and configure the UI elements
    mphValue = new QLabel("N/A", this);
    mphValue->setObjectName("MphValue");
    mphValue->setStyleSheet("font-size: 150pt; font-weight: bold");
    mphValue->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    kphValue = new QLabel("N/A", this);
    kphValue->setObjectName("KphValue");
    kphValue->setStyleSheet("font-size: 20pt");

    statusValue = new QLabel("N/A", this);
    statusValue->setObjectName("StatusValue");
    statusValue->setStyleSheet("font-size: 100pt; font-weight: bold");

    dirValue = new QLabel("N/A", this);
    dirValue->setObjectName("DirValue");
    dirValue->setStyleSheet("font-size: 75pt; font-weight: bold");

    packTempValue = new QLabel("N/A", this);
    packTempValue->setObjectName("PackTempValue");
    packTempValue->setStyleSheet("font-size: 150pt; font-weight: bold");
    packTempValue->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    motorTempValue = new QLabel("N/A", this);
    motorTempValue->setObjectName("MotorTempValue");
    motorTempValue->setStyleSheet("font-size: 150pt; font-weight: bold");
    motorTempValue->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    stateChargeValue = new QLabel("N/A", this);
    stateChargeValue->setObjectName("StateChargeValue");
    stateChargeValue->setStyleSheet("font-size: 150pt; font-weight: bold");
    stateChargeValue->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    // Create the layout and add the UI elements
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(mphValue);
    layout->addWidget(kphValue);
    layout->addWidget(statusValue);
    layout->addWidget(dirValue);
    layout->addWidget(packTempValue);
    layout->addWidget(motorTempValue);
    layout->addWidget(stateChargeValue);

    setLayout(layout);
}

void Home::updateSpeed(int mph, int kph)
{
    QString mphText = (mph != -1) ? QString::number(mph) : "N/A";
    QString kphText = (kph != -1) ? QString::number(kph) : "N/A";
    mphValue->setText(mphText);
    kphValue->setText(kphText);
}

void Home::updateStatus(bool status)
{
    statusValue->setText(status ? "ON" : "OFF");
    statusValue->setStyleSheet(status ? "color: green; font-size: 100pt; font-weight: bold" : "color: red; font-size: 100pt; font-weight: bold");
}

void Home::updateDirection(bool direction)
{
    dirValue->setText(direction ? "FORWARD" : "REVERSE");
}

void Home::updatePackTemp(int packTemp)
{
    QString packTempText = (packTemp != -1) ? (QString::number(packTemp) + "°") : "N/A";
                                                                                  packTempValue->setText(packTempText);
}

void Home::updateMotorTemp(int motorTemp)
{
    QString motorTempText = (motorTemp != -1) ? (QString::number(motorTemp) + "°") : "N/A";
                                                                                     motorTempValue->setText(motorTempText);
}

void Home::updateStateCharge(int stateCharge)
{
    QString stateChargeText = (stateCharge != -1) ? (QString::number(stateCharge) + "%") : "N/A";
    stateChargeValue->setText(stateChargeText);
}
