import QtQuick 6.5
import QtQuick.Shapes 1.15

Item {
    id: gauge
    property int value: 0
    property int maxValue: 100

    Shape {
        id: shape
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        // multisample, decide based on your scene settings
        layer.enabled: true
        layer.samples: 4

        ShapePath {
            fillColor: "transparent"
            strokeColor: "white"
            strokeWidth: 10

            startX: 10
            startY: shape.height - 10
            PathArc {
                relativeX: shape.width - 20
                y: shape.height
                radiusX: 25
                radiusY: 25
            }
        }
    }

    Canvas {
        id: ring
        width: parent.width
        height: parent.height

        property real borderWidth: 10
        property real ringRadius: (ring.width - borderWidth) / 2
        property real startAngle: 3.14159265  // Start at the top

        onPaint: {
            const context = getContext("2d");
            context.reset();

            // Calculate the end angle based on the value
            const endAngle = ring.startAngle + (3.14159265 / (gauge.maxValue / gauge.value));

            // Draw the progress ring
            // Adjust gradient stops based on the value
            const gradientValue = gauge.value / gauge.maxValue;
            const startStop = 1 - gradientValue;

            const gradient = context.createLinearGradient(0, 0, ring.width, 0);
            gradient.addColorStop(startStop, "yellow");
            gradient.addColorStop(1, "green");
            context.fillStyle = gradient;
            context.strokeStyle = gradient

            context.lineWidth = ring.borderWidth;
            context.beginPath();
            context.arc(ring.width / 2, ring.height, ring.ringRadius, ring.startAngle, endAngle);
            context.stroke();
        }
    }

}
