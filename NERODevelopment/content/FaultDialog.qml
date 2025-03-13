import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Popup {
    id: modal
    property int dimension: 500
    property int offset: (modal.width * .02)
    focus: true
    modal: true
    width: dimension * 2
    height: dimension
    padding: 0
    topInset: 0
    leftInset: 0
    rightInset: 0
    bottomInset: 0
    Overlay.modal: Rectangle {
        color: "transparent"
    }

    anchors.centerIn: Overlay.overlay

    contentItem: Rectangle {
        anchors.fill: parent
        color: 'white'
        radius: 20

        Text {
            id: modalTitle
            color: "black"
            font.pixelSize: dimension / 6
            font.bold: true
            wrapMode: Text.WordWrap
            width: (parent.width - modalTitle.x) - modal.offset
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 5
            anchors.topMargin: 5
        }

        Text {
            id: modalDescription
            color: "black"
            font.pixelSize: dimension / 11
            wrapMode: Text.WordWrap
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: modalTitle.bottom
            anchors.leftMargin: 5
            anchors.topMargin: 5
        }
    }

    function openModal(title, text) {
        modalTitle.text = title
        modalDescription.text = text
        modal.open()
    }

    function closeModal() {
        modal.close()
    }
}
