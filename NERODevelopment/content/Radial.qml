import QtQuick
import QtQuick.Shapes
import NERO

Item {
    id: gauge
    property double value: 0
    property double minValue: 0
    property double maxValue: 100
    property int verticalPadding: gauge.height / 10
    property int horizontalPadding: gauge.width / 10
    property int innerStrokeWidth: 5
    property int outerStrokeWidth: 30
    property int mainTextTopPadding: 0
    property int heightOffset: 100
    property string label: "mph"
    property string color: Theme.getColor("accentPurple")
    property string unitLabel: ""
    property int valueFontSize: width / 10
    property int unitFontSize: valueFontSize / 2

    Text {
        id: valueText
        text: Math.round(gauge.value) + unitLabel
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: gauge.valueFontSize
        font.family: webFont.name
        color: Theme.getColor("primaryForeground")

        anchors.centerIn: ring
        anchors.verticalCenterOffset: -gauge.height / 20
    }

    Behavior on value {

        NumberAnimation {
            duration: 100
        }
    }

    Shape {
        id: ring
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        antialiasing: true
        visible: ring.arcVisible

        property real centerX: ring.width / 2
        property real centerY: ring.height / 2
        property real ringRadius: Math.sqrt(
                                      Math.pow(
                                          ring.height - gauge.verticalPadding * 2,
                                          2) + Math.pow(
                                          ring.width - gauge.horizontalPadding * 2,
                                          2)) / 3
        property real startAngle: Math.PI / 2
        property real rawStep: ((gauge.value - gauge.minValue)
                                / (gauge.maxValue - gauge.minValue)) * 2 * Math.PI
        property real clampedStep: Math.min(rawStep, 2 * Math.PI - 0.001)
        property bool arcVisible: clampedStep > 0.001
        property bool largeArc: clampedStep > Math.PI

        property real outerR: ringRadius + gauge.outerStrokeWidth / 2
        property real innerR: ringRadius - gauge.outerStrokeWidth / 2

        function polarX(r, angle) {
            return centerX + r * Math.cos(angle)
        }

        function polarY(r, angle) {
            return centerY + r * Math.sin(angle)
        }

        ShapePath {
            fillColor: gauge.color
            strokeColor: "transparent"
            strokeWidth: -1

            fillGradient: LinearGradient {
                x1: 0; y1: 0
                x2: ring.width; y2: 0
                GradientStop { position: 0.0; color: Theme.fillGradientStop }
                GradientStop { position: 1.0; color: gauge.color }
            }

            startX: ring.polarX(ring.outerR, ring.startAngle)
            startY: ring.polarY(ring.outerR, ring.startAngle)

            PathArc {
                x: ring.polarX(ring.outerR, ring.startAngle + ring.clampedStep)
                y: ring.polarY(ring.outerR, ring.startAngle + ring.clampedStep)
                radiusX: ring.outerR
                radiusY: ring.outerR
                useLargeArc: ring.largeArc
                direction: PathArc.Clockwise
            }

            PathLine {
                x: ring.polarX(ring.innerR, ring.startAngle + ring.clampedStep)
                y: ring.polarY(ring.innerR, ring.startAngle + ring.clampedStep)
            }

            PathArc {
                x: ring.polarX(ring.innerR, ring.startAngle)
                y: ring.polarY(ring.innerR, ring.startAngle)
                radiusX: ring.innerR
                radiusY: ring.innerR
                useLargeArc: ring.largeArc
                direction: PathArc.Counterclockwise
            }

            PathLine {
                x: ring.polarX(ring.outerR, ring.startAngle)
                y: ring.polarY(ring.outerR, ring.startAngle)
            }
        }

        ShapePath {
            fillColor: "transparent"
            strokeColor: gauge.color
            strokeWidth: gauge.innerStrokeWidth
            capStyle: ShapePath.FlatCap

            startX: ring.polarX(ring.innerR, ring.startAngle)
            startY: ring.polarY(ring.innerR, ring.startAngle)

            PathArc {
                x: ring.polarX(ring.innerR, ring.startAngle + ring.clampedStep)
                y: ring.polarY(ring.innerR, ring.startAngle + ring.clampedStep)
                radiusX: ring.innerR
                radiusY: ring.innerR
                useLargeArc: ring.largeArc
                direction: PathArc.Clockwise
            }
        }
    }

    Text {
        id: mph
        text: gauge.label
        color: Theme.getColor("mutedForeground")
        font.family: webFont.name
        font.pixelSize: gauge.unitFontSize
        anchors.top: valueText.bottom
        anchors.horizontalCenter: valueText.horizontalCenter
    }
}
