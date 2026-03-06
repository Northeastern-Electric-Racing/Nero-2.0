import QtQuick 2.15
import QtQuick.Layouts
import NERO

Item {
    id: torqueComponent
    property int torqueValue: 0
    property string title: "TORQUE LIMIT"
    property int valueFontSize
    property int labelFontSize
    property int radius
    property string labelColor: Theme.accentForeground
    property int horizontalIconSpacing: width * 0.1
    property int labelVerticalSpacing: height * 0.1
    property int unitFontSize: valueFontSize / 1.5

    LabeledComponent {
        value: torqueComponent.torqueValue
        title: torqueComponent.title
        radius: torqueComponent.radius
        valueUnit: "%"
        valueFontSize: torqueComponent.valueFontSize
        labelFontSize: torqueComponent.labelFontSize
        anchors.fill: parent
        labelColor: torqueComponent.labelColor
        horizontalIconSpacing: torqueComponent.horizontalIconSpacing
        labelVerticalSpacing: torqueComponent.labelVerticalSpacing
        unitFontSize: torqueComponent.unitFontSize
    }
}
