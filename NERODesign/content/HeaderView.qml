import QtQuick 2.15
import QtQuick.Controls 2.15
import NERO

Item {
    id: header
    width: 800
    height: 100

    property int numCriticalWarnings: headerController.numCriticalWarnings
    property int numNonCriticalWarnings: headerController.numNonCriticalWarnings
    property bool isTalking: headerController.isTalking

    NonCriticalWarning {
        id: nonCriticalWarning
        anchors.left: parent.left
        anchors.top: parent.top
        dimension: parent.height / 2
        anchors.leftMargin: nonCriticalWarning.width / 10
        visible: numNonCriticalWarnings > 0
    }

    CriticalFaultIcon {
        id: criticalFaultIcon
        visible: numCriticalWarnings > 0
        anchors.left: nonCriticalWarning.right
        anchors.top: parent.top
        anchors.topMargin: criticalFaultIcon.height / 5
        dimension: parent.height / 2 * 0.9
    }

    MicrophoneComponent {
        id: microphoneComponent
        height: parent.height / 2 + 10
        width: parent.height / 2 + 10
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 5
        anchors.topMargin: 5
        isTalking: header.isTalking
    }
}
