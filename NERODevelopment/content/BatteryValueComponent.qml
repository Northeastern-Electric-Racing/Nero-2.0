import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    property int iconHeight: 90
    property int iconWidth: 50
    property int batteryValue: 50
    property string title: "CHARGE STATE"

    ColumnLayout {
        id: batteryColumn
        anchors.fill: parent

        RowLayout {
            id: batteryRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter

            Battery {
                id: battery
                Layout.fillHeight: true
                width: 55
                value: batteryValue
            }

            ValueText {
                Layout.preferredWidth: 100
                text: batteryValue
            }
        }

        LabelText {
            text: title
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
        }
    }
}
