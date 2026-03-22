import QtQuick
import QtQuick.Controls
import QtQuick.Particles
import QtQuick.Layouts

import NERO

Item {
    id: endurance
    anchors.fill: parent
    property int torqueLimit: enduranceController.currentMaxTorque
    property int numRegen: enduranceController.currentRegenStrength
    property int hvSOC: enduranceController.stateOfCharge
    property int lvSOC: enduranceController.lowVoltageStateOfCharge
    property int motorTemp: enduranceController.motorTemp
    property int packTemp: enduranceController.packTemp
    property int speed: enduranceController.speed
    property int timerValue: enduranceController.currentTime
    property int lastRunTime: enduranceController.lastTime
    property int fastestRunTime: enduranceController.fastestTime
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
                            enduranceController.enterButtonPressed()
                        }
                    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        HeaderView {
            id: header
            Layout.fillWidth: true
            modeTitle: "ENDURANCE"
        }

        RowLayout {
            id: mainRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: endurance.xMargin
            Layout.rightMargin: endurance.xMargin
            Layout.bottomMargin: endurance.yMargin
            spacing: 20

            ColumnLayout {
                id: thermColumn
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: endurance.verticalSpacing
                Layout.preferredWidth: 2

                ThermometerValueComponent {
                    id: motorTempThermometer
                    thermometerValue: endurance.motorTemp
                    title: "MOTOR TEMP"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    radius: endurance.borderRadii
                    valueFontSize: endurance.valueFontSize
                    labelFontSize: endurance.labelFontSize
                }

                ThermometerValueComponent {
                    id: packTempThermometer
                    thermometerValue: endurance.packTemp
                    title: "PACK TEMP"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: endurance.borderRadii
                    valueFontSize: endurance.valueFontSize
                    labelFontSize: endurance.labelFontSize
                }

                ThermometerValueComponent {
                    id: regen
                    regen: true
                    thermometerValue: endurance.numRegen
                    title: "REGEN"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: endurance.borderRadii
                    valueFontSize: endurance.valueFontSize
                    labelFontSize: endurance.labelFontSize
                    unitFontSize: endurance.labelFontSize / 2
                }
            }

            ColumnLayout {
                id: driveColumn
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 3

                spacing: endurance.verticalSpacing

                TimerDisplay {
                    id: timerDisplay
                    currentRunTime: endurance.timerValue
                    lastRunTime: endurance.lastRunTime
                    fastestRunTime: endurance.fastestRunTime
                    vertical: true
                    radius: endurance.borderRadii

                    Layout.preferredHeight: 1
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Radial {
                    id: spedometer
                    value: endurance.speed
                    valueFontSize: endurance.valueFontSize

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 2
                }
            }

            ColumnLayout {
                id: percentColumn
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: endurance.verticalSpacing
                Layout.preferredWidth: 2

                BatteryValueComponent {
                    id: battery
                    title: "HV SOC"
                    batteryValue: endurance.hvSOC
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    radius: endurance.borderRadii
                    valueFontSize: endurance.valueFontSize
                    labelFontSize: endurance.labelFontSize
                    unitFontSize: endurance.valueFontSize / 1.5
                }

                BatteryValueComponent {
                    id: battery2
                    title: "LV SOC"
                    batteryValue: endurance.lvSOC
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: endurance.borderRadii
                    valueFontSize: endurance.valueFontSize
                    labelFontSize: endurance.labelFontSize
                    unitFontSize: endurance.valueFontSize / 1.5
                }

                TorqueValueComponent {
                    id: torqueValue
                    torqueValue: endurance.torqueLimit
                    valueFontSize: endurance.valueFontSize
                    labelFontSize: endurance.labelFontSize
                    radius: endurance.borderRadii

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
