import QtQuick 2.15
import QtQuick.Controls 2.15
import NERO

Item {
    id: header
    width: 800
    height: 100

    property int numCriticalWarnings: 0
    property int numNonCriticalWarnings: 0
    property bool isTalking: false

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
<<<<<<< HEAD
        anchors.topMargin: nonCriticalWarning.height / 10
=======
>>>>>>> d7883ab716d8874701ad2f9790aaef41d77c74a5
    }

    CriticalFaultIcon {
        id: criticalFaultIcon
        //dimension: 45
        //width: 70
        //height: 70
        visible: true
        anchors.left: nonCriticalWarning.right
        anchors.top: parent.top
<<<<<<< HEAD
        anchors.topMargin: criticalFaultIcon.height / 10
        dimension: 70
=======
        anchors.topMargin: criticalFaultIcon.height / 5
        dimension: 40
>>>>>>> d7883ab716d8874701ad2f9790aaef41d77c74a5
    }

}
