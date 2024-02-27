import QtQuick 2.15

Rectangle {
    // TextInput from virtual keyboard
    id: root

    // public
    property string label: 'label'
    property bool password: false
    property alias text: textInput.text // in/out

    signal accepted(string text) // onAccepted: print('onAccepted', text)

    // private
    width: 500
    height: 100 // default size
    border.width: 0.05 * root.height
    radius: 0.2 * height
    opacity: enabled && !mouseArea.pressed ? 1 : 0.3 // disabled/pressed state

    Text {
        // label
        visible: !textInput.text
        text: label
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
            margins: parent.radius
        }
        font.pixelSize: 0.5 * parent.height
        opacity: 0.3
    }

    TextInput {
        id: textInput

        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
            margins: parent.radius
        }
        font.pixelSize: 0.5 * parent.height
        echoMode: password ? TextInput.Password : TextInput.Normal
    }

    MouseArea {
        // comment out to input text via physical keyboard
        id: mouseArea

        anchors.fill: parent

        onClicked: keyboardController.show()
    }

    KeyboardController {
        id: keyboardController

        password: root.password

        onAccepted: {
            textInput.text = text
            root.accepted(text) // emit
        }
    }
}
