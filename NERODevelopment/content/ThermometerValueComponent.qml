import QtQuick 2.15
import QtQuick.Layouts

Item {
    id: thermometerComponent
    property int thermometerValue
    property string title
    property int horizontalIconSpacing: width * 0.1
    property int labelVerticalSpacing: height * 0.1
    property bool regen: false
    property string labelColor: "#47A7FF"
    property int horizontalPadding: width * 0.1
    property int radius
    property int valueFontSize
    property int labelFontSize

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
        valueUnit: "°"
        radius: thermometerComponent.radius
        valueFontSize: thermometerComponent.valueFontSize
        labelFontSize: thermometerComponent.labelFontSize
    }
}
