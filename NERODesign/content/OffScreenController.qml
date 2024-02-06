import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 1024
    height: 768
    visible: true

    LabelText {
        id: labelText
        x: parent.width * 0.03
        y: parent.height * 0.83
        color: "#ffffff"
        text: "Charge State"
        font.pixelSize: parent.width * 0.04
        font.bold: true
    }

    LabelText {
        id: labelText1
        x: parent.width * 0.05
        y: parent.height * 0.61
        color: "#ffffff"
        text: "Pack Temp"
        font.pixelSize: parent.width * 0.04
        font.bold: true
    }

    LabelText {
        id: labelText2
        x: parent.width * 0.04
        y: parent.height * 0.36
        color: "#fffefe"
        text: "Motor Temp"
        font.pixelSize: parent.width * 0.04
        font.bold: true
    }

    ValueText {
        id: valueText
        x: parent.width * 0.12
        y: parent.height * 0.19
        text: "0"
        font.pixelSize: parent.width * 0.12
        font.bold: true
    }

    ValueText {
        id: valueText1
        x: parent.width * 0.12
        y: parent.height * 0.43
        width: parent.width * 0.09
        height: parent.height * 0.15
        text: "0"
        font.pixelSize: parent.width * 0.12
        font.pointSize: parent.width * 0.12
        font.bold: true
    }

    ValueText {
        id: valueText2
        x: parent.width * 0.12
        y: parent.height * 0.66
        text: "0"
        font.pixelSize: parent.width * 0.12
        font.bold: true

        LabelText {
            id: labelText6
            x: parent.width * 1.2
            y: parent.height * 0.25
            width: parent.width * 0.5
            height: parent.height * 0.5
            color: "#fffefe"
            text: "%"
            font.pixelSize: 80
            font.pointSize: parent.width * 0.5
            font.italic: false
            font.bold: true
        }
    }

        DetailDisplay {
            id: detailDisplay
            x: parent.width * 0.32
            y: parent.height * 0.25
            width: parent.width * 0.29
            height: parent.height * 0.065
            shutdownFlowTask: "SIDE BRBs"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay1
            x: parent.width * 0.32
            y: parent.height * 0.35
            width: parent.width * 0.29
            height: parent.height * 0.065
            shutdownFlowTask: "BMS"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay2
            x: parent.width * 0.32
            y: parent.height * 0.45
            width: parent.width * 0.29
            height: parent.height * 0.065
            shutdownFlowTask: "IMD"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay3
            x: parent.width * 0.32
            y: parent.height * 0.55
            width: parent.width * 0.29
            height: parent.height * 0.065
            shutdownFlowTask: "BSPD"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay4
            x: parent.width * 0.32
            y: parent.height * 0.65
            width: parent.width * 0.29
            height: parent.height * 0.06
            shutdownFlowTask: "MPU"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay5
            x: parent.width * 0.32
            y: parent.height * 0.75
            width: parent.width * 0.29
            height: parent.height * 0.06
            shutdownFlowTask: "BOTS"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay6
            x: parent.width * 0.32 + detailDisplay.width + 10
            y: parent.height * 0.25
            width: parent.width * 0.29
            height: parent.height * 0.065
            shutdownFlowTask: "INERTIA"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay7
            x: parent.width * 0.32 + detailDisplay.width + 10
            y: parent.height * 0.35
            width: parent.width * 0.29
            height: parent.height * 0.065
            shutdownFlowTask: "CP BRB"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay8
            x: parent.width * 0.32 + detailDisplay.width + 10
            y: parent.height * 0.45
            width: parent.width * 0.29
            height: parent.height * 0.065
            shutdownFlowTask: "TSMS"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay9
            x: parent.width * 0.32 + detailDisplay.width + 10
            y: parent.height * 0.55
            width: parent.width * 0.29
            height: parent.height * 0.065
            shutdownFlowTask: "HVD INTRLK"
            status: "CLEAR"
        }

        DetailDisplay {
            id: detailDisplay10
            x: parent.width * 0.32 + detailDisplay.width + 10
            y: parent.height * 0.65
            width: parent.width * 0.29
            height: parent.height * 0.065
            shutdownFlowTask: "SIDE BRBs"
            status: "CLEAR"
        }
    LabelText {
        id: labelText3
        x: parent.width * 0.17
        y: parent.height * 0.06
        color: "#ff0101"
        text: "CAR OFF"
        font.pixelSize: parent.width * 0.06
        font.bold: true
    }

    LabelText {
        id: labelText4
        x: parent.width * 0.44
        y: parent.height * -0.04
        color: "#ffffff"
        text: "-"
        font.pixelSize: parent.width * 0.15
        font.bold: true
    }

    LabelText {
        id: labelText5
        x: parent.width * 0.55
        y: parent.height * 0.06
        color: "#1cff00"
        text: "GLVMS ON"
        font.pixelSize: parent.width * 0.06
        font.bold: true
    }

    LabelText {
        id: labelText7
        x: parent.width * 0.2
        y: parent.height * 0.43
        color: "#fffbfb"
        text: "o"
        font.pixelSize: parent.width * 0.05
        font.bold: true
    }

    LabelText {
        id: labelText8
        x: parent.width * 0.2
        y: parent.height * 0.19
        color: "#fffbfb"
        text: "o"
        font.pixelSize: parent.width * 0.05
        font.bold: true
    }
}
