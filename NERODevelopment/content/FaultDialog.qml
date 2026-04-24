import QtQuick
import QtQuick.Controls
import NERO

Popup {
    id: modal
    property int dimension: 500
    property int offset: (modal.width * .02)
    property var criticalList: []
    property var nonCriticalList: []

    focus: false
    modal: true
    width: dimension * 2
    height: dimension
    padding: 0
    topInset: 0
    leftInset: 0
    rightInset: 0
    bottomInset: 0
    Overlay.modal: Rectangle {
        color: "transparent"
    }

    anchors.centerIn: Overlay.overlay

    contentItem: Rectangle {
        anchors.fill: parent
        color: Theme.popoverBackground
        radius: 20

        Column {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 4

            Text {
                text: "Faults"
                color: Theme.blackForeground
                font.pixelSize: modal.dimension / 6
                font.bold: true
                wrapMode: Text.WordWrap
                width: parent.width
            }

            Repeater {
                model: modal.criticalList
                delegate: Text {
                    required property string modelData
                    text: modelData
                    color: Theme.criticalStatus
                    font.pixelSize: modal.dimension / 11
                    wrapMode: Text.WordWrap
                    width: parent.width
                }
            }

            Repeater {
                model: modal.nonCriticalList
                delegate: Text {
                    required property string modelData
                    text: modelData
                    color: Theme.accentBlue
                    font.pixelSize: modal.dimension / 11
                    wrapMode: Text.WordWrap
                    width: parent.width
                }
            }
        }
    }
}
