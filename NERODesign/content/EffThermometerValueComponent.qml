import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    property int thermometerValue: 0
    property string title: "MOTOR TEMP"
    property int iconHeight: 90
    property int iconWidth: 40
    property int horizontalIconSpacing: -55
    property int labelVerticalSpacing: 10

    color: 'black'

    ColumnLayout {
        id: motorTempColumn
        anchors.fill: parent
        Layout.alignment: Qt.AlignVCenter

        RowLayout {
            id: motorTempRow
            Layout.fillHeight: true
            Layout.fillWidth: true

            Layout.alignment: Qt.AlignCenter
            spacing: -15

            Thermometer {
                id: motorTempThermometer
                value: thermometerValue
                height: 100
                Lightning{
                    dimension: 50
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            ValueText {
                id: text2
                text: thermometerValue
                Layout.preferredWidth: 100
                ValueText {
                    id: text3
                    y: 66
                    width: 35
                    height: 30
                    text: "Nm"
                    font.pixelSize: 25 // Adjust the font size as needed
                    anchors.bottom: text2.bottom
                    anchors.left: text2.right
                    anchors.bottomMargin: 5
                }
            }
        }

        LabelText {
            text: title
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
        }
        Spacer {}
    }
}
