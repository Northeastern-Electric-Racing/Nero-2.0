import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: battery
    property int value: 100
    property int maxValue: 100
    property int horizontalFillMargin: width / 10
    property int verticalFillMargin: height / 20
    height: parent.height
    width: parent.width

    Rectangle {
        id: topOutlet
        x: parent.width / 2 - parent.width / 4
        width: parent.width / 2
        height: parent.height
        color: "white"
        anchors.centerIn: parent
    }

    Rectangle {
        id: mainContainer
        width: parent.width
        height: parent.height - parent.height / 10
        color: "white"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            id: fillContainer
            width: parent.width - (battery.horizontalFillMargin * 2)
            height: (parent.height - (battery.verticalFillMargin * 2))
                    * (battery.value / battery.maxValue)
            color: battery.value > 70 ? "#55FF00" : battery.value > 40 ? "orange" : "red"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: battery.verticalFillMargin
        }
    }
}
