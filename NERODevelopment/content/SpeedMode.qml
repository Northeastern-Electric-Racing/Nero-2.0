import QtQuick 2.15
import QtQuick.Controls 2.15

import NERO

Item {
    id: speedMode
    anchors.fill: parent

    property bool tractionControlStatus: speedController.tractionControl
    property int packTemp: speedController.packTemp
    property int motorTemp: speedController.motorTemp
    property int batteryValue: speedController.chargeState
    property int timerValue: speedController.currentTime
    property int lastRunTime: speedController.lastTime
    property int fastestRunTime: speedController.fastestTime
    property int maxSpeed: speedController.maxSpeed
    property int currentSpeed: speedController.currentSpeed
    property int currentDraw: speedController.current

    Keys.onPressed: {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                speedController.enterButtonPressed();
            }
        }

    HeaderView {
        id: headerView
        anchors {
            top: speedMode.top
            horizontalCenter: speedMode.horizontalCenter
        }
        width: speedMode.width
        height: speedMode.height * 0.15
    }

    LabelText {
        id: labelText
        anchors {
            top: speedMode.top
            horizontalCenter: speedMode.horizontalCenter
        }
        width: speedMode.width * 0.7
        height: speedMode.height * 0.12
        text: speedMode.tractionControlStatus ? "TRACTION CONTROL - ON" : "TRACTION CONTROL - OFF"
        color: "#14ff00"
        font.pixelSize: Math.min(speedMode.height * 0.09,
                                 speedMode.width * 0.06)
        font.bold: true
    }

    LabelText {
        id: vehicleInfo
        anchors {
            bottom: packTempComponent.top
            left: speedMode.left
        }
        width: speedMode.width * 0.25
        height: speedMode.height * 0.1
        text: "VEHICLE INFO"
        color: "white"
        font.pixelSize: Math.min(speedMode.height * 0.06,
                                 speedMode.width * 0.035)
        font.bold: true
    }

    LabelText {
        id: performanceInfo
        anchors {
            bottom: packTempComponent.top
            horizontalCenter: timerDisplay.right
        }
        width: speedMode.width * 0.4
        height: speedMode.height * 0.1
        text: "PERFORMANCE INFO"
        color: "white"
        font.pixelSize: Math.min(speedMode.height * 0.06,
                                 speedMode.width * 0.035)
        font.bold: true
    }

    ThermometerValueComponent {
        id: packTempComponent
        anchors {
            horizontalCenter: vehicleInfo.horizontalCenter
            bottom: motorTempComponent.top
        }

        title: 'PACK TEMP'
        width: speedMode.width * 0.15
        height: speedMode.height * 0.25
        thermometerValue: speedMode.packTemp
    }

    ThermometerValueComponent {
        id: motorTempComponent
        anchors {
            horizontalCenter: vehicleInfo.horizontalCenter
            bottom: batteryValueComponent.top
        }

        title: 'MOTOR TEMP'
        width: speedMode.width * 0.15
        height: speedMode.height * 0.25
        thermometerValue: speedMode.motorTemp
    }

    BatteryValueComponent {
        id: batteryValueComponent
        anchors {
            bottom: speedMode.bottom
            bottomMargin: 15
            horizontalCenter: vehicleInfo.horizontalCenter
        }

        title: 'CHARGE STATE'
        width: speedMode.width * 0.15
        height: speedMode.height * 0.25
        batteryValue: speedMode.batteryValue
    }

    TimerDisplay {
        id: timerDisplay
        anchors {
            bottom: maxDrawGraph.top
            horizontalCenter: speedMode.horizontalCenter
        }
        width: speedMode.width * 0.4
        height: speedMode.height * 0.3
        currentRunTime: speedMode.timerValue
        lastRunTime: speedMode.lastRunTime
        fastestRunTime: speedMode.fastestRunTime
    }

    MaxDrawGraph {
        id: maxDrawGraph
        anchors {
            bottom: speedMode.bottom
            horizontalCenter: speedMode.horizontalCenter
        }
        dimension: Math.min(speedMode.height * 0.4, speedMode.width * 0.25)
        value: speedMode.currentDraw
    }

    MaxSpeedComparator {
        id: maxSpeedComparator
        anchors {
            horizontalCenter: performanceInfo.right
            top: packTempComponent.top
        }
        width: speedMode.width / 10
        height: speedMode.height * 0.7
        currentSpeed: speedMode.currentSpeed
        previousTopSpeed: speedMode.maxSpeed
    }

    onCurrentSpeedChanged: {
        console.log("speed changed")
    }
}
