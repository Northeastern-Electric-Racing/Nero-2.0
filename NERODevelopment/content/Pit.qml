import QtQuick 2.15
import QtQuick.Controls 2.15
import NERO

Item {
    id: pit

    width: parent.width
    height: parent.height
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

    Row {
        id: mainRow
        width: parent.width - horizontalMargin * 2
        height: parent.height
        anchors.centerIn: parent

        Column {
            id: leftCol
            spacing: 10
            width: parent.width / 2
            height: parent.height

            Row {
                id: topRow
                width: parent.width
                height: parent.height / 2

                Column {
                    id: packTempColumn
                    width: parent.width / 2
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter

                    Row {
                        id: packTempRow
                        height: pit.iconHeight
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter

                        Thermometer {
                            id: packTempThermometer
                            value: pit.packTempValue
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: pit.horizontalIconSpacing
                            width: pit.iconWidth
                            height: pit.iconHeight
                        }

                        ValueText {
                            id: packTempText
                            text: pit.packTempValue
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: 25
                            anchors.baseline: parent.bottom
                        }

                    }

                    LabelText {
                        text: 'PACK TEMP'
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: packTempRow.bottom
                        anchors.topMargin: pit.labelVerticalSpacing
                    }

                }

                Column {
                    id: speedLimitCol
                    width: parent.width / 2
                    height: parent.height

                    ValueText {
                        id: maxSpeedText
                        text: pit.maxSpeed
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: 15
                    }

                    LabelText {
                        text: "SPEED LIMIT"
                        anchors.top: maxSpeedText.bottom
                        anchors.topMargin: -pit.labelVerticalSpacing
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

            }

            Row {
                id: bottomRow
                height: parent.height / 2
                width: parent.width


                Column {
                    id: batteryColumn
                    width: parent.width / 2
                    height: parent.height

                    Row {
                        id: batteryRow
                        height: pit.iconHeight
                        width: parent.width

                        Battery {
                            id: battery
                            width: pit.iconWidth
                            height: pit.iconHeight
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: pit.horizontalIconSpacing
                            value: pit.stateOfChargePercentage
                        }


                        ValueText {
                            text: pit.stateOfChargePercentage
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: 25
                            anchors.baseline: parent.bottom
                        }

                    }

                    LabelText {
                        text: "CHARGE STATE"
                        anchors.top: batteryRow.bottom
                        anchors.topMargin: pit.labelVerticalSpacing
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                Column {
                    id: motorTempColumn
                    height: parent.height
                    width: parent.width / 2

                    Row {
                        id: motorTempRow
                        width: parent.width
                        height: pit.iconHeight

                        Thermometer {
                            id: motorTempThermometer
                            value: pit.motorTempValue
                            width: pit.iconWidth
                            height: pit.iconHeight
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: pit.horizontalIconSpacing
                        }

                        ValueText {
                            text: pit.motorTempValue
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: 25
                            anchors.baseline: parent.bottom
                        }

                    }

                    LabelText {
                        text: "MOTOR TEMP"
                        anchors.top: motorTempRow.bottom
                        anchors.topMargin: pit.labelVerticalSpacing
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

            }
        }

        Column {
            id: rightColumn
            anchors.verticalCenter: parent.verticalCenter

            Spedometer {
                id: spedometer
                width: 400
                height: 300
                value: pit.currentSpeed
                maxValue: pit.maxSpeed
                anchors.horizontalCenter: parent.horizontalCenter
                verticalPadding: 50
            }

            DirectionView {
                id: directionView
                forward: forward
                anchors.horizontalCenter: parent.horizontalCenter
                width: 300
                height: 100
                radius: 10
            }
        }

    }


}
