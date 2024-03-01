import QtQuick 2.15

Rectangle {
    // TextInput from virtual keyboard
    id: root

    // public
    property string label: 'label'
    property bool password: false
    property bool selected: false
    property alias text: textInput.text // in/out

    signal accepted(string text)
    // onAccepted: print('onAccepted', text)

    // private
    width: 500
    height: 100 // default size
    radius: 0.2 * height

    onSelectedChanged: {
        if (selected) {
            textInput.focus = true
            keyboardController.show()
        }
    }

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

    KeyboardController {
        id: keyboardController

        password: root.password

        onAccepted: {
            textInput.text = text
            root.accepted(text) // emit
        }
    }
}
