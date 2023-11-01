

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

    Rectangle {
        id: rectangle1
        x: parent.width/2
        y: parent.height/2
        width: 20
        height: 70
        color: "#55ff00"
        border.color: "#55ff00"
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
    ArcItem {
        id: arc
        x: parent.width/2.44
        y: parent.height/4
        width: 200
        height: 200
        end: 270
        begin: 90
        strokeWidth: 20
        strokeColor: "#55ff00"
        fillColor: "#00000000"
    }

    Rectangle {
        id: rectangle2
        x: parent.width/2.1
        y: parent.height/1.7
        width: 70
        height: 20
        color: "#55ff00"
        border.color: "#55ff00"
    }

    Rectangle {
        id: circle2
        x: parent.width/2.2
        y: parent.height/5
        width: 110
        height: 200
        color: "#0055ff00"
        radius: 200
        border.color: "#55ff00"
        border.width: 20
    }
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


