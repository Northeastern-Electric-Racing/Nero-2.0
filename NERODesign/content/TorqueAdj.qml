import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: torqueAdj
    property bool upSelected: false
    property bool downSelected: false
    height: 200
    width: 100
    color: "black"

    Column {
        id: arrowContainer
        height: parent.height
        width: parent.width

        Item {
            height: parent.height / 3
            width: parent.width

            Canvas {
                id: upCanvas

                anchors.centerIn: parent

                height: parent.height
                width: parent.width

                antialiasing: true

                onPaint: {
                    var ctx = upCanvas.getContext('2d')

                    ctx.strokeStyle = "white"
                    ctx.lineWidth = upCanvas.height * 0.3
                    ctx.beginPath()
                    ctx.moveTo(upCanvas.width * 0.05, upCanvas.height)
                    ctx.lineTo(upCanvas.width / 2, upCanvas.height * 0.1)
                    ctx.lineTo(upCanvas.width * 0.95, upCanvas.height)
                    ctx.stroke()

                }

                SequentialAnimation {
                    id: upAnimation
                    running: upSelected
                    loops: Animation.Infinite
                    PropertyAnimation { target: upCanvas; property: "opacity"; to: 0; duration: 500 }
                    PropertyAnimation { target: upCanvas; property: "opacity"; to: 1; duration: 500 }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    upSelected = !upSelected;
                }
            }
        }

        Rectangle{
            height: parent.height / 3
            width: parent.width

            color: "transparent"
        }

        Item {
            height: parent.height / 3
            width: parent.width

            anchors.baseline: True

            Canvas {
                id: downCanvas

                anchors.centerIn: parent

                height: parent.height
                width: parent.width

                antialiasing: true

                transform: Rotation { angle: 180; origin.x: downCanvas.width/2; origin.y: downCanvas.height/2 }

                onPaint: {
                    var ctx = downCanvas.getContext('2d')

                    ctx.strokeStyle = "white"
                    ctx.lineWidth = downCanvas.height * 0.3
                    ctx.beginPath()
                    ctx.moveTo(downCanvas.width * 0.05, downCanvas.height)
                    ctx.lineTo(downCanvas.width / 2, downCanvas.height * 0.1)
                    ctx.lineTo(downCanvas.width * 0.95, downCanvas.height)
                    ctx.stroke()
                }

                SequentialAnimation {
                    id: downAnimation
                    running: downSelected
                    loops: Animation.Infinite
                    PropertyAnimation { target: downCanvas; property: "opacity"; to: 0; duration: 500 }
                    PropertyAnimation { target: downCanvas; property: "opacity"; to: 1; duration: 500 }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    downSelected = !downSelected;
                }
            }
        }
    }
}
