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
            }

            ValueText {
                text: thermometerValue
                Layout.preferredWidth: 100
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
