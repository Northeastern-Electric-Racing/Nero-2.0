import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    width: 800
    height: 480
    visible: true

    GridLayout {
        id: gridLayout
        x: parent.width * 0.3
        y: parent.height * 0.17
        width: parent.width * 0.5
        height: parent.height * 0.6
        rows: 6
        columns: 2
        columnSpacing: 10
        rowSpacing: 10

        // DetailDisplay elements
        DetailDisplay {
            Layout.row: 0
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "SIDE BRBs"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 1
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "BMS"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 2
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "IMD"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 3
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "BSPD"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 4
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "MPU"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 5
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "BOTS"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 0
            Layout.column: 1
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "INERTIA"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 0
            Layout.column: 2
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "CP BRB"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 0
            Layout.column: 3
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "TSMS"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 0
            Layout.column: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "HVD INTRLK"
            status: "CLEAR"
        }

        DetailDisplay {
            Layout.row: 0
            Layout.column: 5
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            width: parent.width * 0.45
            height: parent.height * 0.1
            shutdownFlowTask: "HVD CNCTR"
            status: "CLEAR"
        }
    }

    GridLayout {
        id: valueGridLayout
        x: parent.width * 0.1
        y: parent.height * 0.01
        width: parent.width * 0.8
        height: parent.height * 0.2
        rows: 1
        columns: 4
        columnSpacing: 10
        rowSpacing: 10

        LabelText {
            text: "CAR OFF"
            Layout.row: 0
            Layout.column: 0
            Layout.columnSpan: 2
            color: "#ff0101"
            font.pixelSize: parent.height * 0.5
            font.bold: true
        }

        LabelText {
            text: "-"
            Layout.row: 0
            Layout.column: 2
            Layout.columnSpan: 2
            color: "#ffffff"
            font.pixelSize: parent.height * 0.5
            font.bold: true
        }

        LabelText {
            text: "GLVMS ON"
            Layout.row: 0
            Layout.column: 3
            Layout.columnSpan: 2
            color: "#1cff00"
            font.pixelSize: parent.height * 0.5
            font.bold: true
            anchors.right: parent.right
        }
    }

    GridLayout {
        id: labelGridLayout
        x: parent.width * 0.05
        y: parent.height * 0.2
        width: parent.width * 0.4
        height: parent.height * 0.9
        rows: 3

        LabelText {
            text: "Charge State"
            Layout.row: 2
            Layout.column: 0
            color: "#ffffff"
            font.pixelSize: parent.width * 0.1
            font.bold: true
        }

        LabelText {
            text: "Pack Temp"
            Layout.row: 1
            Layout.column: 0
            color: "#ffffff"
            font.pixelSize: parent.width * 0.1
            font.bold: true
        }

        LabelText {
            text: "Motor Temp"
            Layout.row: 0
            Layout.column: 0
            color: "#fffefe"
            font.pixelSize: parent.width * 0.1
            font.bold: true
        }
    }

    GridLayout {
        id: measurementGrid
        x: parent.width * 0.007
        y: parent.height * 0.15
        width: parent.width * 0.4
        height: parent.height * 0.78
        rows: 6
        columns: 2
        columnSpacing: 1
        rowSpacing: 0

        LabelText {
            text: "°"
            Layout.row: 0
            Layout.column: 0
            color: "#fffbfb"
            font.pixelSize: parent.width * 0.2
            font.bold: true
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
        }

        LabelText {
            text: "°"
            Layout.row: 1
            Layout.column: 0
            color: "#fffbfb"
            font.pixelSize: parent.width * 0.2
            font.bold: true
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
        }

        LabelText {
            text: "%"
            Layout.row: 2
            Layout.column: 0
            color: "#fffbfb"
            font.pixelSize: parent.width * 0.13
            font.bold: true
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
        }
    }

    GridLayout {
        id: numberGrid
        x: -parent.width * 0.05
        y: parent.height * 0.1
        width: parent.width * 0.4
        height: parent.height * 0.9
        rows: 3

        ValueText {
            text: "0"
            Layout.row: 0
            Layout.column: 0
            color: "#fffbfb"
            font.pixelSize: parent.width * 0.3
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        ValueText {
            text: "0"
            Layout.row: 1
            Layout.column: 0
            color: "#fffbfb"
            font.pixelSize: parent.width * 0.3
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        ValueText {
            text: "0"
            Layout.row: 2
            Layout.column: 0
            color: "#fffbfb"
            font.pixelSize: parent.width * 0.3
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
