import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    width: 132
    height: 150
    color: "transparent"

    property bool highlighted: false
    property string text: "Off"
    property string source: "qrc:/content/images/zzz.png"

    Column {
        id: layout
        anchors.centerIn: parent
        spacing: 4

        Rectangle {
            id: icon
            width: 132
            height: 132
            radius: 15

            gradient: Gradient {
                GradientStop { position: 0.0; color: highlighted ? "#6e6e6e" : "#191919" }
                GradientStop { position: 1.0; color: highlighted ? "#333333" : "#111111" }
            }

            Image {
                id: iconImage
                source: root.source
                anchors.centerIn: parent
                width: 80
                height: 80
                fillMode: Image.PreserveAspectFit
            }
        }

        LabelText {
            id: label
            text: root.text
            font.pixelSize: 15
            width: layout.width
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
