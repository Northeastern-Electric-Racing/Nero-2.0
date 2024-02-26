import QtQuick 2.15

Item {
    id: configuration

    property string title: "Configuration"
    property string driverText: ""
    property string locationText: ""
    property string systemText: ""

    ValueText {
        id: title
        text: qsTr("Configuration")
    }

    Row {
        id: driverRow
        anchors.top: title.bottom
        anchors.topMargin: 5
        LabelText {
            text: "Driver"
        }

        ValueText {
            text: configuration.driverText
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
