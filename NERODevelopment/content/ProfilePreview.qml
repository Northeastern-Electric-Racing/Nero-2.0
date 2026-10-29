import QtQuick
import QtQuick.Layouts
import NERO

// Previews the highlighted profile, Escape goes home through the VCU
Rectangle {
    id: profilePreview
    color: "black"
    height: 480
    width: 800

    property string profile: profileController.profiles[profileController.highlightedIndex]

    Component.onCompleted: profileController.reset()

    // Up and down switch profiles, left and right scroll that profile's images
    Connections {
        target: buttons
        function onPressed(button) {
            profileController.press(button);
        }
    }

    // Scaled to 75% so the profile list and counter don't cover the screenshot
    Image {
        id: previewImage
        x: 20
        y: 20
        width: parent.width * 0.75
        height: parent.height * 0.75
        visible: profileController.previewImages.length > 0
        source: visible ? profileController.previewImages[profileController.imageIndex] : ""
        fillMode: Image.PreserveAspectFit

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: Theme.mutedForeground
            border.width: 2
        }
    }

    LabelText {
        visible: !previewImage.visible
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 15
        text: profile.charAt(0) + profile.slice(1).toLowerCase() + " Placeholder"
        color: "white"
        font.bold: true
    }

    LabelText {
        visible: previewImage.visible
        anchors.top: previewImage.bottom
        anchors.horizontalCenter: previewImage.horizontalCenter
        anchors.topMargin: 15
        text: (profileController.imageIndex + 1) + " / " + profileController.previewImages.length
        color: "white"
        font.bold: true
    }

    ColumnLayout {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20
        spacing: 10

        Repeater {
            model: profileController.profiles

            SubMenuIcon {
                highlighted: index === profileController.highlightedIndex
                text: modelData
            }
        }
    }
}
