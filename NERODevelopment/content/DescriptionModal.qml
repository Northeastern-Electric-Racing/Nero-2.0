import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import NERO

Rectangle {
    id: modal
    property int dimension: 500
    property int offset: (modal.width * .02)
    visible: false
    focus: true

    width: dimension * 2
    height: dimension
    color: Theme.getColor("popoverBackground")
    radius: 20

    Rectangle {
        id: modalPic
        height: parent.height / 1.5
        width: parent.height / 1.5
        x: dimension / 10
        y: -width / 2
        radius: width / 2
        color: Theme.getColor("backgroundPicture")

        border.color: Theme.getColor("popoverBackground")
        border.width: 10
        clip: true // This enables clipping of content outside the rectangle

        Image {
            id: modalImage
            anchors.fill: parent // This makes the image fill the rectangle
            source: "/qt/qml/content/images/neroLogo.png" // Set the image source here
            fillMode: Image.PreserveAspectCrop

            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Item {
                    width: modalPic.width
                    height: modalPic.height
                    Rectangle {
                        width: modalPic.width
                        height: modalPic.height
                        radius: modalPic.radius
                        color: Theme.getColor("popoverBackground")
                    }
                }
            }
        }
    }

    Text {
        id: modalTitle
        color: Theme.getColor("inverseForeground")
        font.pixelSize: dimension / 6
        font.bold: true
        wrapMode: Text.WordWrap
        width: (parent.width - modalTitle.x) - modal.offset
        x: (modalPic.x + modalPic.width) + (modal.offset)
    }

    Text {
        id: modalDescription
        color: Theme.getColor("inverseForeground")
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
            color: Theme.getColor("descriptionButtonBackground")
            border.color: Theme.getColor("descriptionButtonBorder")
            border.width: 3
            radius: 10
        }
        x: (parent.width - width) - modal.offset
        y: (parent.height - height) - modal.offset
        text: "ACKNOWLEDGE"
        font.pixelSize: parent.height / 15
        onClicked: closeModal()
        Component.onCompleted: {
            modalButton.contentItem.color = Theme.getColor("descriptionButtonForeground")
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
