import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import NERO

Rectangle {
    id: pit
    anchors.fill: parent
    property int stateOfChargePercentage: homeController.stateOfCharge
    property int lvStateOfChargePercentage: homeController.lowVoltageStateOfCharge
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

    color: Theme.getColor("background")
    height: 480
    width: 800

    HeaderView {
        id: headerView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
    }

    RowLayout {
        id: mainRow
        anchors {
            leftMargin: pit.horizontalMargin
            rightMargin: pit.horizontalMargin
            topMargin: 0
            bottomMargin: pit.bottomMargin
            top: headerView.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        spacing: parent.width / 40

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

            BatteryValueComponent {
                Layout.fillWidth: true
                Layout.fillHeight: true

                batteryValue: pit.lvStateOfChargePercentage
                title: "LV SOC"
                radius: pit.componentRadii
                valueFontSize: pit.valueFontSize
                labelFontSize: pit.labelFontSize
                unitFontSize: pit.unitFontSize
            }
        }
    }
}
