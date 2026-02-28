import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NERO

Rectangle {
    id: root
    color: Theme.getColor("transparent")

    property bool highlighted: false
    property string text: "Off"
    property string source: "/qt/qml/content/images/zzz.png"

    ColumnLayout {
        id: imageContainer
        anchors.fill: parent
        spacing: 4

        Rectangle {
            id: icon
            Layout.fillWidth: true
            Layout.preferredHeight: root.width
            radius: 15

            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: highlighted ? Theme.getColor("iconHighlightedBackground") : Theme.getColor("iconBackground")
                }
                GradientStop {
                    position: 1.0
                    color: highlighted ? Theme.getColor("iconHighlightedGradientStop") : Theme.getColor("iconGradientStop")
                }
            }

            Image {
                id: iconImage
                source: root.source
                width: parent.width * 0.6
                height: width
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
            }
        }

        LabelText {
            id: label
            text: root.text
            font.pixelSize: 15
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
