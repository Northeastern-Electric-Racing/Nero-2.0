import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import NERO

Rectangle {
    id: offScreen
    width: parent.width
    height: parent.height
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
        console.log(attributeStatusMap["MPU/Fuses/BRB"])
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
        id: lvFaultContainer
        anchors.left: parent.left
        anchors.leftMargin: parent.width / 20
        anchors.top: headerView.bottom

        height: parent.height / 2
        width: parent.width / 2
        color: "transparent"

        Rectangle {
            id: lvStatusContainer
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width
            height: parent.height / 5
            color: "transparent"

            StatusDisplay {
                id: glvms
                shutdownFlowTask: "GLVMS"
                status: 1
                highlight: offScreen.selectedFlowIndex == 0
                anchors.left: parent.left
                anchors.leftMargin: -parent.width / 20
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
                width: parent.width / 3
                height: parent.height
            }

            Arrow {
                stops: [{
                        "x": glvms.x + glvms.width,
                        "y": glvms.y + glvms.height / 2
                    }, {
                        "x": brbs.x,
                        "y": brbs.y + brbs.height / 2
                    }]
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
                    id: bms
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -parent.height / 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    shutdownFlowTask: "BMS"
                    status: offScreen.bmsStatus
                    highlight: offScreen.selectedFlowIndex == 2
                    height: parent.height / 4
                    width: parent.width
                }

                StatusDisplay {
                    id: imd
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: parent.height / 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    shutdownFlowTask: "IMD"
                    status: offScreen.imdStatus
                    highlight: offScreen.selectedFlowIndex == 3
                    height: parent.height / 4
                    width: parent.width
                }
            }

            StatusDisplay {
                id: latch
                anchors.centerIn: parent
                shutdownFlowTask: "LATCH"
                status: offScreen.bmsStatus == 1 && offScreen.imdStatus == 1
                        && offScreen.mpuStatus == 1 && offScreen.bspdStatus == 1
                height: parent.height / 4
                width: parent.width / 3
            }

            Rectangle {
                id: canbspdContainer
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 3
                color: "transparent"

                StatusDisplay {
                    id: can
                    shutdownFlowTask: "CAN"
                    status: offScreen.mpuStatus
                    highlight: offScreen.selectedFlowIndex == 4
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -parent.height / 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height / 4
                    width: parent.width
                }

                StatusDisplay {
                    id: bspd
                    shutdownFlowTask: "BSPD"
                    status: offScreen.bspdStatus
                    highlight: offScreen.selectedFlowIndex == 5
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: parent.height / 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height / 4
                    width: parent.width
                }
            }
        }

        Arrow {
            stops: [{
                    "x": brbs.x + brbs.width / 2,
                    "y": brbs.y + brbs.height
                }, {
                    "x": latch.x + latch.width / 2,
                    "y": latch.y + lvStatusContainer.height
                }]
        }

        Arrow {
            stops: [{
                    "x": bms.x + bms.width / 2,
                    "y": bms.y + bms.height + lvStatusContainer.height
                }, {
                    "x": bms.x + bms.width / 2,
                    "y": latch.y + latch.height / 2 + lvStatusContainer.height
                }, {
                    "x": latch.x,
                    "y": latch.y + latch.height / 2 + lvStatusContainer.height
                }]
        }

        Arrow {
            stops: [{
                    "x": imd.x + imd.width / 2,
                    "y": imd.y + lvStatusContainer.height
                }, {
                    "x": imd.x + imd.width / 2,
                    "y": latch.y + latch.height / 2 + lvStatusContainer.height
                }, {
                    "x": latch.x,
                    "y": latch.y + latch.height / 2 + lvStatusContainer.height
                }]
        }

        Arrow {
            stops: [{
                    "x": can.x + can.width / 2 + canbspdContainer.x,
                    "y": can.y + can.height + lvStatusContainer.height
                }, {
                    "x": can.x + can.width / 2 + canbspdContainer.x,
                    "y": latch.y + latch.height / 2 + lvStatusContainer.height
                }, {
                    "x": latch.x + latch.width,
                    "y": latch.y + latch.height / 2 + lvStatusContainer.height
                }]
        }

        Arrow {
            stops: [{
                    "x": bspd.x + bspd.width / 2 + canbspdContainer.x,
                    "y": bspd.y + lvStatusContainer.height
                }, {
                    "x": bspd.x + bspd.width / 2 + canbspdContainer.x,
                    "y": latch.y + latch.height / 2 + lvStatusContainer.height
                }, {
                    "x": latch.x + latch.width,
                    "y": latch.y + latch.height / 2 + lvStatusContainer.height
                }]
        }
    }

    Rectangle {
        id: botsInertiaContainer
        anchors.top: lvFaultContainer.bottom
        anchors.left: lvFaultContainer.horizontalCenter
        anchors.leftMargin: -latch.width / 2
        width: parent.width * 0.42
        height: parent.height / 10
        color: "transparent"

        StatusDisplay {
            id: bots
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            shutdownFlowTask: "BOTS"
            status: offScreen.botsStatus
            highlight: offScreen.selectedFlowIndex == 6
            height: parent.height
            width: parent.width * 0.4
        }

        StatusDisplay {
            id: inertia
            anchors.right: parent.right
            anchors.rightMargin: parent.width / 20
            anchors.verticalCenter: parent.verticalCenter
            shutdownFlowTask: "INERTIA"
            status: offScreen.inertiaStatus
            highlight: offScreen.selectedFlowIndex == 7
            height: parent.height
            width: parent.width * 0.4
        }

        Arrow {
            stops: [{
                    "x": bots.x + bots.width,
                    "y": bots.y + bots.height / 2
                }, {
                    "x": inertia.x,
                    "y": bots.y + bots.height / 2
                }]
        }
    }

    Arrow {
        stops: [{
                "x": latch.x + latch.width / 2 + lvFaultContainer.x,
                "y": latch.y + latch.height * 2 + lvFaultContainer.y
            }, {
                "x": bots.x + bots.width / 2 + botsInertiaContainer.x,
                "y": bots.y + botsInertiaContainer.y
            }]
    }

    Rectangle {
        id: cpBrbInterlocksTsmsContainer
        anchors.top: botsInertiaContainer.bottom
        anchors.left: inertia.anchors.right
        anchors.leftMargin: -inertia.width - botsInertiaContainer.width / 20
        anchors.topMargin: parent.height / 22
        height: parent.height / 10
        width: parent.width * 0.55
        color: "transparent"

        StatusDisplay {
            id: cpBrb
            shutdownFlowTask: "CP BRB"
            status: offScreen.cockPitBrbStatus
            anchors.left: parent.left
            highlight: offScreen.selectedFlowIndex == 8
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.width * 0.31
        }

        StatusDisplay {
            id: interlocks
            shutdownFlowTask: "INTERLOCKS"
            status: offScreen.hvdConnectorStatus == 1
                    && offScreen.hvdInterlockStatus == 1
            highlight: offScreen.selectedFlowIndex == 9
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.width * 0.31
        }

        StatusDisplay {
            id: tsms
            shutdownFlowTask: "TSMS"
            status: offScreen.tsmsStatus
            highlight: offScreen.selectedFlowIndex == 10
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.width * 0.31
        }

        Arrow {
            stops: [{
                    "x": cpBrb.x + cpBrb.width,
                    "y": cpBrb.y + cpBrb.height / 2
                }, {
                    "x": interlocks.x,
                    "y": interlocks.y + interlocks.height / 2
                }]

            l2: 10
        }

        Arrow {
            stops: [{
                    "x": interlocks.x + interlocks.width,
                    "y": interlocks.y + interlocks.height / 2
                }, {
                    "x": tsms.x,
                    "y": tsms.y + tsms.height / 2
                }]

            l2: 10
        }
    }

    Arrow {
        stops: [{
                "x": inertia.x + inertia.width / 2 + botsInertiaContainer.x,
                "y": inertia.y + inertia.height + botsInertiaContainer.y
            }, {
                "x": cpBrb.x + cpBrb.width / 2 + cpBrbInterlocksTsmsContainer.x,
                "y": cpBrb.y + cpBrbInterlocksTsmsContainer.y
            }]
    }

    Rectangle {
        id: tempBatteryContainer
        anchors.left: lvFaultContainer.right
        anchors.top: offHeader.bottom
        anchors.right: parent.right
        anchors.bottom: cpBrbInterlocksTsmsContainer.top
        color: "transparent"

        Rectangle {
            id: tempContainer
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: parent.height / 2
            color: "transparent"

            ThermometerValueComponent {
                id: packTemp
                title: "Pack Temp"
                thermometerValue: offScreen.packTemp
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: parent.width / 20
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width * 0.4
            }

            ThermometerValueComponent {
                title: "Motor Temp"
                thermometerValue: offScreen.motorTemp
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: parent.width / 20
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width * 0.4
            }
        }

        BatteryValueComponent {
            id: soc
            title: "Charge State"
            batteryValue: offScreen.stateOfCharge
            anchors.top: tempContainer.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height / 10
            width: parent.width / 2
        }
    }

    HeaderView {
        id: headerView
        height: 100
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Image {
        id: logo
        anchors.left: parent.left
        anchors.top: botsInertiaContainer.bottom
        anchors.bottom: parent.bottom
        anchors.right: cpBrbInterlocksTsmsContainer.left
        anchors.rightMargin: parent.width / 30
        anchors.leftMargin: parent.width / 30
        source: "qrc:/content/images/neroLogo.png"
        fillMode: Image.PreserveAspectFit
    }

    DescriptionModal {
        id: descriptionModal
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height / 20
        dimension: 300
    }
}
