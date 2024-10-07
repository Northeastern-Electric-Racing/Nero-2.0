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
    property bool dialog: homeController.dialogVisible

    property int maxSpeed: 5
    property int horizontalMargin: 10
    property int horizontalIconSpacing: -55
    property int iconWidth: 40
    property int iconHeight: 90
    property int labelVerticalSpacing: 10
    color: 'black'
    height: 480
    width: 800

    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
            homeController.enterButtonPressed();
        }
    }

    Rectangle {
        id: mainRow
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 8
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        color: "transparent"

        Rectangle {
            id: coreInfo
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.bottomMargin: 20
            anchors.bottom: parent.bottom
            width: parent.width / 2
            color: "transparent"

            Rectangle {
                id: topRow
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                height: parent.height / 2
                color: "transparent"

                ThermometerValueComponent {
                    id: packTempThermometer
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width / 2

                    thermometerValue: pit.packTempValue
                    title: "PACK TEMP"
                }

                ColumnLayout {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.bottomMargin: 19
                    anchors.topMargin: 18
                    width: parent.width / 2

                    ValueText {
                        text: pit.maxSpeed
                        Layout.alignment: Qt.AlignHCenter
                        font.pixelSize: 0.6 * parent.height
                    }

                    LabelText {
                        text: "SPEED LIMIT"
                        Layout.alignment: Qt.AlignHCenter
                        font.pixelSize: 0.15 * parent.width
                    }
                }
            }
            Rectangle {
                id: bottomRow
                anchors.top: topRow.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                color: "transparent"

                BatteryValueComponent {
                    id: battery
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 22

                    width: parent.width / 2
                    title: "PACK SOC"
                    batteryValue: pit.stateOfChargePercentage
                }

                ThermometerValueComponent {
                    anchors.top: parent.top
                    anchors.left: battery.right
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    thermometerValue: pit.motorTempValue
                    title: "MOTOR TEMP"
                }
            }
        }

        ColumnLayout {
            id: spedometerColumn
            anchors.left: coreInfo.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

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

    onDialogChanged: {
        if (pit.dialog) {
            dialog.openModal();
        } else {
            dialog.closeModal();
        }
    }

    Lily {
        id: dialog
    }

    HeaderView {
        id: headerView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
    }
}
