import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import NERO

Item {
    id: speedMode
    anchors.fill: parent

    property bool tractionControlStatus: speedController.tractionControl
    property int packTemp: speedController.packTemp
    property int motorTemp: speedController.motorTemp
    property int timerValue: speedController.currentTime
    property int lastRunTime: speedController.lastTime
    property int fastestRunTime: speedController.fastestTime
    property int maxSpeed: speedController.maxSpeed
    property int currentSpeed: speedController.currentSpeed
    property int maxDraw: speedController.maxCurrent
    property int dcl: speedController.currentDischarge
    property double regen: speedController.regen

    property int xMargin: width / 20
    property int yMargin: height / 20
    property int verticalSpacing: height / 40
    property int borderRadii: 10
    property int valueFontSize: Math.min(height / 8, width / 10)
    property int labelFontSize: Math.min(height / 30, width / 20)
    property int radialUnitFontSize: valueFontSize / 4

    Keys.onPressed: event => {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
            speedController.enterButtonPressed();
        } else if (event.key === Qt.Key_Right) {
            headerController.toggleFaultAlerts();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        HeaderView {
            id: header
            Layout.fillWidth: true
            modeTitle: "PERFORMANCE"
        }

        LabelText {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: -30
            text: speedMode.tractionControlStatus ? "TRACTION CONTROL - ON" : "TRACTION CONTROL - OFF"
            color: speedMode.tractionControlStatus ? Theme.goodStatus : Theme.criticalStatus
            font.pixelSize: 18
            font.bold: true
        }

        TimerDisplay {
            id: timerDisplay
            Layout.fillWidth: true
            Layout.leftMargin: speedMode.xMargin
            Layout.rightMargin: speedMode.xMargin
            Layout.topMargin: speedMode.verticalSpacing
            height: speedMode.height / 12
            currentRunTime: speedMode.timerValue
            lastRunTime: speedMode.lastRunTime
            fastestRunTime: speedMode.fastestRunTime
            radius: speedMode.borderRadii
        }

        RowLayout {
            id: mainRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: speedMode.xMargin
            Layout.rightMargin: speedMode.xMargin
            Layout.bottomMargin: speedMode.yMargin
            Layout.topMargin: speedMode.verticalSpacing
            spacing: speedMode.width / 20

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                spacing: speedMode.height / 20

                ThermometerValueComponent {
                    thermometerValue: speedMode.regen
                    title: "Regen"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    radius: speedMode.borderRadii
                    valueFontSize: speedMode.valueFontSize
                    labelFontSize: speedMode.labelFontSize
                    regen: true
                    unit: "A"
                    unitAnchorBottom: true
                }

                ThermometerValueComponent {
                    thermometerValue: speedMode.motorTemp
                    title: "MOTOR TEMP"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    radius: speedMode.borderRadii
                    valueFontSize: speedMode.valueFontSize
                    labelFontSize: speedMode.labelFontSize
                }

                ThermometerValueComponent {
                    thermometerValue: speedMode.packTemp
                    title: "PACK TEMP"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    radius: speedMode.borderRadii
                    valueFontSize: speedMode.valueFontSize
                    labelFontSize: speedMode.labelFontSize
                }
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                LabelText {
                    color: Theme.accentForeground
                    text: "TOP SPEED"
                    Layout.preferredHeight: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: speedMode.labelFontSize
                }

                Radial {
                    value: speedMode.maxSpeed
                    Layout.preferredHeight: 9
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    color: Theme.accentBlue
                    valueFontSize: speedMode.valueFontSize
                    unitFontSize: speedMode.radialUnitFontSize
                }
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                clip: false

                LabelText {
                    color: Theme.accentForeground
                    text: "MAX DRAW"
                    Layout.preferredHeight: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: speedMode.labelFontSize
                }

                Radial {
                    value: speedMode.maxDraw
                    Layout.preferredHeight: 9
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    maxValue: 400
                    label: "DCL: " + speedMode.dcl
                    unitLabel: 'A'
                    valueFontSize: speedMode.valueFontSize
                    unitFontSize: speedMode.radialUnitFontSize
                }
            }
        }
    }
}
