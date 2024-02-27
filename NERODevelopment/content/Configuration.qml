import QtQuick 2.15
import QtQuick.Controls

Item {
    id: configuration
    anchors.fill: parent
    property string title: "Configuration"
    property string driverText: ""
    property string locationText: ""
    property string systemText: ""

    ValueText {
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        id: title
        text: qsTr("Configuration")
    }

    Row {
        id: driverRow
        anchors.top: title.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        LabelText {
            text: "Driver"
        }

        KeyboardInput {
            label: "joe mama"
            onAccepted: {
                configuration.driverText = text
            }
        }
    }

    Row {
        id: locationRow
        anchors.top: driverRow.bottom
        anchors.topMargin: 5
        LabelText {
            text: "Location"
        }

        ValueText {
            text: configuration.locationText
        }
    }

    Row {
        id: systemRow
        anchors.top: locationRow.bottom
        anchors.topMargin: 5
        LabelText {
            text: "System"
        }

        ValueText {
            text: configuration.systemText
        }
    }
}
