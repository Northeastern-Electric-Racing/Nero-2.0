import QtQuick
import QtQuick.Layouts
import NERO

Item {
    id: thermometerComponent
    property int thermometerValue
    property string title: "MOTOR TEMP"
    property int horizontalIconSpacing: width * 0.1
    property int labelVerticalSpacing: height * 0.1
    property bool regen: false
    property string labelColor: Theme.accentForeground
    property int horizontalPadding: width * 0.1
    property int radius
    property int valueFontSize
    property int labelFontSize
    property int unitFontSize: valueFontSize

    LabeledComponent {
        anchors.fill: parent
        icon: Thermometer {
            id: thermometer
            regen: thermometerComponent.regen
            value: thermometerComponent.thermometerValue
        }
        value: thermometerComponent.thermometerValue
        title: thermometerComponent.title
        horizontalIconSpacing: thermometerComponent.horizontalIconSpacing
        labelVerticalSpacing: thermometerComponent.labelVerticalSpacing
        labelColor: thermometerComponent.labelColor
        horizontalPadding: thermometerComponent.horizontalPadding
        valueUnit: regen ? "A" : "°"
        radius: thermometerComponent.radius
        valueFontSize: thermometerComponent.valueFontSize
        labelFontSize: thermometerComponent.labelFontSize
        unitFontSize: thermometerComponent.unitFontSize
        unitAnchorBottom: regen ? true : false
    }
}
