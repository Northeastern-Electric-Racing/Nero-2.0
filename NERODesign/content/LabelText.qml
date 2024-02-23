import QtQuick 2.15
import QtQuick.Controls 2.15

Text {
    id: text1
    font.family: webFont.name
    font.pixelSize: 24
    states: [
        State {
            name: "State1"
        }
    ]
    color: "#BFBFBF"
}
