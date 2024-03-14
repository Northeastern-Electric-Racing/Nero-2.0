import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    property int thermometerValue: 0
    property string title: "MOTOR TEMP"
    property int iconHeight: parent.width * 0.9
    property int iconWidth: parent.width * 0.9
    property int horizontalIconSpacing: -parent.width * 0.1
    property int labelVerticalSpacing: parent.height * 0.1

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
            spacing: -labelVerticalSpacing

            Thermometer {
                id: motorTempThermometer
                value: thermometerValue
                height: iconHeight
            }

            ValueText {
                text: thermometerValue
                Layout.preferredWidth: iconWidth
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
