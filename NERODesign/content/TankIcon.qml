import QtQuick 2.15

Item {
    id: tankIcon
    width: 400
    height: 300

    property real barrelAngle: 0
    property color tankColor: "black"

    Rectangle {
        width: 170
        height: 40
        radius: height / 2
        color: tankColor
        anchors.verticalCenterOffset: 14
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle {
        width: 55
        height: 20
        radius: height / 2
        color: tankColor
        anchors.verticalCenterOffset: 13
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        rotation: -22
        anchors.horizontalCenterOffset: -89
    }

    Rectangle {
        width: 55
        height: 20
        radius: height / 2
        color: tankColor
        anchors.verticalCenterOffset: 13
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        rotation: 22
        anchors.horizontalCenterOffset: 89
    }

    Item {

        width: 250
        height: 35
        clip: true
        anchors.verticalCenterOffset: 30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            width: 250
            height: 100
            radius: height / 2
            color: tankColor
            anchors.verticalCenterOffset: 15
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Rectangle {
        width: 250
        height: 45
        radius: height / 2
        color: tankColor
        anchors.verticalCenterOffset: 75
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle {
        id: turretTop
        width: 100
        height: 50
        radius: height / 2
        color: tankColor
        anchors.verticalCenterOffset: -50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: tankHatch
            width: 35
            height: 20
            radius: height / 5
            color: tankColor
            anchors.verticalCenterOffset: -19
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Rectangle {
        id: turretBottom
        width: 100
        height: 40
        color: tankColor
        anchors.verticalCenterOffset: -30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: barrel
            width: 140
            height: 16
            color: tankColor
            anchors.verticalCenterOffset: -5
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 50
            transformOrigin: Item.Left
            rotation: barrelAngle

            Rectangle {
                id: barrelTip
                width: 35
                height: 25
                radius: height / 4
                color: tankColor
                anchors.horizontalCenterOffset: 75
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
