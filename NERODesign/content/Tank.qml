import QtQuick 2.15
import QtQuick.Studio.DesignEffects

Rectangle {
    id: tank
    anchors.fill: parent
    focus: tank.isFocused
    visible: true

    width: 800
    height: 600

    property bool isFocused: false

    property int playerScore: 0
    property int aiScore: 0

    Canvas {
        id: background

        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var scaleX = 50;
            var scaleY = 100;
            var offsetX = width / 2;
            var offsetY = height / 2;

            var A = Math.floor(3 + Math.random() * (6 - 3));
            var B = Math.floor(3 + Math.random() * (6 - 3));
            var C = Math.floor(3 + Math.random() * (6 - 3));
            var D = Math.floor(3 + Math.random() * (6 - 3));
            var E = Math.floor(Math.random() * (4));
            var F = Math.random();

            var points = [];
            for (var i = 0; i < background.width; i++) {
                var x = (i - offsetX) / scaleX;
                var y = D * ((1 / 15) * (1 + 3 * Math.sin(x / A + E)) * Math.sin(x / B) * (1 / 2 + Math.sin(x / C))) - F;
                var pixelY = offsetY - y * scaleY;
                points.push({x: i, y: pixelY});
            }

            var gradient = ctx.createLinearGradient(0, 0, 0, offsetY);
            gradient.addColorStop(0, "white");
            gradient.addColorStop(0.25, "lightsteelblue");
            gradient.addColorStop(1, "cornflowerblue");

            ctx.fillStyle = gradient;
            ctx.beginPath();
            ctx.moveTo(0, 0);
            for (var j = 0; j < points.length; j++) {
                ctx.lineTo(points[j].x, points[j].y);
            }
            ctx.lineTo(width, 0);
            ctx.closePath();
            ctx.fill();

            ctx.fillStyle = "white";
            ctx.beginPath();
            ctx.moveTo(0, height);
            for (var k = 0; k < points.length; k++) {
                ctx.lineTo(points[k].x, points[k].y);
            }
            ctx.lineTo(width, height);
            ctx.closePath();
            ctx.fill();

            ctx.strokeStyle = "white";
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.moveTo(points[0].x, points[0].y);
            for (var l = 1; l < points.length; l++) {
                ctx.lineTo(points[l].x, points[l].y);
            }
            ctx.stroke();
        }

        Component.onCompleted: requestPaint()
    }

    Row {
        anchors.top: parent.top
        anchors.topMargin: 40
        width: parent.width

        Item {
            id: player
            width: 100
            height: 60
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.top: parent.top
            anchors.topMargin: 20

            Text {
                id: playerLabel
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "Player:"
                font.pixelSize: 19
                font.weight: Font.Medium
                color: "black"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                id: playerScoreDisplay
                width: 60
                height: 13
                radius: height / 2
                color: "transparent"
                border.color: "mediumblue"
                border.width: 2.5
                anchors.horizontalCenter: player.horizontalCenter
                anchors.top: playerLabel.bottom
                anchors.topMargin: 10
                enabled: false
                clip: true

                Rectangle {
                    width: playerScoreDisplay.width / 3 + 7
                    height: playerScoreDisplay.height - 2
                    radius: (playerScoreDisplay.height - 2) / 2
                    anchors.left: parent.left
                    anchors.top: parent.top
                    color: tank.playerScore >= 1 ? "white" : "royalblue"
                    border.color: "transparent"
                    z: -1
                }

                Rectangle {
                    id: separator1
                    width: 2.5
                    height: playerScoreDisplay.height
                    anchors.left: playerScoreDisplay.left
                    anchors.leftMargin: playerScoreDisplay.width / 3
                    anchors.verticalCenter: parent.verticalCenter
                    color: "mediumblue"
                    border.color: "transparent"
                }

                Rectangle {
                    width: playerScoreDisplay.width / 3 - 2
                    height: playerScoreDisplay.height
                    anchors.left: separator1.right
                    anchors.top: parent.top
                    color: tank.playerScore >= 2 ? "white" : "royalblue"
                    border.color: "transparent"
                    z: -1
                }

                Rectangle {
                    width: 2.5
                    height: playerScoreDisplay.height
                    anchors.left: playerScoreDisplay.left
                    anchors.leftMargin: 2* playerScoreDisplay.width / 3
                    color: "mediumblue"
                    border.color: "transparent"
                }

                Rectangle {
                    width: playerScoreDisplay.width / 3 + 5
                    height: playerScoreDisplay.height - 2
                    radius: (playerScoreDisplay.height - 2) / 2
                    anchors.right: parent.right
                    anchors.top: parent.top
                    color: tank.playerScore >= 3 ? "white" : "royalblue"
                    border.color: "transparent"
                    z: -2
                }
            }
        }

        Item {
            id: ai
            width: 100
            height: 60
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: aiLabel
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "AI:"
                font.pixelSize: 19
                font.weight: Font.Medium
                color: "black"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                id: aiScoreDisplay
                width: 60
                height: 13
                radius: height / 2
                color: "transparent"
                border.color: "darkred"
                border.width: 2.5
                anchors.horizontalCenter: ai.horizontalCenter
                anchors.top: aiLabel.bottom
                anchors.topMargin: 10
                clip: true

                Rectangle {
                    width: aiScoreDisplay.width / 3 + 7
                    height: aiScoreDisplay.height - 2
                    radius: (aiScoreDisplay.height - 2) / 2
                    anchors.left: parent.left
                    anchors.top: parent.top
                    color: tank.aiScore >= 1 ? "white" : "#d44562"
                    border.color: "transparent"
                    z: -1
                }

                Rectangle {
                    id: separator2
                    width: 2.5
                    height: aiScoreDisplay.height
                    anchors.left: aiScoreDisplay.left
                    anchors.leftMargin: aiScoreDisplay.width / 3
                    anchors.verticalCenter: parent.verticalCenter
                    color: "darkred"
                    border.color: "transparent"
                }

                Rectangle {
                    width: aiScoreDisplay.width / 3 - 2
                    height: aiScoreDisplay.height
                    anchors.left: separator2.right
                    anchors.top: parent.top
                    color: tank.aiScore >= 2 ? "white" : "#d44562"
                    border.color: "transparent"
                    z: -1
                }

                Rectangle {
                    width: 2.5
                    height: aiScoreDisplay.height
                    anchors.left: aiScoreDisplay.left
                    anchors.leftMargin: 2* aiScoreDisplay.width / 3
                    color: "darkred"
                    border.color: "transparent"
                }

                Rectangle {
                    width: aiScoreDisplay.width / 3 + 5
                    height: aiScoreDisplay.height - 2
                    radius: (aiScoreDisplay.height - 2) / 2
                    anchors.right: parent.right
                    anchors.top: parent.top
                    color: tank.aiScore >= 3 ? "white" : "#d44562"
                    border.color: "transparent"
                    z: -2
                }
            }
        }
    }



}
