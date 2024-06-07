import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import NERO

Item {
    id: offScreen
    width: parent.width
    height: parent.width
    visible: true
    focus: true

    property variant attributeStatusMap: offViewController.attributeStatus

    property int sideBrbStatus: attributeStatusMap["MPU/Fuses/BRB"]
    property int bmsStatus: attributeStatusMap["MPU/Shutdown/BMS"]
    property int imdStatus: attributeStatusMap["MPU/Shutdown/IMD"]
    property int bspdStatus: attributeStatusMap["MPU/Shutdown/BSPD"]
    property int botsStatus: attributeStatusMap["MPU/Shutdown/BOTS"]
    property int inertiaStatus: attributeStatusMap["MPU/Shutdown/Inertia"]
    property int cockPitBrbStatus: attributeStatusMap["MPU/Shutdown/CockpitBRB"]
    property int tsmsStatus: attributeStatusMap["MPU/State/TSMS"]
    property int hvdInterlockStatus: attributeStatusMap["MPU/Shutdown/HVC_Interlock"]
    property int hvdConnectorStatus: attributeStatusMap["MPU/Shutdown/HVD_Interlock"]
    property int mpuStatus: attributeStatusMap["MPU/Fault/Severity"]

    property double packTemp: offViewController.packTemp
    property double motorTemp: offViewController.motorTemp
    property double stateOfCharge: offViewController.stateOfCharge

    property int selectedFlowIndex: offViewController.selectedFlowIndex

    property bool didSelect: offViewController.didSelect

    Keys.onPressed: event => {
                        switch (event.key) {
                            case Qt.Key_Up:
                            offViewController.upButtonPressed()
                            break
                            case Qt.Key_Left:
                            offViewController.leftButtonPressed()
                            break
                            case Qt.Key_Right:
                            offViewController.rightButtonPressed()
                            break
                            case Qt.Key_Down:
                            offViewController.downButtonPressed()
                            break
                            case Qt.Key_Return:
                            offViewController.enterButtonPressed()
                            break
                            default:
                            break
                        }
                    }

    onAttributeStatusMapChanged: {
        console.log(attributeStatusMap)
    }

    onDidSelectChanged: {
        if (offScreen.didSelect) {
            descriptionModal.openModal(offViewController.selectedName,
                                       offViewController.selectedDescription,
                                       offViewController.selectedUrl)
        } else {
            descriptionModal.closeModal()
        }
    }

    RowLayout {
        id: offHeader
        anchors.top: parent.top
        anchors.topMargin: 15
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

    Rectangle {
        id: offValuesColumn
        anchors.top: offHeader.bottom
        anchors.topMargin: 5
        width: parent.width / 4
        anchors.left: parent.left
        property int valueHeight: 130
        property int valueMargin: 50

        ValueDisplay {
            id: packTempDisplay
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: offValuesColumn.valueMargin

            height: offValuesColumn.valueHeight
            value: offScreen.packTemp
            title: "Pack Temp"
        }

        ValueDisplay {
            id: motorTempDisplay
            anchors.top: packTempDisplay.bottom
            anchors.left: parent.left

            anchors.leftMargin: offValuesColumn.valueMargin

            height: offValuesColumn.valueHeight
            value: offScreen.motorTemp
            title: "Motor Temp"
        }

        ValueDisplay {
            anchors.top: motorTempDisplay.bottom
            anchors.left: parent.left

            anchors.leftMargin: offValuesColumn.valueMargin

            height: offValuesColumn.valueHeight
            value: offScreen.stateOfCharge
            title: "Charge State"
            adornment: "%"
        }
    }

    GridLayout {
        id: shutdownFlow
        rows: 6
        columns: 2
        columnSpacing: 10
        anchors.top: offHeader.bottom
        anchors.left: offValuesColumn.right
        anchors.leftMargin: 30
        anchors.topMargin: 5

        // DetailDisplay elements
        DetailDisplay {
            Layout.row: 0
            Layout.column: 0
            shutdownFlowTask: "SIDE BRBs"
            highlight: offScreen.selectedFlowIndex == 0
            status: offScreen.sideBrbStatus
        }

        DetailDisplay {
            Layout.row: 1
            Layout.column: 0
            shutdownFlowTask: "BMS"
            status: offScreen.bmsStatus
            highlight: offScreen.selectedFlowIndex == 1
        }

        DetailDisplay {
            Layout.row: 2
            Layout.column: 0
            shutdownFlowTask: "IMD"
            status: offScreen.imdStatus
            highlight: offScreen.selectedFlowIndex == 2
        }

        DetailDisplay {
            Layout.row: 3
            Layout.column: 0
            shutdownFlowTask: "BSPD"
            status: offScreen.bspdStatus
            highlight: offScreen.selectedFlowIndex == 3
        }

        DetailDisplay {
            Layout.row: 4
            Layout.column: 0
            shutdownFlowTask: "MPU"
            status: offScreen.mpuStatus
            highlight: offScreen.selectedFlowIndex == 4
        }

        DetailDisplay {
            Layout.row: 5
            Layout.column: 0
            shutdownFlowTask: "BOTS"
            status: offScreen.botsStatus
            highlight: offScreen.selectedFlowIndex == 5
        }

        DetailDisplay {
            Layout.row: 0
            Layout.column: 1
            shutdownFlowTask: "INERTIA"
            status: offScreen.inertiaStatus
            highlight: offScreen.selectedFlowIndex == 6
        }

        DetailDisplay {
            Layout.row: 1
            Layout.column: 1
            shutdownFlowTask: "CP BRB"
            status: offScreen.cockPitBrbStatus
            highlight: offScreen.selectedFlowIndex == 7
        }

        DetailDisplay {
            Layout.row: 2
            Layout.column: 1
            shutdownFlowTask: "TSMS"
            status: offScreen.tsmsStatus
            highlight: offScreen.selectedFlowIndex == 8
        }

        DetailDisplay {
            Layout.row: 3
            Layout.column: 1
            shutdownFlowTask: "HVD INTRLK"
            status: offScreen.hvdInterlockStatus
            highlight: offScreen.selectedFlowIndex == 9
        }

        DetailDisplay {
            Layout.row: 4
            Layout.column: 1
            shutdownFlowTask: "HVD CNCTR"
            status: offScreen.hvdConnectorStatus
            highlight: offScreen.selectedFlowIndex == 10
        }
    }

    DescriptionModal {
        id: descriptionModal
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height / 7
        dimension: 300
    }

    HeaderView {
        height: 100
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
