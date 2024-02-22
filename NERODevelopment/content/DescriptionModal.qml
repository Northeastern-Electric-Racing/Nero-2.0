import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Rectangle {
    id: modal
    property int dimension: 500
    property int offset: (modal.width * .02)
    visible: false
    focus: true

    width: dimension * 2
    height: dimension
    color: "white"
    radius: 20

    Rectangle {
        id: modalPic
        height: parent.height / 1.5
        width: parent.height / 1.5
        x: dimension / 10
        y: -width / 2
        radius: 1000
        color: "#cbcaca"

        border.color: "white"
        border.width: 10
        clip: true
        Image {
            id: modalImage
            width: parent.width - parent.border.width
            height: parent.height - parent.border.width
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectCrop
        }

        Rectangle {
            id: mask
            width: parent.width / 2
            height: width
            radius: width / 2
            visible: false
        }
    }

    Text {
        id: modalTitle
        color: "black"
        font.pixelSize: dimension / 6
        font.bold: true
        wrapMode: Text.WordWrap
        width: (parent.width - modalTitle.x) - modal.offset
        x: (modalPic.x + modalPic.width) + (modal.offset)
    }

    Text {
        id: modalDescription
        color: "black"
        font.pixelSize: dimension / 11
        wrapMode: Text.WordWrap
        width: (parent.width - modalDescription.x) - modal.offset
        x: modalTitle.x
        y: modalTitle.height
    }

    Button {
        id: modalButton
        height: parent.height / 5
        width: parent.width / 3
        background: Rectangle {
            color: "black"
            border.color: "blue"
            border.width: 3
            radius: 10
        }
        x: (parent.width - width) - modal.offset
        y: (parent.height - height) - modal.offset
        text: "ACKNOWLEDGE"
        font.pixelSize: parent.height / 15
        onClicked: closeModal()
        Component.onCompleted: {
            modalButton.contentItem.color = "white"
        }
    }

    function openModal(title, text, imageUrl) {
        modalImage.source = imageUrl
        modalTitle.text = title
        modalDescription.text = text
        modal.visible = true
    }

    function closeModal() {
        modal.visible = false
    }
}
