import QtQuick 2.15
import QtQuick.Controls 2.15
import NERO

Item {
    width: 800
    height: 480

    MicrophoneComponent {
        id: microphoneComponent
        width: 100
        height: 100
        anchors.right: parent.right
        anchors.top: parent.top
    }

    NonCriticalWarning {
        id: nonCriticalWarning
        width: 100
        height: 100
        anchors.right: parent.right
        anchors.verticalCenterOffset: -84
        anchors.verticalCenter: parent.verticalCenter
        dimension: 70

    }

    CriticalFaultIcon {
        id: criticalFaultIcon
        x: 700
        y: 230
        width: 70
        height: 70
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 15
    }

}
