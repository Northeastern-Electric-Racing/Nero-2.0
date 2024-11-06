import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: battery
    property int value: 100
    property int maxValue: 100
    property int horizontalFillMargin: width / 10
    property int verticalFillMargin: height / 20

    Rectangle {
        id: topOutlet
        x: parent.width / 2 - parent.width / 4
        width: parent.width / 4.5
        height: parent.height
        radius: 30
        color: battery.value > 70 ? "#55FF00" : battery.value > 40 ? "orange" : "red"
        anchors.centerIn: parent
    }

    Rectangle {
        id: mainContainer
        y: 48
        width: parent.width / 2.15
        height: parent.height - parent.height / 10
        radius: 30
        color: battery.value > 70 ? "#55FF00" : battery.value > 40 ? "orange" : "red"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            id: emptyContainer
            y: 123
            width: parent.width - (battery.horizontalFillMargin * 1.5)
            height: (parent.height - (battery.verticalFillMargin * 3))
            radius: 30
            color: "black"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 36
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: fillContainer
                y: 136
                width: parent.width
                height: (parent.height * (battery.value / battery.maxValue))
                gradient: Gradient {
                                GradientStop { position: 0.0; color: battery.value > 70 ? "#55FF00" : battery.value > 40 ? "orange" : "red" }
                                GradientStop { position: 1.0; color: "black" }
                            }
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
