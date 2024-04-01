import QtQuick 2.15

Item {
    id: item1
    property int iconHeight: 90
    property int iconWidth: 50
    property int batteryValue: 50
    property string title: "HV Score"

    Column {
        id: batteryColumn
        anchors.fill: parent
        anchors.left: item1.left

        Row {
            id: batteryRow

            Battery {
                id: battery
                height: iconHeight
                width: iconWidth
                value: batteryValue
            }

            ValueText {
                id: text2
                text: batteryValue
                ValueText {
                    id: text3
                    text: "%"
                    font.pixelSize: 55 // Adjust the font size as needed
                    anchors.bottom: text2.bottom
                    anchors.left: text2.right
                    anchors.bottomMargin: 5
                }
            }
        }

        LabelText {
            text: title
            anchors.horizontalCenter: batteryRow.horizontalCenter
        }
    }
}
