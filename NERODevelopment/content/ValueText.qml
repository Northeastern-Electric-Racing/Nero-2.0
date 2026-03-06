import QtQuick 2.15
import QtQuick.Controls 2.15
import NERO

Text {
    property string value
    text: value.length < 2 ? "0" + value : value
    font.family: webFont.name
    font.pixelSize: 85
    font.letterSpacing: -3
    color: Theme.primaryForeground
}
