import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Particles
import QtQuick.Layouts

Item {
    id: efficiency
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
    width: 800
    height: 480


    Row {
        id: statusIndicators
        width: parent.width
        height: parent.height * 0.15

        NonCriticalWarning {
            id: nonCriticalWarning
            height: parent.height
            dimension: parent.height * 0.8
        }

        CriticalFaultIcon {
            id: criticalFaultIcon
            dimension: parent.height * 0.6
            anchors.verticalCenter: parent.verticalCenter
        }

        Rectangle {
            width: statusIndicators.width - criticalFaultIcon.width - nonCriticalWarning.width - microphoneComponent.width
            height: parent.height
            color: "transparent"
        }

        MicrophoneComponent {
            id: microphoneComponent
            width: 60
            height: parent.height * 0.8
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Row {
        id: tempRegen
        width: parent.width * 0.25
        height: parent.height - statusIndicators.height

        ColumnLayout {
            id: thermColumn
            width: parent.width * 0.25
            height: parent.height

            ThermometerValueComponent {
                id: motorTempThermometer
                width: parent.width
                thermometerValue: 50
                title: "MOTOR TEMP"
            }

            ThermometerValueComponent {
                id: packTempThermometer
                width: parent.width
                thermometerValue: 50
                title: "PACK TEMP"
            }

            EffThermometerValueComponent {
                id: regen
                width: parent.width
                thermometerValue: 50
                title: "REGEN"
            }
        }
    }

    Row {
        id: driveMode
        width: parent.width * 0.5
        height: parent.height - statusIndicators.height
        anchors.top: statusIndicators.bottom
        anchors.left: tempRegen.right
        Column{
            id: driveColumn
            width: efficiency.width * 0.5
            height: parent.height

            Rectangle {
                id: onRect
                width: parent.width
                height: parent.width * 0.1
                color: "transparent"
                LabelText{
                    text: "ON"
                    color: "#18ff00"
                    anchors.centerIn: parent
                    font.pixelSize: 50
                }
            }

            Spedometer {
                id: spedometer
                width: parent.width
                anchors.top: onRect.bottom
            }

            DirectionView {
                id: directionView
                width: parent.width * 0.5
                height: parent.height * 0.15
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: spedometer.bottom
            }
        }
    }

    Row {
        id: percentRow
        width: parent.width * 0.25
        height: parent.height - statusIndicators.height
        anchors.left: driveMode.right
        ColumnLayout{
            id: percentColumn
            width: efficiency.width * 0.5
            height: parent.height

            TorqueValueComponent {
                id: torqueValue
            }

            EffBatteryValueComponent{
                id: battery
                title: "HV SOC"
                anchors.right: percentRow.right
            }

            EffBatteryValueComponent{
                id: battery2
                title: "LV SOC"
                anchors.bottom: percentRow.bottom
            }
        }
    }
}




