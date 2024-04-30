import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import NERO

Rectangle {
    id: offScreen
    width: 800
    height: 480
    visible: true
    focus: true
    color: "black"

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

    Rectangle {
        id: lvFaultContainer
        anchors.left: parent.left
        anchors.top: headerView.bottom
        anchors.topMargin: -parent.height / 12
        height: parent.height / 2
        width: parent.width / 2
        color: "transparent"

        Rectangle {
            id: lvStatusContainer
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width
            height: parent.height / 6
            color: "transparent"

            StatusDisplay {
                id: glvms
                shutdownFlowTask: "GLVMS"
                status: 1
                highlight: offScreen.selectedFlowIndex == 0
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width / 3
                height: parent.height
            }

            StatusDisplay {
                id: brbs
                shutdownFlowTask: "BRBS"
                status: offScreen.sideBrbStatus
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                highlight: offScreen.selectedFlowIndex == 1
                height: parent.height
            }

            Arrow {
                origin: glvms
                end: brbs
            }
        }

        Rectangle {
            anchors.top: lvStatusContainer.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            color: "transparent"

            Rectangle {
                id: bmsimdContainer
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: parent.width / 3
                color: "transparent"

                StatusDisplay {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -parent.height / 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    shutdownFlowTask: "BMS"
                    status: offScreen.bmsStatus
                    highlight: offScreen.selectedFlowIndex == 2
                    height: parent.height
                }

                StatusDisplay {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: parent.height / 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    shutdownFlowTask: "IMD"
                    status: offScreen.imdStatus
                    highlight: offScreen.selectedFlowIndex == 3
                    height: parent.height
                }
            }

            StatusDisplay {
                id: latchContainer
                anchors.centerIn: parent
                shutdownFlowTask: "LATCH"
                highlight: offScreen.selectedFlowIndex == 4
                status: offScreen.bmsStatus == 1 && offScreen.imdStatus == 1
                        && offScreen.mpuStatus == 1 && offScreen.bspdStatus == 1
                height: parent.height
            }

            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 3
                color: "transparent"

                StatusDisplay {
                    shutdownFlowTask: "CAN"
                    status: offScreen.mpuStatus
                    highlight: offScreen.selectedFlowIndex == 5
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -parent.height / 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                }

                StatusDisplay {
                    shutdownFlowTask: "BSPD"
                    status: offScreen.bspdStatus
                    highlight: offScreen.selectedFlowIndex == 6
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: parent.height / 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                }
            }
        }
    }

    Rectangle {
        id: botsInertiaContainer
        anchors.top: lvFaultContainer.bottom
        anchors.left: lvFaultContainer.horizontalCenter
        width: parent.width / 4
        height: parent.height / 8
        color: "transparent"

        StatusDisplay {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            shutdownFlowTask: "BOTS"
            status: offScreen.botsStatus
            highlight: offScreen.selectedFlowIndex == 7
            height: parent.height
        }

        StatusDisplay {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            shutdownFlowTask: "INERTIA"
            status: offScreen.inertiaStatus
            highlight: offScreen.selectedFlowIndex == 8
            height: parent.height
        }
    }

    Rectangle {
        anchors.top: botsInertiaContainer.bottom
        anchors.left: botsInertiaContainer.right
        height: parent.height / 4
        width: parent.width * 0.4
        color: "transparent"

        StatusDisplay {
            shutdownFlowTask: "CP BRB"
            status: offScreen.cockPitBrbStatus
            anchors.left: parent.left
            anchors.top: parent.top
            highlight: offScreen.selectedFlowIndex == 9
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
        }

        StatusDisplay {
            shutdownFlowTask: "INTERLOCKS"
            status: offScreen.hvdConnectorStatus == 1
                    && offScreen.hvdInterlockStatus == 1
            highlight: offScreen.selectedFlowIndex == 10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
        }

        StatusDisplay {
            shutdownFlowTask: "TSMS"
            status: offScreen.tsmsStatus
            highlight: offScreen.selectedFlowIndex == 11
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
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
        id: headerView
        height: 100
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
