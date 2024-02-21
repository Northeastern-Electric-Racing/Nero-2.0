import QtQuick 2.15
import QtQuick.Controls 2.15
import NERO

Item {
    width: 800
    height: 480

    MicrophoneComponent {
        id: microphoneComponent
        width: 70
        height: 70
        anchors.right: parent.right
        anchors.top: parent.topRight
    }

    NonCriticalWarning {
        id: nonCriticalWarning
        width: 70
        height: 70
        anchors.left: parent.left
        anchors.top: parent.topLeft
        dimension: 70

    }

    CriticalFaultIcon {
        id: criticalFaultIcon
        width: 70
        height: 70
        anchors.left: nonCriticalWarning.right
        anchors.top: parent.top
    }

}
