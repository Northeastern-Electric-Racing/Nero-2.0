import QtQuick 2.15
import NERO

Item {
    id: navigation
    anchors.fill: parent
    focus: !navigation.isSelected
    property int selectedPageIndex: navigationController.selectedPageIndex
    property bool isSelected: navigationController.isSelected
    property int exitPageIndex: 7
    property int offPageIndex: 0
    property int pitPageIndex: 1
    property int speedPageIndex: 2
    property int efficiencyPageIndex: 3
    property int debugPageIndex: 4
    property int configurationPageIndex: 5
    property int flappyPageIndex: 6

    Keys.onPressed: event => {
                        console.log(navigationController.isSelected,
                                    selectedPageIndex, event.key)
                        switch (event.key) {
                            case Qt.Key_Escape:
                            navigationController.homeButtonPressed()
                            break
                            case Qt.Key_Down:
                            if (!this.isSelected) {
                                navigationController.downButtonPressed()
                            }
                            break
                            case Qt.Key_Up:
                            if (!this.isSelected) {
                                navigationController.upButtonPressed()
                            }
                            break
                            case Qt.Key_Return:
                            if (!this.isSelected) {
                                navigationController.enterButtonPressed()
                            }
                        }
                    }

    Column {
        visible: !navigation.isSelected
        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        anchors.topMargin: 8
        spacing: 15

        LabelText {
            text: "NERO Home Menu"
            font.pixelSize: 40
            anchors.horizontalCenter: parent.horizontalCenter
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === offPageIndex
            text: "Off"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === pitPageIndex
            text: "Pit"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === speedPageIndex
            text: "Performance"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === efficiencyPageIndex
            text: "Efficiency"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === debugPageIndex
            text: "Debug"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === configurationPageIndex
            text: "Configuration"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === flappyPageIndex
            text: "Flappy Bird"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === exitPageIndex
            text: "Exit"
        }
    }

    OffScreen2 {
        visible: selectedPageIndex === offPageIndex && isSelected
        focus: selectedPageIndex === offPageIndex && isSelected
    }

    Pit {
        visible: selectedPageIndex === pitPageIndex && isSelected
        focus: selectedPageIndex === pitPageIndex && isSelected
    }

    SpeedMode {
        visible: selectedPageIndex === speedPageIndex && isSelected
        focus: selectedPageIndex === speedPageIndex && isSelected
    }

    EfficiencyScreen {
        visible: selectedPageIndex === efficiencyPageIndex && isSelected
        focus: selectedPageIndex === efficiencyPageIndex && isSelected
    }

    DebugTable {
        visible: selectedPageIndex === debugPageIndex && isSelected
        focus: selectedPageIndex === debugPageIndex && isSelected
    }

    Configuration {
        visible: selectedPageIndex === configurationPageIndex && isSelected
        isFocused: selectedPageIndex === configurationPageIndex && isSelected
    }

    FlappyBird {
        visible: selectedPageIndex === flappyPageIndex && isSelected
        isFocused: selectedPageIndex === flappyPageIndex && isSelected
    }
}
