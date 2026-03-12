import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Particles
import QtQuick.Layouts

import NERO

Item {
    id: efficiency
    anchors.fill: parent
    property int torqueLimit: efficiencyController.currentMaxTorque
    property int numRegen: efficiencyController.currentRegenStrength
    property int hvSOC: efficiencyController.stateOfCharge
    property int lvSOC: efficiencyController.lowVoltageStateOfCharge
    property int motorTemp: efficiencyController.motorTemp
    property int packTemp: efficiencyController.packTemp
    property int speed: efficiencyController.speed
    property int timerValue: efficiencyController.currentTime
    property int lastRunTime: efficiencyController.lastTime
    property int fastestRunTime: efficiencyController.fastestTime
    property int xMargin: width / 20
    property int yMargin: height / 20
    property int verticalSpacing: height / 40
    property int borderRadii: 10
    property int valueFontSize: Math.min(height / 8, width / 8)
    property int labelFontSize: Math.min(height / 20, width / 20)

    width: 800
    height: 480

    Keys.onPressed: event => {
                        if (event.key === Qt.Key_Return
                            || event.key === Qt.Key_Enter) {
                            efficiencyController.enterButtonPressed()
                        }
                    }

    HeaderView {
        id: header
    }

    RowLayout {
        id: mainRow
        anchors {
            top: header.bottom
            bottom: parent.bottom
            right: parent.right
            left: parent.left
            rightMargin: efficiency.xMargin
            leftMargin: efficiency.xMargin
            bottomMargin: efficiency.yMargin
        }
        spacing: 20

        ColumnLayout {
            id: thermColumn
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: efficiency.verticalSpacing
            Layout.preferredWidth: 2

            ThermometerValueComponent {
                id: motorTempThermometer
                thermometerValue: efficiency.motorTemp
                title: "MOTOR TEMP"
                Layout.fillHeight: true
                Layout.fillWidth: true
                radius: efficiency.borderRadii
                valueFontSize: efficiency.valueFontSize
                labelFontSize: efficiency.labelFontSize
            }

            ThermometerValueComponent {
                id: packTempThermometer
                thermometerValue: efficiency.packTemp
                title: "PACK TEMP"
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: efficiency.borderRadii
                valueFontSize: efficiency.valueFontSize
                labelFontSize: efficiency.labelFontSize
            }

            ThermometerValueComponent {
                id: regen
                regen: true
                thermometerValue: efficiency.numRegen
                title: "REGEN"
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: efficiency.borderRadii
                valueFontSize: efficiency.valueFontSize
                labelFontSize: efficiency.labelFontSize
                unitFontSize: efficiency.labelFontSize / 2
            }
        }

        ColumnLayout {
            id: driveColumn
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 3

            spacing: efficiency.verticalSpacing

            TimerDisplay {
                id: timerDisplay
                currentRunTime: efficiency.timerValue
                lastRunTime: efficiency.lastRunTime
                fastestRunTime: efficiency.fastestRunTime
                vertical: true
                radius: efficiency.borderRadii

                Layout.preferredHeight: 1
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Radial {
                id: spedometer
                value: efficiency.speed
                valueFontSize: efficiency.valueFontSize

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredHeight: 2
            }
        }

        ColumnLayout {
            id: percentColumn
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: efficiency.verticalSpacing
            Layout.preferredWidth: 2

            BatteryValueComponent {
                id: battery
                title: "HV SOC"
                batteryValue: efficiency.hvSOC
                Layout.fillHeight: true
                Layout.fillWidth: true
                radius: efficiency.borderRadii
                valueFontSize: efficiency.valueFontSize
                labelFontSize: efficiency.labelFontSize
                unitFontSize: efficiency.valueFontSize / 1.5
            }

            TorqueValueComponent {
                id: torqueValue
                torqueValue: efficiency.torqueLimit
                valueFontSize: efficiency.valueFontSize
                labelFontSize: efficiency.labelFontSize
                radius: efficiency.borderRadii

                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
