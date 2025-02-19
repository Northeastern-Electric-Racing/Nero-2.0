import QtQuick 2.15
import QtQuick.Layouts

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
                color: 'transparent'
            },
            GradientStop {
                position: 0.5
                color: '#191919'
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
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            // topMargin: labelComponent.labelVerticalSpacing
            leftMargin: labelComponent.horizontalPadding
            rightMargin: labelComponent.horizontalPadding
            bottomMargin: labelComponent.labelVerticalSpacing
        }

        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: componentContainer
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }

            sourceComponent: labelComponent.icon
        }

        ValueText {
            id: valueText
            value: labelComponent.value
            font.pixelSize: labelComponent.valueFontSize
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        LabelText {
            text: labelComponent.valueUnit
            color: "#777777"
            font.pixelSize: labelComponent.unitFontSize
            Layout.fillWidth: true
            Layout.alignment: labelComponent.unitAnchorBottom ? Qt.AlignBottom : Qt.AlignTop
        }
    }
}
