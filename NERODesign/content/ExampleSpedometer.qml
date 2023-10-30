import QtQuick 2.11

Rectangle {
    id: knob
    color: "black"
    transformOrigin: Item.Center

    property int lineWidth: width / 8
    property int fontSize: width / 7

    property color knobBackgroundColor: Qt.rgba(0.1, 0.1, 0.1, 0.1)
    property color knobColor: Qt.rgba(1, 0, 0, 1)

    //value parameters
    property double from:0
    property double value: 0
    property double to: 5

    //progress circle angle
    property double fromAngle: Math.PI - 1
    property double toAngle: Math.PI *2 + 1

    //progress from right to left
    property bool reverse: false

    onValueChanged: canvas.requestPaint()

    Behavior on value {
        NumberAnimation{
            duration: 1000 //ms
        }
    }

    Canvas {
        id: background
        width: parent.width
        height: parent.height
        antialiasing: true

        property int radius: background.width/2

        onPaint: {
            var ctx = background.getContext('2d');
            ctx.strokeStyle = "gray";
            ctx.lineWidth = knob.lineWidth;
            ctx.lineCap = "round"
            ctx.beginPath();
            ctx.clearRect(0, 0, background.width, background.height);
            ctx.arc(radius, radius, radius - knob.lineWidth, knob.fromAngle, knob.toAngle, false);
            ctx.stroke();
        }
    }

    Canvas {
        id:canvas
        width: parent.width
        height: parent.height
        antialiasing: true

        property double step: knob.value / (knob.to - knob.from) * (knob.toAngle - knob.fromAngle)
        property int radius: width/2

        onPaint: {
            var ctx = canvas.getContext('2d');
            ctx.strokeStyle = "yellow" ;
            ctx.lineWidth = knob.lineWidth;
            ctx.lineCap = "round"
            ctx.beginPath();
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.arc(radius, radius, radius - knob.lineWidth, knob.fromAngle, knob.fromAngle + step, false);

            ctx.stroke();
        }
    }
}
