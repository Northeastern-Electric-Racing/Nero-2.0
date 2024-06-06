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
    property int lvSOC: efficiencyController.lowVoltageStateOfCharge
    property int motorTemp: efficiencyController.motorTemp
    property int packTemp: efficiencyController.packTemp
    property int speed: efficiencyController.speed
    property int timerValue: efficiencyController.currentTime
    property int lastRunTime: efficiencyController.lastTime
    property int fastestRunTime: efficiencyController.fastestTime

    width: 800
    height: 480

    Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                efficiencyController.enterButtonPressed();
            }
        }


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
            thermometerValue: efficiency.motorTemp
            Layout.preferredHeight: parent.height / 3
            title: "MOTOR TEMP"
        }

        ThermometerValueComponent {
            id: packTempThermometer
            anchors.horizontalCenter: parent.horizontalCenter
            thermometerValue: efficiency.packTemp
            Layout.preferredHeight: parent.height / 3
            title: "PACK TEMP"
        }

        ThermometerValueComponent {
            id: regen
            regen: true
            Layout.preferredHeight: parent.height / 3
            anchors.horizontalCenter: parent.horizontalCenter
            thermometerValue: efficiency.numRegen
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

        TimerDisplay {
            id: timerDisplay
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            width: efficiency.width * 0.4
            height: efficiency.height * 0.3
            currentRunTime: efficiency.timerValue
            lastRunTime: efficiency.lastRunTime
            fastestRunTime: efficiency.fastestRunTime
        }

        Spedometer {
            id: spedometer
            width: parent.width
            value: efficiency.speed
            anchors.top: onRect.bottom
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
            torqueValue: efficiency.torqueLimit
            Layout.preferredHeight: parent.height / 4
            anchors.horizontalCenter: parent.horizontalCenter
        }

        BatteryValueComponent {
            id: battery
            title: "HV SOC"
            batteryValue: efficiency.hvSOC
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.preferredHeight: parent.height / 4
        }

        BatteryValueComponent {
            id: battery2
            title: "LV SOC"
            batteryValue: efficiency.lvSOC
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.preferredHeight: parent.height / 4
        }
    }
}
