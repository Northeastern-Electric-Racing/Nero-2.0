import QtQuick 2.15
import QtQuick.Layouts

Item {
    property int value: 0
    property string title: ""
    property string adornment: "°"

    ColumnLayout {
        Layout.alignment: Qt.AlignCenter

        ValueText {
            Layout.alignment: Qt.AlignCenter
            text: value + adornment
        }

        LabelText {
            Layout.alignment: Qt.AlignCenter
            text: title
        }
    }
}
