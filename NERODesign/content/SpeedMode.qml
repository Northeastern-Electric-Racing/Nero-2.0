import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: speedMode
    property int widthValue: 800
    property int heightValue: 480

    width: widthValue
    height: heightValue

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
        text: "TRACTION CONTROL - ON"
        color: "#14ff00"
        font.pixelSize: Math.min(speedMode.height * 0.09, speedMode.width * 0.06)
        font.bold : true
    }

    LabelText {
        id: vehicleInfo
        anchors{
            bottom: packTempComponent.top
            left: speedMode.left
        }
        width: speedMode.width * 0.25
        height: speedMode.height * 0.1
        text: "VEHICLE INFO"
        color: "white"
        font.pixelSize: Math.min(speedMode.height * 0.06,
                                 speedMode.width * 0.035)
        font.bold : true
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
        font.bold : true
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
    }

    BatteryValueComponent {
        id: batteryValueComponent
        anchors {
            bottom: speedMode.bottom
            horizontalCenter: vehicleInfo.horizontalCenter
        }

        title: 'CHARGE STATE'
        width: speedMode.width * 0.15
        height: speedMode.height * 0.25
    }

    TimerDisplay {
        id: timerDisplay
        anchors {
            bottom: maxDrawGraph.top
            horizontalCenter: speedMode.horizontalCenter
        }
        width: speedMode.width * 0.4
        height: speedMode.height * 0.3
    }

    MaxDrawGraph {
        id: maxDrawGraph
        anchors {
            bottom: speedMode.bottom
            horizontalCenter: speedMode.horizontalCenter
        }
        dimension: Math.min(speedMode.height * 0.4,
                            speedMode.width * 0.25)
    }

    MaxSpeedComparator {
        id: maxSpeedComparator
        anchors {
            horizontalCenter: performanceInfo.right
            top: packTempComponent.top
        }
        width: speedMode.width / 10
        height: speedMode.height * 0.7
    }
}
