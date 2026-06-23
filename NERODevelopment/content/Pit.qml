import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import NERO

Rectangle {
    id: pit
    anchors.fill: parent
    property int stateOfChargePercentage: homeController.stateOfCharge
    property real lvVoltage: homeController.lowVoltage
    property int packTempValue: homeController.packTemp
    property int motorTempValue: homeController.motorTemp
    property int currentSpeed: homeController.speed
    property bool forward: homeController.direction

    property int maxSpeed: 5
    property int horizontalMargin: parent.width / 40
    property int componentRadii: 20
    property int bottomMargin: parent.height / 10
    property int valueFontSize: Math.min(height / 7.5, width / 7.5)
    property int labelFontSize: Math.min(height / 20, width / 20)
    property int unitFontSize: valueFontSize / 1.5

    color: Theme.background
    height: 480
    width: 800

    Keys.onPressed: event => {
        if (event.key === Qt.Key_Right) {
            headerController.toggleFaultAlerts();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        HeaderView {
            Layout.fillWidth: true
            height: 100
            modeTitle: pit.forward ? "PIT - DRIVE" : "PIT - KEVIN"
        }

        RowLayout {
            id: mainRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: pit.horizontalMargin
            Layout.rightMargin: pit.horizontalMargin
            Layout.bottomMargin: pit.bottomMargin

            spacing: pit.width / 40

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 5
                spacing: parent.height / 20

                ThermometerValueComponent {
                    thermometerValue: pit.motorTempValue
                    title: 'MOTOR TEMP'
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: pit.componentRadii
                    valueFontSize: pit.valueFontSize
                    labelFontSize: pit.labelFontSize
                }

                ThermometerValueComponent {
                    thermometerValue: pit.packTempValue
                    title: 'PACK TEMP'
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: pit.componentRadii
                    valueFontSize: pit.valueFontSize
                    labelFontSize: pit.labelFontSize
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 6
                spacing: parent.height / 20

                Radial {
                    value: pit.currentSpeed
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 3
                    valueFontSize: pit.valueFontSize
                    maxValue: pit.maxSpeed
                }

                DirectionView {
                    forward: pit.forward
                    radius: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 1
                    Layout.leftMargin: 20
                    Layout.rightMargin: 20
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 5
                spacing: parent.height / 20

                BatteryValueComponent {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    batteryValue: pit.stateOfChargePercentage
                    title: "HV SOC"
                    radius: pit.componentRadii
                    valueFontSize: pit.valueFontSize
                    labelFontSize: pit.labelFontSize
                    unitFontSize: pit.unitFontSize
                }

                ThermometerValueComponent {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    thermometerValue: pit.lvVoltage
                    title: "LV"
                    unit: "V"
                    radius: pit.componentRadii
                    valueFontSize: pit.valueFontSize
                    labelFontSize: pit.labelFontSize
                    unitFontSize: pit.unitFontSize
                }
            }
        }
    }
}
