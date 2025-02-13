import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import NERO

Rectangle {
    id: pit
    anchors.fill: parent
    property int stateOfChargePercentage: homeController.stateOfCharge
    property int packTempValue: homeController.packTemp
    property int motorTempValue: homeController.motorTemp
    property int currentSpeed: homeController.speed
    property bool forward: homeController.status
    property int hvSOC: efficiencyController.stateOfCharge
    property int lvSOC: efficiencyController.lowVoltageStateOfCharge

    property int maxSpeed: 5
    property int horizontalMargin: 10
    property int horizontalIconSpacing: -55
    property int iconWidth: 40
    property int iconHeight: 90
    property int labelVerticalSpacing: 10
    color: 'black'
    height: 480
    width: 800

    Rectangle {
        id: mainRow
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 8
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        color: "transparent"

        Rectangle {
            id: coreInfoLeft
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.bottomMargin: 20
            anchors.bottom: parent.bottom
            width: parent.width / 2
            color: "transparent"

            Rectangle {
                id: topLeft
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                height: parent.height / 2
                color: "transparent"

                ThermometerValueComponent {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: parent.width / 1.75
                    radius: 25
                    thermometerValue: pit.motorTempValue
                    title: "MOTOR TEMP"
                }
            }

            Rectangle {
                id: bottomLeft
                anchors.top: topLeft.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                color: "transparent"

                ThermometerValueComponent {
                    id: packTempThermometer
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width / 1.75
                    radius: 25
                    //thermometerColor: "blue"
                    thermometerValue: pit.packTempValue
                    title: "PACK TEMP"
                }
            }
        }

        Rectangle {
            id: coreInfoRight
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.bottomMargin: 20
            anchors.bottom: parent.bottom
            width: parent.width / 2
            color: "transparent"

            Rectangle {
                id: topRight
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                height: parent.height / 2
                color: "transparent"

                BatteryValueComponent {
                    id: hvSocBattery
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    width: parent.width / 1.75
                    radius: 25
                    batteryValue: hvSOC
                    title: "HV SOC"
                }
            }

            Rectangle {
                id: bottomRight
                anchors.top: topRight.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                color: "transparent"

                BatteryValueComponent {
                    id: lvSocBattery
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    width: parent.width / 1.75
                    radius: 25
                    batteryValue: lvSOC
                    title: "LV SOC"
                }
            }
        }

        ColumnLayout {
            id: speedometerColumn
            height: parent.height - 30
            anchors.centerIn: mainRow

            Radial {
                id: speedometer
                Layout.preferredWidth: 350
                Layout.preferredHeight: 250
                value: pit.currentSpeed
                maxValue: pit.maxSpeed
                valueFontSize: width / 5
                verticalPadding: 75
                anchors.centerIn: parent
            }

            DirectionView {
                id: directionView
                forward: forward
                Layout.preferredWidth: 200
                Layout.preferredHeight: 75
                radius: 10
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    HeaderView {
        id: headerView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 75

        RowLayout {
                anchors.centerIn: parent
                spacing: 10

                Text {
                    text: "CAR ON"
                    color: "#00FF00"
                    font {
                        pixelSize: 36
                        bold: true
                    }
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    text: "-"
                    color: "white"
                    font {
                        pixelSize: 36
                        bold: true
                    }
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    text: "GLVMS OFF"
                    color: "#FF0000"
                    font {
                        pixelSize: 36
                        bold: true
                    }
                    Layout.alignment: Qt.AlignVCenter
                }
            }
    }
}
