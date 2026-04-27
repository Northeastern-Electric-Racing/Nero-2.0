import QtQuick
import QtQuick.Controls
import NERO

Item {
    id: header
    width: 800
    height: 100

    property var criticalFaults: headerController.criticalFaults
    property var nonCriticalFaults: headerController.nonCriticalFaults
    property bool faultAlertsEnabled: headerController.faultAlertsEnabled
    property bool ownsDialog: false
    property string modeTitle: ""
    property real titleHorizontalCenterOffset: 0

    Timer {
        id: autoCloseTimer
        interval: 3000
        repeat: false
        onTriggered: faultDialog.close()
    }

    function refreshFaultDialog() {
        if (!ownsDialog)
            return;
        if (!faultAlertsEnabled) {
            faultDialog.close();
            autoCloseTimer.stop();
            return;
        }
        if (criticalFaults.length === 0 && nonCriticalFaults.length === 0) {
            faultDialog.close();
            autoCloseTimer.stop();
            return;
        }
        faultDialog.close();
        faultDialog.open();
        autoCloseTimer.restart();
    }

    onCriticalFaultsChanged: refreshFaultDialog()
    onNonCriticalFaultsChanged: refreshFaultDialog()
    onFaultAlertsEnabledChanged: refreshFaultDialog()

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
        anchors.horizontalCenterOffset: header.titleHorizontalCenterOffset
        color: Theme.offCarForeground
        font.pixelSize: 36
        font.bold: true
    }

    FaultDialog {
        id: faultDialog
        dimension: 300
        criticalList: criticalFaults
        nonCriticalList: nonCriticalFaults
    }
}
