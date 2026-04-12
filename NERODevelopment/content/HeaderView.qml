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

    Timer {
        id: autoCloseTimer
        interval: 3000
        repeat: false
        onTriggered: faultDialog.closeModal()
    }

    onCriticalFaultsChanged: {
        if (criticalFaults.length > 0) {
            faultDialog.openModal("Critical Faults", criticalFaults.join("\n"))
            autoCloseTimer.restart()
        }
    }

    onNonCriticalFaultsChanged: {
        if (nonCriticalFaults.length > 0) {
            faultDialog.openModal("Non Critical Faults",
                                  nonCriticalFaults.join("\n"))
            autoCloseTimer.restart()
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

    FaultDialog {
        id: faultDialog
        dimension: 300
    }
}
