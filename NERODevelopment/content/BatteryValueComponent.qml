import QtQuick 2.15
import QtQuick.Layouts

Item {
    id: batteryComponent
    property int batteryValue: 0
    property string title: "Charge State"
    property int horizontalIconSpacing: width * 0.1
    property int labelVerticalSpacing: height * 0.1
    property string labelColor: "#47AFFF"
    property int horizontalPadding: width * 0.1

    LabeledComponent {
        anchors.fill: parent
        icon: Battery {
            value: batteryValue
        }
        value: batteryComponent.batteryValue
        title: batteryComponent.title
        horizontalIconSpacing: batteryComponent.horizontalIconSpacing
        labelVerticalSpacing: batteryComponent.labelVerticalSpacing
        labelColor: batteryComponent.labelColor
        horizontalPadding: batteryComponent.horizontalPadding
        valueUnit: "%"
    }
}
