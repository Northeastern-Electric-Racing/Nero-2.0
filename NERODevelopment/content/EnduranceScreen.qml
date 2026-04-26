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
    property int motorTemp: enduranceController.motorTemp
    property int packTemp: enduranceController.packTemp
    property int speed: enduranceController.speed
    property int timerValue: enduranceController.currentTime
    property int lastRunTime: enduranceController.lastTime
    property int fastestRunTime: enduranceController.fastestTime
    property int powerDrawPercent: enduranceController.powerDrawPercent
    property int xMargin: width / 20
    property int yMargin: height / 20
    property int verticalSpacing: height / 40
    property int borderRadii: 10
    property int valueFontSize: Math.min(height / 8, width / 8)
    property int labelFontSize: Math.min(height / 20, width / 20)
    property int columnUnit: (width - xMargin - 2 * 20) / 7
    property int sideColumnWidth: 2 * columnUnit
    property int centerColumnWidth: 3 * columnUnit

    width: 800
    height: 480

    Keys.onPressed: event => {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
            enduranceController.enterButtonPressed();
        } else if (event.key === Qt.Key_Right) {
            headerController.toggleFaultAlerts();
        }
    }

    GridLayout {
        anchors.fill: parent
        columns: 3
        rowSpacing: 0
        columnSpacing: 20

        HeaderView {
            id: header
            Layout.row: 0
            Layout.column: 0
            Layout.columnSpan: 2
            Layout.fillWidth: true
            modeTitle: "ENDURANCE"
            titleHorizontalCenterOffset: (endurance.width - header.width) / 2
        }

        ColumnLayout {
            id: thermColumn
            Layout.row: 1
            Layout.column: 0
            Layout.leftMargin: endurance.xMargin / 2
            Layout.bottomMargin: endurance.yMargin
            Layout.fillHeight: true
            Layout.preferredWidth: endurance.sideColumnWidth
            Layout.minimumWidth: endurance.sideColumnWidth
            Layout.maximumWidth: endurance.sideColumnWidth
            spacing: endurance.verticalSpacing

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
                unit: "A"
                unitAnchorBottom: true
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
            Layout.row: 1
            Layout.column: 1
            Layout.bottomMargin: endurance.yMargin
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: endurance.centerColumnWidth
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
            Layout.row: 0
            Layout.column: 2
            Layout.rowSpan: 2
            Layout.topMargin: endurance.yMargin
            Layout.rightMargin: endurance.xMargin / 2
            Layout.bottomMargin: endurance.yMargin
            Layout.fillHeight: true
            Layout.preferredWidth: endurance.sideColumnWidth
            Layout.minimumWidth: endurance.sideColumnWidth
            Layout.maximumWidth: endurance.sideColumnWidth
            spacing: endurance.verticalSpacing

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

            TorqueValueComponent {
                id: torqueValue
                torqueValue: endurance.torqueLimit
                valueFontSize: endurance.valueFontSize
                labelFontSize: endurance.labelFontSize
                radius: endurance.borderRadii

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            BatteryValueComponent {
                id: powerDraw
                title: "PWR DRAW"
                batteryValue: endurance.powerDrawPercent
                Layout.fillHeight: true
                Layout.fillWidth: true
                radius: endurance.borderRadii
                valueFontSize: endurance.valueFontSize
                labelFontSize: endurance.labelFontSize
                unitFontSize: endurance.valueFontSize / 1.5
            }
        }
    }
}
