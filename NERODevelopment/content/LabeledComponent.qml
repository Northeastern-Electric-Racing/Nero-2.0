import QtQuick 2.15

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

    radius: height / 8

    height: 100
    width: 100

    LabelText {
        id: labelText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: labelComponent.labelVerticalSpacing

        text: title
        font.pixelSize: 0.15 * parent.width
        color: labelComponent.labelColor
    }

    Rectangle {
        id: thermRow
        anchors {
            top: labelText.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: labelComponent.labelVerticalSpacing
            leftMargin: labelComponent.horizontalPadding
            rightMargin: labelComponent.horizontalPadding
            bottomMargin: labelComponent.labelVerticalSpacing
        }

        color: 'transparent'

        Loader {
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
            text: value
            font.pixelSize: 0.8 * parent.height
            anchors.left: componentContainer.item ? componentContainer.item.right : componentContainer.right
            anchors.leftMargin: labelComponent.horizontalIconSpacing
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        LabelText {
            text: valueUnit
            anchors.left: valueText.right
            anchors.top: parent.top
            color: "#777777"
            font.pixelSize: 0.8 * parent.height
        }
    }
}
