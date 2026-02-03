import QtQuick 2.15
import QtQuick.Layouts
import NERO

Rectangle {
    id: labelComponent
    property int value
    property string title
    property int horizontalIconSpacing
    property int labelVerticalSpacing
    property string labelColor
    property int horizontalPadding
    property Component icon
    property string valueUnit
    property int valueFontSize
    property int labelFontSize
    property int unitFontSize: valueFontSize
    property bool unitAnchorBottom: false

    gradient: Gradient {
        stops: [
            GradientStop {
                position: 1.0
                color: Theme.getColor("transparent")
            },
            GradientStop {
                position: 0.5
                color: Theme.getColor("iconBackground")
            }
        ]
    }
    height: 100
    width: 100

    LabelText {
        id: labelText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: labelComponent.labelVerticalSpacing

        text: title
        font.pixelSize: labelComponent.labelFontSize
        color: labelComponent.labelColor
    }

    RowLayout {
        id: mainRow
        anchors {
            top: labelText.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            leftMargin: labelComponent.horizontalPadding
            rightMargin: labelComponent.horizontalPadding
            bottomMargin: labelComponent.labelVerticalSpacing
        }

        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 1
            visible: !!labelComponent.icon

            sourceComponent: labelComponent.icon
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 3

            Item {
                Layout.fillWidth: true
            }

            Row {
                Layout.alignment: Qt.AlignHCenter

                ValueText {
                    id: valueText
                    value: labelComponent.value
                    font.pixelSize: labelComponent.valueFontSize
                }

                LabelText {
                    text: labelComponent.valueUnit
                    color: Theme.getColor("mutedForeground")
                    font.pixelSize: labelComponent.unitFontSize
                    anchors.bottom: labelComponent.unitAnchorBottom ? parent.bottom : null
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }
}
