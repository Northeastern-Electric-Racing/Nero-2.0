

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

/*
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Studio.Components
import QtQuick.Timeline 1.0

Item {
    id: item1
    width: 1024
    height: 768

    Image {
        id: microphonealt1svgrepocom
        x: 700
        y: 72
        width: 324
        height: 180
        source: "images/microphone-alt-1-svgrepo-com.svg"
        fillMode: Image.PreserveAspectFit
    }

}*/

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Studio.Components

Item {
    id: item1
    width: 1024
    height: 768

    ColorAnimation {
        id: colorAnimation
        target: rectangle2
        property: "fillColor"
        loops: -1
        //alwaysRunToEnd: true
        running: true
        from: "#55ff00"
        to: "black"
    }

    RectangleItem {
        id: rectangle2
        anchors.fill: parent
        strokeWidth: 0
        strokeColor: "#ffffff"
        fillColor: "#55ff00"
        adjustBorderRadius: true
    }

    /*
    Rectangle {
        id: circle1
        x: 379
        y: 176
        width: 120
        height: 210
        radius: 200
        border.color: "#ffffff"
        border.width: 30
        color: "#00000000"
    }*/
}

/*
Rectangle {
    anchors.fill: parent

    // Define your SVG icon
    Image {
        source: "your_icon.svg"
        anchors.centerIn: parent
        width: 100
        height: 100

        SequentialAnimation {
            id: pulseAnimation
            loops: Animation.Infinite
            running: true

            ColorAnimation {
                from: "blue" // Start color
                to: "white" // End color
                duration: 1000 // Animation duration (ms)
            }

            PauseAnimation {
                duration: 500 // Pause before reversing (ms)
            }

            ColorAnimation {
                from: "cyan" // Start color (reversed)
                to: "blue" // End color (reversed)
                duration: 1000 // Animation duration (ms)
            }
        }
    }
}*/


