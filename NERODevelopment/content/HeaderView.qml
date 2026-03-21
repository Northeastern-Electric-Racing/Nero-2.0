import QtQuick
import QtQuick.Controls
import NERO

Item {
    id: header
    width: 800
    height: 100

    property var criticalFaults: headerController.criticalFaults
    property var nonCriticalFaults: headerController.nonCriticalFaults
    property string modeTitle: ""
    property string statusText: ""
    property color statusColor: Theme.offCarForeground

    Timer {
        id: timer
    }

    function delay(delayTime, cb) {
        timer.interval = delayTime
        timer.repeat = false
        timer.triggered.connect(cb)
        timer.start()
    }

    onCriticalFaultsChanged: {
        if (criticalFaults.length > 0) {
            faultDialog.openModal("Critical Faults", criticalFaults.join("\n"))
            delay(3000, faultDialog.closeModal)
        }
    }

    onNonCriticalFaultsChanged: {
        if (nonCriticalFaults.length > 0) {
            faultDialog.openModal("Non Critical Faults",
                                  nonCriticalFaults.join("\n"))
            delay(3000, faultDialog.closeModal)
        }
    }

    NonCriticalWarning {
        id: nonCriticalWarning
        anchors.left: parent.left
        anchors.top: parent.top
        dimension: parent.height / 2
        anchors.leftMargin: nonCriticalWarning.width / 10
        visible: nonCriticalFaults.length > 0
        numWarnings: nonCriticalFaults.length
    }

    CriticalFaultIcon {
        id: criticalFaultIcon
        visible: criticalFaults.length > 0
        anchors.left: nonCriticalWarning.right
        anchors.top: parent.top
        anchors.topMargin: criticalFaultIcon.height / 5
        dimension: parent.height / 2 * 0.9
        numWarnings: criticalFaults.length
    }

    LabelText {
        visible: modeTitle !== ""
        text: modeTitle
        anchors.centerIn: parent
        color: Theme.offCarForeground
        font.pixelSize: 36
        font.bold: true
    }

    LabelText {
        visible: statusText !== ""
        text: statusText
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        color: statusColor
        font.pixelSize: 18
        font.bold: true
    }

    FaultDialog {
        id: faultDialog
        dimension: 300
    }
}
