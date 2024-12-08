import QtQuick 2.15

Canvas {
    id: tank

    anchors.fill: parent
    focus: tank.isFocused
    visible: true

    property bool isFocused: false

    width: 800
    height: 600

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
        for (var i = 0; i < tank.width; i++) {
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
        for (var i = 0; i < points.length; i++) {
            ctx.lineTo(points[i].x, points[i].y);
        }
        ctx.lineTo(width, 0);
        ctx.closePath();
        ctx.fill();

        ctx.fillStyle = "white";
        ctx.beginPath();
        ctx.moveTo(0, height);
        for (var i = 0; i < points.length; i++) {
            ctx.lineTo(points[i].x, points[i].y);
        }
        ctx.lineTo(width, height);
        ctx.closePath();
        ctx.fill();

        ctx.strokeStyle = "white";
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.moveTo(points[0].x, points[0].y);
        for (var i = 1; i < points.length; i++) {
            ctx.lineTo(points[i].x, points[i].y);
        }
        ctx.stroke();
    }

    Component.onCompleted: requestPaint()
}
