import QtQuick 2.15
import QtQuick.Controls 2.15
import NERO

Item {
    width: 800
    height: 100

    MicrophoneComponent {
        id: microphoneComponent
        width: 70
        height: 70
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: microphoneComponent.width / 10
        anchors.topMargin: microphoneComponent.height / 10
    }

    NonCriticalWarning {
        id: nonCriticalWarning
        anchors.left: parent.left
        anchors.top: parent.top
        dimension: 45
        anchors.leftMargin: nonCriticalWarning.width / 10
        anchors.topMargin: nonCriticalWarning.height / 10
    }

    CriticalFaultIcon {
        id: criticalFaultIcon
        //dimension: 45
        //width: 70
        //height: 70
        visible: true
        anchors.left: nonCriticalWarning.right
        anchors.top: parent.top
        anchors.topMargin: criticalFaultIcon.height / 10
        dimension: 70
    }

}
