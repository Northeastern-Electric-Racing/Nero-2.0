import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

Rectangle {
    id: modal
    property int dimension: 500
    visible: false
    focus: true

    width: dimension * 2
    height: dimension
    color: "white"
    radius: 20

    Keys.onPressed: (event)=> {
        if (event.key === Qt.Key_Return)
            openModal("file:///Users/petermoise/Downloads/messi.jpeg", "GOAT", "description");

    }


    Rectangle {
        id: modalPic
        height: parent.height/1.5
        width: parent.height/1.5
        x: dimension/10
        y: -width/2
        radius: 1000
        color: "#cbcaca"

        border.color: "white"
        border.width: 10
        clip: true
        Image {
            id: modalImage
            source: ""
            width: parent.width - parent.border.width
            height: parent.height - parent.border.width
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectCrop
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: mask
            }
        }

        Rectangle {
            id: mask
            width: parent.width/2
            height: width
            radius: width/2
            visible: false
        }
    }

    Text {
        id: modalTitle
        text: ""
        color: "black"
        font.pixelSize: dimension/5
        font.bold: true
        wrapMode: Text.WordWrap
        width: (parent.width - modalTitle.x) - 20
        x: (modalPic.x + modalPic.width) + 20
    }

    Text {
        id: modalDescription
        text: ""
        color: "black"
        font.pixelSize: dimension/11
        wrapMode: Text.WordWrap
        width: (parent.width - modalDescription.x) - 20
        x: modalTitle.x
        y: modalTitle.height
    }

    Button {
        id: modalButton
        height: parent.height/5
        width: parent.width/3
        background: Rectangle {
            color: "black"
            radius: 10
        }
        x: (parent.width - width) -30
        y: (parent.height - height) - 30
        text: "ACKNOWLEDGE"
        font.pixelSize: parent.height/15
        onClicked: closeModal()
    }

    function openModal(imageUrl, title, text) {
        modalImage.source = imageUrl
        modalTitle.text = title
        modalDescription.text = text
        modal.visible = true
    }

    function closeModal() {
        modal.visible = false
    }
}
