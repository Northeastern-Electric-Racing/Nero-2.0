import QtQuick 2.15
import QtQuick.Controls

Item {
    id: root

    // public
    property bool password: false

    signal accepted(string text)
    // onAccepted: print('onAccepted', text)
    signal rejected
    // onRejected: print('onRejected')

    // private
    width: 500
    height: 500 // default size

    property double rowSpacing: 0.01 * width // horizontal spacing between keyboard
    property double columnSpacing: 0.02 * height // vertical   spacing between keyboard
    property bool shift: false
    property bool symbols: false
    property double columns: 10
    property double rows: 5
    property int selectedIndex: 0
    property int numKeys: 44

    Keys.onPressed: event => {
                        console.log(selectedIndex)
                        switch (event.key) {
                            case Qt.Key_Up:
                            selectedIndex = Math.max(0, selectedIndex - columns)
                            break
                            case Qt.Key_Down:
                            selectedIndex = Math.min(numKeys,
                                                     selectedIndex + columns)
                            break
                            case Qt.Key_Left:
                            selectedIndex = Math.max(0, selectedIndex - 1)
                            break
                            case Qt.Key_Right:
                            selectedIndex = Math.min(numKeys, selectedIndex + 1)
                            break
                        }
                    }

    MouseArea {
        anchors.fill: parent
    } // don't allow touches to pass to MouseAreas underneath

    Rectangle {
        // input
        width: root.width
        height: 0.2 * root.height

        Button {
            // close v
            id: closeButton

            text: '\u2193' // BLACK DOWN-POINTING TRIANGLE
            width: height
            height: 0.8 * parent.height
            anchors.verticalCenter: parent.verticalCenter
            x: columnSpacing

            onClicked: rejected() // emit
        }

        TextInput {
            id: textInput

            cursorVisible: true
            anchors {
                left: closeButton.right
                right: clearButton.left
                verticalCenter: parent.verticalCenter
                margins: 0.03 * root.width
            }
            font.pixelSize: 0.5 * parent.height
            clip: true
            echoMode: password ? TextInput.Password : TextInput.Normal

            onAccepted: if (acceptableInput)
                            root.accepted(text) // keyboard Enter key
        }

        Button {
            // clear x
            id: clearButton

            text: '\u2715' // BLACK DOWN-POINTING TRIANGLE
            width: height
            height: 0.8 * parent.height
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: columnSpacing
            }
            enabled: textInput.text

            onClicked: textInput.text = ''
        }
    }

    Rectangle {
        width: parent.width
        height: 0.8 * parent.height
        anchors.bottom: parent.bottom

        Item {
            // keys
            id: keyboard

            anchors {
                fill: parent
                leftMargin: columnSpacing
            }

            Column {
                spacing: columnSpacing

                Row {
                    // 1234567890
                    spacing: rowSpacing

                    Repeater {
                        model: [{
                                "text": '1',
                                "width": 1
                            }, {
                                "text": '2',
                                "width": 1
                            }, {
                                "text": '3',
                                "width": 1
                            }, {
                                "text": '4',
                                "width": 1
                            }, {
                                "text": '5',
                                "width": 1
                            }, {
                                "text": '6',
                                "width": 1
                            }, {
                                "text": '7',
                                "width": 1
                            }, {
                                "text": '8',
                                "width": 1
                            }, {
                                "text": '9',
                                "width": 1
                            }, {
                                "text": '0',
                                "width": 1
                            }]

                        delegate: Button {
                            text: modelData.text
                            width: modelData.width * keyboard.width / columns - rowSpacing
                            height: keyboard.height / rows - columnSpacing
                            highlighted: root.selectedIndex === index
                            focus: root.selectedIndex === index

                            onClicked: root.clicked(text)

                            Keys.onReturnPressed: {
                                clicked()
                                event.accepted = true
                            }
                        }
                    }
                }

                Row {
                    // qwertyuiop
                    spacing: rowSpacing

                    Repeater {
                        model: [{
                                "text": 'q',
                                "symbol": '+',
                                "width": 1
                            }, {
                                "text": 'w',
                                "symbol": '\u00D7',
                                "width": 1
                            }, // MULTIPLICATION SIGN
                            {
                                "text": 'e',
                                "symbol": '\u00F7',
                                "width": 1
                            }, // DIVISION SIGN
                            {
                                "text": 'r',
                                "symbol": '=',
                                "width": 1
                            }, {
                                "text": 't',
                                "symbol": '/',
                                "width": 1
                            }, {
                                "text": 'y',
                                "symbol": '_',
                                "width": 1
                            }, {
                                "text": 'u',
                                "symbol": '<',
                                "width": 1
                            }, {
                                "text": 'i',
                                "symbol": '>',
                                "width": 1
                            }, {
                                "text": 'o',
                                "symbol": '[',
                                "width": 1
                            }, {
                                "text": 'p',
                                "symbol": ']',
                                "width": 1
                            }]

                        delegate: Button {
                            text: symbols ? modelData.symbol : shift ? modelData.text.toUpperCase(
                                                                           ) : modelData.text
                            width: modelData.width * keyboard.width / columns - rowSpacing
                            height: keyboard.height / rows - columnSpacing

                            highlighted: root.selectedIndex === index + root.columns
                            focus: root.selectedIndex === index + root.columns

                            onClicked: root.clicked(text)

                            Keys.onReturnPressed: {
                                clicked()
                                event.accepted = true
                            }
                        }
                    }
                }

                Row {
                    // asdfghjkl
                    spacing: rowSpacing

                    Repeater {
                        model: [{
                                "text": '',
                                "symbol": '',
                                "width": 0.5
                            }, {
                                "text": 'a',
                                "symbol": '!',
                                "width": 1
                            }, {
                                "text": 's',
                                "symbol": '@',
                                "width": 1
                            }, {
                                "text": 'd',
                                "symbol": '#',
                                "width": 1
                            }, {
                                "text": 'f',
                                "symbol": '$',
                                "width": 1
                            }, {
                                "text": 'g',
                                "symbol": '%',
                                "width": 1
                            }, {
                                "text": 'h',
                                "symbol": '&',
                                "width": 1
                            }, {
                                "text": 'j',
                                "symbol": '*',
                                "width": 1
                            }, {
                                "text": 'k',
                                "symbol": '(',
                                "width": 1
                            }, {
                                "text": 'l',
                                "symbol": ')',
                                "width": 1
                            }, {
                                "text": '',
                                "symbol": '',
                                "width": 0.5
                            }]

                        delegate: Button {
                            text: symbols ? modelData.symbol : shift ? modelData.text.toUpperCase(
                                                                           ) : modelData.text
                            width: modelData.width * keyboard.width / columns - rowSpacing
                            height: keyboard.height / rows - columnSpacing
                            highlighted: root.selectedIndex === index + 2 * root.columns
                            focus: root.selectedIndex === index + 2 * root.columns

                            onClicked: root.clicked(text)

                            Keys.onReturnPressed: {
                                clicked()
                                event.accepted = true
                            }
                        }
                    }
                }

                Row {
                    // zxcvbnm
                    spacing: rowSpacing

                    Repeater {
                        model: [{
                                "text": '\u2191',
                                "symbol": '',
                                "width": 1.5
                            }, // UPWARDS ARROW (shift)
                            {
                                "text": 'z',
                                "symbol": '-',
                                "width": 1
                            }, {
                                "text": 'x',
                                "symbol": "'",
                                "width": 1
                            }, {
                                "text": 'c',
                                "symbol": '"',
                                "width": 1
                            }, {
                                "text": 'v',
                                "symbol": ':',
                                "width": 1
                            }, {
                                "text": 'b',
                                "symbol": ';',
                                "width": 1
                            }, {
                                "text": 'n',
                                "symbol": ',',
                                "width": 1
                            }, {
                                "text": 'm',
                                "symbol": '?',
                                "width": 1
                            }, {
                                "text": '\u2190',
                                "symbol": '\u2190',
                                "width": 1.5
                            } // LEFTWARDS ARROW (backspace)
                        ]

                        delegate: Button {
                            text: symbols ? modelData.symbol : shift ? modelData.text.toUpperCase(
                                                                           ) : modelData.text
                            width: modelData.width * keyboard.width / columns - rowSpacing
                            height: keyboard.height / rows - columnSpacing
                            highlighted: root.selectedIndex === index + 3 * root.columns + 1
                            focus: root.selectedIndex === index + 3 * root.columns + 1

                            onClicked: root.clicked(text)

                            Keys.onReturnPressed: {
                                clicked()
                                event.accepted = true
                            }
                        }
                    }
                }

                Row {
                    // space
                    spacing: rowSpacing

                    Repeater {
                        model: [{
                                "text": symbols ? 'AB' : '@#',
                                "width": 1.5
                            }, {
                                "text": ',',
                                "width": 1
                            }, {
                                "text": ' ',
                                "width": 5
                            }, // space
                            {
                                "text": '.',
                                "width": 1
                            }, {
                                "text": '\u21B5',
                                "width": 1.5
                            } // DOWNWARDS ARROW WITH CORNER LEFTWARDS (enter)
                        ]

                        delegate: Button {
                            text: modelData.text
                            width: modelData.width * keyboard.width / columns - rowSpacing
                            height: keyboard.height / rows - columnSpacing
                            highlighted: root.selectedIndex === index + 4 * root.columns
                            focus: root.selectedIndex === index + 4 * root.columns

                            onClicked: root.clicked(text)

                            Keys.onReturnPressed: {
                                clicked()
                                event.accepted = true
                            }
                        }
                    }
                }
            }
        }
    }

    signal clicked(string text)
    onClicked: {
        if (text == '\u2190') {
            // LEFTWARDS ARROW (backspace)
            var position = textInput.cursorPosition
            textInput.text = textInput.text.substring(
                        0,
                        textInput.cursorPosition - 1) + textInput.text.substring(
                        textInput.cursorPosition, textInput.text.length)
            textInput.cursorPosition = position - 1
        } else if (text == '\u2191')
            shift = !shift // UPWARDS ARROW (shift)
        else if (text == '@#')
            symbols = true
        else if (text == 'AB')
            symbols = false
        else if (text == '\u21B5')
            accepted(textInput.text) // DOWNWARDS ARROW WITH CORNER LEFTWARDS (enter)
        else {
            // insert text
            var position = textInput.cursorPosition
            textInput.text = textInput.text.substring(
                        0,
                        textInput.cursorPosition) + text + textInput.text.substring(
                        textInput.cursorPosition, textInput.text.length)
            textInput.cursorPosition = position + 1

            shift = false // momentary
        }
    }
}
