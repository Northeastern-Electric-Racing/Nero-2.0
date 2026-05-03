import QtQuick
import QtQuick.Controls
import NERO

LabelText {
    property string label: ""
    property bool active: false
    text: label + (active ? " - ON" : " - OFF")
    color: active ? Theme.goodStatus : Theme.criticalStatus
    font.pixelSize: 18
    font.bold: true
}
