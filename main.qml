import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Button {
        id: button
        anchors.centerIn: parent
        text: "Click me"
        onClicked: homeController.callMe()
    }
}
