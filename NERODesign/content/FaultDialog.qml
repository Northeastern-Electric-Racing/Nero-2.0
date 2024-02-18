import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: faultDialog
    visible: true
    property string faultName: "Oh snap!"
    property string faultMessage: "An error has occurred while creating an error report."
    property bool isCritical: true

    width: 500
    height: 400
    focus: true

    Rectangle {
        id: rectangle
        width: parent.width
        height: parent.height
        color: "white"
        radius: 10

        ColumnLayout {
            id: layout
            anchors.fill: parent

            CriticalFaultIcon {
                id: critical
                visible: isCritical
                dimension: 50
                numWarnings: 0
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 60
            }

            NonCriticalWarning {
                id: nonCritical
                visible: !isCritical
                dimension: 50
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 60
                numWarnings: 0
            }

            Text {
                id: name
                text: faultDialog.faultName
                color: "#4b5c6b"
                font.pixelSize: 32
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.topMargin: -20
            }

            Text {
                id: message
                text: faultDialog.faultMessage
                wrapMode: Text.WordWrap
                color: "#4b5c6b"
                font.pixelSize: 20
                Layout.preferredWidth: parent.width * 0.5
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
                font.weight: Font.Thin
            }

            Button {
                id: button
                Layout.fillWidth: true
                Layout.preferredHeight: 75
                flat: true
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -6

                Text {
                    id: dismiss
                    font.pixelSize: 20
                    text: "Dismiss"
                    color: "white"
                    anchors.centerIn: parent
                    font.weight: Font.DemiBold
                }

                background: Rectangle {
                    radius: 0
                    color: "#e4615c"
                }
                onClicked: closeModal()
            }
        }
    }

    function openModal(name, message) {
        faultDialog.faultName = name
        faultDialog.faultMessage = message
        faultDialog.visible = true
    }

    function closeModal() {
        faultDialog.visible = false
    }

    Keys.onPressed: (event)=> {
        if (event.key === Qt.Key_Enter)
            closeModal()
        if (event.key === Qt.Key_Return)
            closeModal()
    }
}
