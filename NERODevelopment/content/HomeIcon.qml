import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true
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
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.width
            radius: 15

            gradient: Gradient {
                GradientStop { position: 0.0; color: highlighted ? "#6e6e6e" : "#191919" }
                GradientStop { position: 1.0; color: highlighted ? "#333333" : "#111111" }
            }

            Image {
                id: iconImage
                source: root.source
                anchors.centerIn: parent
                width: parent.width * 0.6
                height: width
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
