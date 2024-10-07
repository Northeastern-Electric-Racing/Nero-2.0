import QtQuick 2.15

Rectangle {
    id: dialog
    visible: false
    property string dialogName: "Lily Shiomitsu"

    width: 400
    height: 400
    focus: true
    color: "white"
    border.color: "black"
    radius: 10
    anchors.centerIn: parent

    Text {
        id: dialogDescription
        color: "black"
        font.pixelSize: 24
        anchors.centerIn: parent
    }

    function openModal() {
        dialogDescription.text = dialogName
        dialog.visible = true
    }

    function closeModal() {
        dialog.visible = false
    }

}
