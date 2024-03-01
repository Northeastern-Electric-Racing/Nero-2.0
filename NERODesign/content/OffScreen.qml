import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import NERO

Item {
    id: offScreen
    width: parent.width
    height: parent.width
    visible: true
    property string sideBrbStatus: offViewController.attributeStatusMap[""]
    property string bmsStatus: offViewConotroller.attributeStatusMap[""]
    property double packTemp: offViewController.packTemp
    property double motorTemp: offViewContrller.motorTemp
    property double stateOfCharge: offViewController.stateOfCharge

    Row {
        id: offHeader
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        LabelText {
            text: "CAR OFF"
            color: "#ff0101"
            font.pixelSize: 40
            font.bold: true
        }

        LabelText {
            text: "-"
            color: "#ffffff"
            font.pixelSize: 40
            font.bold: true
        }

        LabelText {
            text: "GLVMS ON"
            color: "#1cff00"
            font.pixelSize: 40
            font.bold: true
        }
    }

    Row {
        anchors.top: offHeader.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        Column {}

        GridLayout {
            id: shutdownFlow

            rows: 6
            columns: 2

            // DetailDisplay elements
            DetailDisplay {
                Layout.row: 0
                Layout.column: 0
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "SIDE BRBs"
                status: offScreen.sideBrbStatus
            }

            DetailDisplay {
                Layout.row: 1
                Layout.column: 0
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "BMS"
                status: offScreen.bmsStatus
            }

            DetailDisplay {
                Layout.row: 2
                Layout.column: 0
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "IMD"
                status: "CLEAR"
            }

            DetailDisplay {
                Layout.row: 3
                Layout.column: 0
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "BSPD"
                status: "CLEAR"
            }

            DetailDisplay {
                Layout.row: 4
                Layout.column: 0
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "MPU"
                status: "CLEAR"
            }

            DetailDisplay {
                Layout.row: 5
                Layout.column: 0
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "BOTS"
                status: "CLEAR"
            }

            DetailDisplay {
                Layout.row: 0
                Layout.column: 1
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "INERTIA"
                status: "CLEAR"
            }

            DetailDisplay {
                Layout.row: 0
                Layout.column: 2
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "CP BRB"
                status: "CLEAR"
            }

            DetailDisplay {
                Layout.row: 0
                Layout.column: 3
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "TSMS"
                status: "CLEAR"
            }

            DetailDisplay {
                Layout.row: 0
                Layout.column: 4
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "HVD INTRLK"
                status: "CLEAR"
            }

            DetailDisplay {
                Layout.row: 0
                Layout.column: 5
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                shutdownFlowTask: "HVD CNCTR"
                status: "CLEAR"
            }
        }
    }
}
