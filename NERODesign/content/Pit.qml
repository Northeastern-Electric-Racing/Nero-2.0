import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: pit
    width: 800
    height: 480
    property int stateOfChargePercentage: 0
    property int packTempValue: 0
    property int motorTempValue: 0
    property int currentSpeed: 0
    property int maxSpeed: 5
    property int horizontalMargin: 10
    property bool forward: true

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
                        height: 110
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter

                        Thermometer {
                            id: packTempThermometer
                            width: 50
                            height: 110
                        }

                        Text {
                            text: pit.packTempValue
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            font.family: webFont.name
                            font.pixelSize: 85
                            color: "white"
                        }

                    }

                    Text {
                        text: 'PACK TEMP'
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: packTempRow.bottom
                        font.family: webFont.name
                        font.pixelSize: 24
                        color: "white"
                    }

                }

                Column {
                    id: speedLimitCol
                    width: parent.width / 2
                    height: parent.height

                    Text {
                        id: maxSpeedText
                        text: pit.maxSpeed
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: 5
                        font.family: webFont.name
                        font.pixelSize: 85
                        color: "white"
                    }

                    Text {
                        text: "SPEED LIMIT"
                        anchors.top: maxSpeedText.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: webFont.name
                        color: "white"
                        font.pixelSize: 24
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
                        height: 110
                        width: parent.width

                        Battery {
                            id: battery
                            width: 50
                            height: 110
                            value: pit.stateOfChargePercentage
                        }


                        Text {
                            text: pit.stateOfChargePercentage
                            anchors.centerIn: parent
                            font.family: webFont.name
                            font.pixelSize: 85
                            color: "white"
                        }

                    }

                    Text {
                        text: "CHARGE STATE"
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: webFont.name
                        font.pixelSize: 24
                        color: "white"
                    }
                }

                Column {
                    id: motorTempColumn
                    height: parent.height
                    width: parent.width / 2

                    Row {
                        id: motorTempRow
                        width: parent.width
                        height: 110

                        Thermometer {
                            id: motorTempThermometer
                            value: pit.motorTempValue
                            width: 50
                            height: 110
                        }

                        Text {
                            text: pit.motorTempValue
                            anchors.centerIn: parent
                            font.family: webFont.name
                            font.pixelSize: 85
                            color: "white"
                        }

                    }

                    Text {
                        text: "MOTOR TEMP"
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: webFont.name
                        color: "white"
                        font.pixelSize: 24
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
