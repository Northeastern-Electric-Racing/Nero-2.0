import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 200
    height: 40
    property string InputText: "CHARGE STATE"
    property FontLoader applicationFont: FontLoader {
            source: "Roboto-Black.ttf"
        }

    Text {
        id: text1
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "#bfbfbf"
        text: InputText
        font.pixelSize: 24
        font.family: applicationFont
        font.bold: true
    }


}
