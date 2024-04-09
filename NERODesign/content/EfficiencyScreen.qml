import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Particles
import QtQuick.Layouts

import NERO

Item {
    id: efficiency
    anchors.fill: parent
    property int torqueLimit: efficiencyController.currentMaxTorque
    property int numRegen: efficiencyController.currentRegenStrength
    property int hvSOC: efficiencyController.stateOfCharge
    property int lvSOC: efficiencyController.stateOfChargeDelta
    property int inverterTemp: efficiencyController.inverterTemp
    property bool motorTemp: efficiencyController.motorTemp
    property bool averageCellTemp: efficiencyController.averageCellTemp

    width: 800
    height: 480

    HeaderView {
        id: header
    }

    ColumnLayout {
        id: thermColumn
        width: parent.width * 0.25
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        ThermometerValueComponent {
            id: motorTempThermometer
            anchors.horizontalCenter: parent.horizontalCenter
            thermometerValue: motorTemp
            Layout.preferredHeight: parent.height / 3
            title: "MOTOR TEMP"
        }

        ThermometerValueComponent {
            id: packTempThermometer
            anchors.horizontalCenter: parent.horizontalCenter
            thermometerValue: averageCellTemp
            Layout.preferredHeight: parent.height / 3
            title: "PACK TEMP"
        }

        ThermometerValueComponent {
            id: regen
            regen: true
            Layout.preferredHeight: parent.height / 3
            anchors.horizontalCenter: parent.horizontalCenter
            thermometerValue: numRegen
            title: "REGEN"
        }
    }

    Column {
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: thermColumn.right
        anchors.bottom: parent.bottom
        id: driveColumn
        width: efficiency.width * 0.5
        height: parent.height

        Rectangle {
            id: onRect
            width: parent.width
            height: parent.width * 0.1
            color: "transparent"
            LabelText {
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

    ColumnLayout {
        id: percentColumn
        width: efficiency.width * 0.5
        anchors.top: header.bottom
        anchors.left: driveColumn.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        TorqueValueComponent {
            id: torqueValue
            torqueValue: 70
            Layout.preferredHeight: parent.height / 4
            anchors.horizontalCenter: parent.horizontalCenter
        }

        BatteryValueComponent {
            id: battery
            title: "HV SOC"
            batteryValue: hvSOC
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.preferredHeight: parent.height / 4
        }

        BatteryValueComponent {
            id: battery2
            title: "LV SOC"
            batteryValue: lvSOC
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.preferredHeight: parent.height / 4
        }
    }
}
