// QMLDialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: nameBox
    width: 300
    height: 150
    color: "lightgray"
    visible: false  // Initially hidden
    focus: true

    // Center the rectangle in the parent
    anchors.centerIn: parent

    // Text inside the rectangle displaying your name
    Text {
        text: "Hello, Tilak Patel!"
        anchors.centerIn: parent
        font.pixelSize: 24
        color: "black"
    }

    // Key handling for showing and hiding the nameBox
    Keys.onPressed: {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
            nameBox.visible = !nameBox.visible
        }
    }
}
