import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    property int iconHeight: 90
    property int iconWidth: 50
    property int batteryValue: 50
    property string title: "TORQUE LIMIT"
    color:"transparent"

    ColumnLayout {
        id: batteryColumn
        anchors.fill: parent

        RowLayout {
            id: batteryRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter

            TorqueAdj {
                Layout.fillWidth: true
                Layout.fillHeight: true
                anchors.verticalCenter: parent.verticalCenter
            }

            ValueText {
                id: text2
                Layout.preferredWidth: 100
                text: batteryValue
                ValueText {
                    id: text3
                    text: "%"
                    font.pixelSize: 55 // Adjust the font size as needed
                    anchors.bottom: text2.bottom
                    anchors.left: text2.right
                    anchors.leftMargin: -5
                    anchors.bottomMargin: 5
                }
            }


        }

        LabelText {
            text: title
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
        }
    }
}
