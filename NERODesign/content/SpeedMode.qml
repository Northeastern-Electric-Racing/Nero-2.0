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
        x: 0
        y: 0
        width: speedMode.width
        height: speedMode.height * 0.15
    }

    LabelText {
        id: labelText
        x: speedMode.width * 0.15
        y: speedMode.height * 0.01
        width: speedMode.width * 0.7
        height: speedMode.height * 0.12
        text: "TRACTION CONTROL - ON"
        color: "#14ff00"
        font.pixelSize: Math.min(speedMode.height * 0.09, speedMode.width * 0.06)
        font.bold : true
    }

    LabelText {
        id: vehicleInfo
        x: speedMode.width * 0.05
        y: speedMode.height * 0.12
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
        x: speedMode.width * 0.5
        y: speedMode.height * 0.12
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
        title: 'PACK TEMP'
        x: speedMode.width * 0.05
        y: speedMode.height * 0.2
        width: speedMode.width * 0.15
        height: speedMode.height * 0.25
    }

    ThermometerValueComponent {
        id: motorTempComponent
        title: 'MOTOR TEMP'
        x: speedMode.width * 0.05
        y: speedMode.height * 0.45
        width: speedMode.width * 0.15
        height: speedMode.height * 0.25
    }

    BatteryValueComponent {
        id: batteryValueComponent
        title: 'CHARGE STATE'
        x: speedMode.width * 0.05
        y: speedMode.height * 0.7
        width: speedMode.width * 0.15
        height: speedMode.height * 0.25
    }

    TimerDisplay {
        id: timerDisplay
        x: speedMode.width * 0.3
        y: speedMode.height * 0.25
        width: speedMode.width * 0.4
        height: speedMode.height * 0.3
    }

    MaxDrawGraph {
        id: maxDrawGraph
        x: speedMode.width * 0.38
        y: speedMode.height * 0.6
        dimension: Math.min(speedMode.height * 0.4,
                            speedMode.width * 0.25)
    }

    MaxSpeedComparator {
        id: maxSpeedComparator
        x: speedMode.width * 0.88
        y: speedMode.height * 0.2
        width: speedMode.width / 10
        height: speedMode.height * 0.7
    }
}
