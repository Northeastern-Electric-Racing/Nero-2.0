

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 1024
    height: 768
    visible: true

    LabelText {
        id: labelText
        x: 33
        y: 640
        color: "#ffffff"
        text: "Charge State"
        font.pixelSize: 40
        font.bold: true
    }

    LabelText {
        id: labelText1
        x: 53
        y: 469
        color: "#ffffff"
        text: "Pack Temp"
        font.pixelSize: 40
        font.bold: true
    }

    LabelText {
        id: labelText2
        x: 36
        y: 280
        color: "#fffefe"
        text: "Motor Temp"
        font.pixelSize: 40
        font.bold: true
    }

    ValueText {
        id: valueText
        x: 125
        y: 150
        text: "0"
        font.pixelSize: 120
        font.bold: true
    }

    ValueText {
        id: valueText1
        x: 125
        y: 330
        width: 95
        height: 114
        text: "0"
        font.pixelSize: 120
        font.pointSize: 120
        font.bold: true
    }

    ValueText {
        id: valueText2
        x: 125
        y: 510
        text: "0"
        font.pixelSize: 120
        font.bold: true

        LabelText {
            id: labelText6
            x: 72
            y: 61
            width: 72
            height: 79
            color: "#ffffff"
            text: "%"
            font.pixelSize: 60
            font.italic: false
            font.bold: true
        }
    }

    DetailDisplay {
        id: detailDisplay
        x: 333
        y: 196
        width: 300
        height: 50
        shutdownFlowTask: "SIDE BRBs"
        status: "CLEAR"
    }

    DetailDisplay {
        id: detailDisplay1
        x: 333
        y: 276
        width: 300
        height: 50
        status: "CLEAR"
        shutdownFlowTask: "BMS"
    }

    DetailDisplay {
        id: detailDisplay2
        x: 333
        y: 359
        width: 300
        height: 50
        status: "CLEAR"
        shutdownFlowTask: "IMD"
    }

    DetailDisplay {
        id: detailDisplay3
        x: 333
        y: 439
        width: 300
        height: 50
        status: "CLEAR"
        shutdownFlowTask: "BSPD"

        DetailDisplay {
            id: detailDisplay4
            x: 0
            y: 81
            width: 300
            height: 50
            status: "CLEAR"
            shutdownFlowTask: "MPU"
        }

        DetailDisplay {
            id: detailDisplay5
            x: 0
            y: 171
            width: 300
            height: 50
            status: "CLEAR"
            shutdownFlowTask: "BOTS"
        }

        DetailDisplay {
            id: detailDisplay6
            x: 352
            y: -243
            width: 300
            height: 50
            status: "CLEAR"
            shutdownFlowTask: "INERTIA"
        }

        DetailDisplay {
            id: detailDisplay7
            x: 352
            y: -163
            width: 300
            height: 50
            status: "CLEAR"
            shutdownFlowTask: "CP BRB"
        }

        DetailDisplay {
            id: detailDisplay8
            x: 352
            y: -80
            width: 300
            height: 50
            status: "CLEAR"
            shutdownFlowTask: "TSMS"
        }

        DetailDisplay {
            id: detailDisplay9
            x: 352
            y: 0
            width: 300
            height: 50
            status: "CLEAR"
            shutdownFlowTask: "HVD INTRLK"
        }

        DetailDisplay {
            id: detailDisplay10
            x: 352
            y: 81
            width: 300
            height: 50
            status: "CLEAR"
            shutdownFlowTask: "SIDE BRBs"
        }
    }

    LabelText {
        id: labelText3
        x: 171
        y: 49
        color: "#ff0101"
        text: "CAR OFF"
        font.pixelSize: 60
        font.bold: true
    }

    LabelText {
        id: labelText4
        x: 453
        y: -28
        color: "#ffffff"
        text: "-"
        font.pixelSize: 150
        font.bold: true
    }

    LabelText {
        id: labelText5
        x: 561
        y: 49
        color: "#1cff00"
        text: "GLVMS ON"
        font.pixelSize: 60
        font.bold: true
    }

    LabelText {
        id: labelText7
        x: 200
        y: 330
        color: "#fffbfb"
        text: "o"
        font.pixelSize: 50
        font.bold: true
    }

    LabelText {
        id: labelText8
        x: 200
        y: 150
        color: "#fffbfb"
        text: "o"
        font.pixelSize: 50
        font.bold: true
    }
}
