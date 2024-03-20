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

    property int maxSpeed: 5
    property int horizontalMargin: 10
    property int horizontalIconSpacing: -55
    property int iconWidth: 40
    property int iconHeight: 90
    property int labelVerticalSpacing: 10
    color: 'black'
    height: 480
    width: 800

    RowLayout {
        id: mainRow
        anchors.fill: parent
        anchors.leftMargin: horizontalMargin
        anchors.rightMargin: horizontalMargin

        Rectangle {
            id: coreInfo
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width / 2

            ThermometerValueComponent {
                id: packTempThermometer
                width: 100
                height: 100
                thermometerValue: pit.packTempValue
                title: "PACK TEMP"
            }

            ColumnLayout {
                ValueText {
                    text: pit.maxSpeed
                    Layout.alignment: Qt.AlignHCenter
                }

                LabelText {
                    text: "SPEED LIMIT"
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            BatteryValueComponent {
                width: 100
                height: 100
                Layout.alignment: Qt.AlignTop
                batteryValue: pit.stateOfChargePercentage
            }

            ThermometerValueComponent {
                width: 100
                height: 100
                thermometerValue: pit.motorTempValue
                title: "MOTOR TEMP"
            }
        }

        ColumnLayout {
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.width / 2

            Spedometer {
                id: spedometer
                Layout.preferredWidth: 400
                Layout.preferredHeight: 300
                value: pit.currentSpeed
                maxValue: pit.maxSpeed
                verticalPadding: 50
                Layout.alignment: Qt.AlignHCenter
            }

            DirectionView {
                id: directionView
                forward: forward
                Layout.preferredWidth: 300
                Layout.preferredHeight: 100
                radius: 10
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }

    HeaderView {
        id: headerView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
    }
}
