import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import NERO

Item {
    id: speedMode
    anchors.fill: parent

    property int packTemp: speedController.packTemp
    property int motorTemp: speedController.motorTemp
    property int maxSpeed: speedController.maxSpeed
    property int currentSpeed: speedController.currentSpeed
    property int maxDraw: speedController.maxCurrent
    property int dcl: speedController.currentDischarge
    property double regen: speedController.regen
    property int regenPercentage: speedController.regenPercentage
    property int powerDrawPercent: speedController.powerDrawPercent

    property int xMargin: width / 20
    property int yMargin: height / 20
    property int verticalSpacing: height / 40
    property int borderRadii: 10
    property int valueFontSize: Math.min(height / 8, width / 10)
    property int labelFontSize: Math.min(height / 30, width / 20)
    property int radialUnitFontSize: valueFontSize / 4

    Keys.onPressed: event => {
        if (event.key === Qt.Key_Right) {
            headerController.toggleFaultAlerts();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        HeaderView {
            id: header
            Layout.fillWidth: true
            modeTitle: "PERFORMANCE"
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: -30
            spacing: 30

            StatusIndicator {
                label: "TRACTION CONTROL"
                active: speedController.tractionControl
            }

            StatusIndicator {
                label: "LAUNCH CONTROL"
                active: speedController.launchControl
            }
        }

        RowLayout {
            id: mainRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: speedMode.xMargin
            Layout.rightMargin: speedMode.xMargin
            Layout.bottomMargin: speedMode.yMargin
            Layout.topMargin: speedMode.verticalSpacing
            spacing: speedMode.width / 20

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                spacing: speedMode.height / 20

                ThermometerValueComponent {
                    thermometerValue: speedMode.regen
                    title: "Regen"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    radius: speedMode.borderRadii
                    valueFontSize: speedMode.valueFontSize
                    labelFontSize: speedMode.labelFontSize
                    regen: true
                    unit: "A"
                    unitAnchorBottom: true
                }

                ThermometerValueComponent {
                    thermometerValue: speedMode.motorTemp
                    title: "MOTOR TEMP"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    radius: speedMode.borderRadii
                    valueFontSize: speedMode.valueFontSize
                    labelFontSize: speedMode.labelFontSize
                }

                ThermometerValueComponent {
                    thermometerValue: speedMode.packTemp
                    title: "PACK TEMP"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    radius: speedMode.borderRadii
                    valueFontSize: speedMode.valueFontSize
                    labelFontSize: speedMode.labelFontSize
                }
            }

            ColumnLayout {
                id: radialColumn
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 2
                spacing: 0

                RowLayout {
                    id: radialRow
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 4
                    Layout.bottomMargin: speedMode.yMargin / 4
                    spacing: speedMode.width / 20

                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1

                        LabelText {
                            color: Theme.accentForeground
                            text: "TOP SPEED"
                            Layout.preferredHeight: speedMode.labelFontSize
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignBottom
                            font.pixelSize: speedMode.labelFontSize
                        }

                        Radial {
                            value: speedMode.maxSpeed
                            Layout.preferredHeight: 9
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.maximumHeight: 213
                            verticalPadding: 5
                            color: Theme.accentBlue
                            valueFontSize: speedMode.valueFontSize
                            unitFontSize: speedMode.radialUnitFontSize
                        }
                    }

                    ColumnLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                        clip: false

                        LabelText {
                            color: Theme.accentForeground
                            text: "MAX DRAW"
                            Layout.preferredHeight: speedMode.labelFontSize
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignBottom
                            font.pixelSize: speedMode.labelFontSize
                        }

                        Radial {
                            value: speedMode.maxDraw
                            Layout.preferredHeight: 9
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.maximumHeight: 213
                            verticalPadding: 5
                            maxValue: 400
                            label: "DCL: " + speedMode.dcl
                            unitLabel: 'A'
                            valueFontSize: speedMode.valueFontSize
                            unitFontSize: speedMode.radialUnitFontSize
                        }
                    }
                }

                RowLayout {
                    id: componentRow
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    spacing: speedMode.width / 40

                    BatteryValueComponent {
                        title: "PWR DRAW"
                        batteryValue: speedMode.powerDrawPercent
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        radius: speedMode.borderRadii
                        valueFontSize: speedMode.valueFontSize
                        labelFontSize: speedMode.labelFontSize
                        unitFontSize: speedMode.valueFontSize / 1.5
                    }

                    BatteryValueComponent {
                        title: "REGEN"
                        batteryValue: speedMode.regenPercentage
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        radius: speedMode.borderRadii
                        valueFontSize: speedMode.valueFontSize
                        labelFontSize: speedMode.labelFontSize
                        unitFontSize: speedMode.valueFontSize / 1.5
                    }
                }
            }
        }
    }
}
