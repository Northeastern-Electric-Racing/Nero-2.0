import QtQuick 2.15
import NERO

Item {
    id: navigation
    anchors.fill: parent
    focus: !navigation.isSelected
    property int selectedPageIndex: navigationController.selectedPageIndex
    property bool isSelected: navigationController.isSelected
    property bool gamePageOpen: navigationController.isGamesOpen
    property int offPageIndex: 0
    property int pitDrivePageIndex: 1
    property int pitReversePageIndex: 2
    property int speedPageIndex: 3
    property int efficiencyPageIndex: 4
    property int gamePageIndex: 5
    property int exitPageIndex: gamePageOpen ? 8 : 6

    property int flappyPageIndex: 6
    property int snakePageIndex: 7

    Keys.onPressed: event => {
                        switch (event.key) {
                            case Qt.Key_Escape:
                            navigationController.homeButtonPressed()
                            break
                            case Qt.Key_Right:
                            if (!this.isSelected) {
                                navigationController.downButtonPressed()
                            }
                            break
                            case Qt.Key_Left:
                            if (!this.isSelected) {
                                navigationController.upButtonPressed()
                            }
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
                            break
                        }
                    }

    HeaderView {
        id: header
    }

    Column {
        visible: !navigation.isSelected
        spacing: 15
        anchors.centerIn: parent

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 15

            HomeIcon {
                highlighted: selectedPageIndex === offPageIndex
                text: "OFF"
            }

            HomeIcon {
                highlighted: selectedPageIndex === pitDrivePageIndex
                text: "PIT - DRIVE"
                source: "qrc:/content/images/flag.png"
            }

            HomeIcon {
                highlighted: selectedPageIndex === pitReversePageIndex
                text: "PIT - REVERSE"
                source: "qrc:/content/images/flag.png"
            }

            HomeIcon {
                highlighted: selectedPageIndex === speedPageIndex
                text: "PERFORMANCE"
                source: "qrc:/content/images/hare.png"
            }

            HomeIcon {
                highlighted: selectedPageIndex === efficiencyPageIndex
                text: "EFFICIENCY"
                source: "qrc:/content/images/turtle.png"
            }
        }

        Row {
            visible: !navigation.isSelected
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 15
            height: 150

            HomeIcon {
                highlighted: selectedPageIndex === gamePageIndex
                text: "GAMES"
                source: "qrc:/content/images/game.png"
                visible: !gamePageOpen
            }

            Column {
                visible: gamePageOpen
                spacing: 10

                SubMenuIcon {
                    highlighted: selectedPageIndex === flappyPageIndex
                    text: "FLAPPY BIRD"
                }

                SubMenuIcon {
                    highlighted: selectedPageIndex === snakePageIndex
                    text: "SNAKE"
                }
            }

            HomeIcon {
                highlighted: selectedPageIndex === exitPageIndex
                text: "Exit"
            }
        }
    }

    OffScreen2 {
        visible: selectedPageIndex === offPageIndex && isSelected
        focus: selectedPageIndex === offPageIndex && isSelected
    }

    Pit {
        visible: selectedPageIndex === pitDrivePageIndex && isSelected
        focus: selectedPageIndex === pitDrivePageIndex && isSelected
    }

    Pit {
        visible: selectedPageIndex === pitReversePageIndex && isSelected
        focus: selectedPageIndex === pitReversePageIndex && isSelected
    }

    SpeedMode {
        visible: selectedPageIndex === speedPageIndex && isSelected
        focus: selectedPageIndex === speedPageIndex && isSelected
    }

    EfficiencyScreen {
        visible: selectedPageIndex === efficiencyPageIndex && isSelected
        focus: selectedPageIndex === efficiencyPageIndex && isSelected
    }

    FlappyBird {
        visible: selectedPageIndex === flappyPageIndex && isSelected
        isFocused: selectedPageIndex === flappyPageIndex && isSelected
    }

    Snake {
        visible: selectedPageIndex === snakePageIndex && isSelected
        isFocused: selectedPageIndex === snakePageIndex && isSelected
    }
}
