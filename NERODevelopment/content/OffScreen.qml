import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import NERO

Item {
    id: offScreen
    width: parent.width
    height: parent.width
    visible: true
    focus: true

    property variant attributeStatusMap: offViewController.attributeStatus

    property int shutdownEfuseStatus: attributeStatusMap["VCU/eFuses/Shutdown"]
    property int bmsStatus: attributeStatusMap["VCU/Shutdown/BMS_GPIO"]
    property int imdStatus: attributeStatusMap["VCU/Shutdown/IMD_GPIO"]
    property int bspdStatus: attributeStatusMap["VCU/Shutdown/BSPD_GPIO"]
    property int botsStatus: attributeStatusMap["VCU/Shutdown/BOTS_GPIO"]
    property int inertiaStatus: attributeStatusMap["VCU/Shutdown/INERTIA_SW_GPIO"]
    property int cockPitBrbStatus: attributeStatusMap["VCU/Shutdown/CKPT_GPIO"]
    property int tsmsStatus: attributeStatusMap["VCU/CarState/tsms"]
    property int hvdInterlockStatus: attributeStatusMap["VCU/Shutdown/HVD_GPIO"]
    property int hvdConnectorStatus: attributeStatusMap["VCU/Shutdown/HV_C_GPIO"]
    property int mcEfuseStatus: attributeStatusMap["VCU/eFuses/MC"]

    property double packTemp: offViewController.packTemp
    property double motorTemp: offViewController.motorTemp
    property double stateOfCharge: offViewController.stateOfCharge

    property int selectedFlowIndex: offViewController.selectedFlowIndex

    property bool didSelect: offViewController.didSelect

    Keys.onPressed: event => {
        switch (event.key) {
        case Qt.Key_Up:
            offViewController.upButtonPressed();
            break;
        case Qt.Key_Left:
            offViewController.leftButtonPressed();
            break;
        case Qt.Key_Right:
            offViewController.rightButtonPressed();
            break;
        case Qt.Key_Down:
            offViewController.downButtonPressed();
            break;
        case Qt.Key_Return:
            offViewController.enterButtonPressed();
            break;
        default:
            break;
        }
    }

    onDidSelectChanged: {
        if (offScreen.didSelect) {
            descriptionModal.openModal(offViewController.selectedName, offViewController.selectedDescription, offViewController.selectedUrl);
        } else {
            descriptionModal.closeModal();
        }
    }

    RowLayout {
        id: offHeader
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter

        LabelText {
            text: "CAR OFF"
            color: Theme.offCarForeground
            font.pixelSize: 40
            font.bold: true
        }

        LabelText {
            text: "-"
            color: Theme.primaryForeground
            font.pixelSize: 40
            font.bold: true
        }

        LabelText {
            text: "GLVMS ON"
            color: Theme.onGLVMSForeground
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
            status: offScreen.shutdownEfuseStatus
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
            shutdownFlowTask: "CAN"
            status: offScreen.mcEfuseStatus
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
