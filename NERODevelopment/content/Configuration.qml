import QtQuick 2.15
import QtQuick.Controls
import NERO

Item {
    id: configuration
    anchors.fill: parent
    property string title: "Configuration"
    property string driverText: ""
    property string locationText: ""
    property string systemText: ""
    property int selectedConfigurationIndex: configurationController.selectedConfigurationIndex
    property bool isKeyboardSelected: configurationController.isKeyboardSelected
    property int inputHeight: 70
    property bool isFocused: false
    focus: isFocused

    Keys.onPressed: event => {
                        switch (event.key) {
                            case Qt.Key_Up:
                            configurationController.upButtonPressed()
                            break
                            case Qt.Key_Down:
                            configurationController.downButtonPressed()
                            break
                            case Qt.Key_Return:
                            configurationController.enterButtonPressed()
                            break
                            default:
                            break
                        }
                    }

    onVisibleChanged: {
        if (visible) {
            isFocused = false
            isFocused = true
        }
    }

    onIsFocusedChanged: {
        focus = !isFocused
        focus = isFocused
    }

    ValueText {
        id: title
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Configuration")
    }

    Row {
        id: driverBackground
        anchors.top: title.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        KeyboardInput {
            id: driverKeyboard
            label: "Enter Driver"
            onAccepted: text => {
                            configuration.driverText = text
                            configuration.isFocused = false
                            configuration.isFocused = true
                            configurationController.setDriverName(text)
                            configurationController.setIsKeyboardSelected(false)
                        }
            onRejected: {
                configuration.isFocused = false
                configurationController.setIsKeyboardSelected(false)
            }
            selected: configuration.isKeyboardSelected
                      && configuration.selectedConfigurationIndex === 0

            border.width: configuration.selectedConfigurationIndex === 0 ? 2 : 0
            border.color: "blue"
            height: configuration.inputHeight
        }
    }

    Row {
        id: locationBackground
        anchors.top: driverBackground.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        KeyboardInput {
            label: "Enter Location"
            onAccepted: text => {
                            configuration.locationText = text
                            configuration.isFocused = false
                            configuration.isFocused = true
                            configurationController.setLocationName(text)
                            configurationController.setIsKeyboardSelected(false)
                        }
            onRejected: {
                configuration.isFocused = false
                configurationController.setIsKeyboardSelected(false)
            }
            selected: configuration.isKeyboardSelected
                      && configuration.selectedConfigurationIndex === 1

            border.width: configuration.selectedConfigurationIndex === 1 ? 2 : 0
            border.color: "blue"
            height: configuration.inputHeight
        }
    }

    Row {
        id: systemBackground
        anchors.top: locationBackground.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        KeyboardInput {
            label: "Enter System"
            onAccepted: text => {
                            configuration.systemText = text
                            configuration.isFocused = false
                            configuration.isFocused = true
                            configurationController.setSystemName(text)
                            configurationController.setIsKeyboardSelected(false)
                        }
            onRejected: {
                configuration.isFocused = false
                configurationController.setIsKeyboardSelected(false)
            }
            selected: configuration.isKeyboardSelected
                      && configuration.selectedConfigurationIndex === 2

            border.width: configuration.selectedConfigurationIndex === 2 ? 2 : 0
            border.color: "blue"
            height: configuration.inputHeight
        }
    }

    Button {
        id: submitButton
        anchors.top: systemBackground.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        background: Rectangle {
            border.color: "blue"
            border.width: configuration.selectedConfigurationIndex === 3 ? 2 : 0
            radius: 10
        }

        contentItem: LabelText {
            text: "Save"
            color: "black"
        }
    }
}
