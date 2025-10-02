import QtQuick 2.15
import QtQuick.Layouts
import NERO

Item {
    id: navigation
    anchors.fill: parent
    focus: !navigation.isSelected
    property int selectedPageIndex: navigationController.selectedPageIndex
    property bool isSelected: navigationController.isSelected
    property bool gamePageOpen: navigationController.isGamesOpen
    property bool themeMenuOpen: navigationController.isThemeMenuOpen

    property int offPageIndex: 0
    property int pitDrivePageIndex: 1
    property int pitReversePageIndex: 2
    property int speedPageIndex: 3
    property int efficiencyPageIndex: 4
    property int gamePageIndex: 5

    property int flappyPageIndex: 6
    property int snakePageIndex: 7
    property int lightThemeIndex: 6
    property int darkThemeIndex: 7

    property int exitPageIndex: gamePageOpen ? 9 : (themeMenuOpen ? 8 : 7)
    property int themePageIndex: gamePageOpen ? 8 : 6

    height: 480
    width: 800

    property int boxSize: Math.min(height / 3, width / 6)

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

    LabelText {
        id: tsmsIndicator
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height * 0.1
        text: navigationController.isTsOn ? "TSMS - ON" : "TSMS - OFF"
        color: navigationController.isTsOn ? "green" : "red"
    }

    ColumnLayout {
        anchors {
            top: header.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: parent.width * 0.02
            bottomMargin: parent.height * 0.1
        }
        visible: !navigation.isSelected
        spacing: 15

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 15
            Layout.alignment: Qt.AlignHCenter

            HomeIcon {
                height: navigation.boxSize
                width: navigation.boxSize
                highlighted: selectedPageIndex === offPageIndex
                text: "OFF"
            }

            HomeIcon {
                height: navigation.boxSize
                width: navigation.boxSize
                highlighted: selectedPageIndex === pitDrivePageIndex
                text: "PIT - DRIVE"
                source: "/qt/qml/content/images/flag.png"
            }

            HomeIcon {
                height: navigation.boxSize
                width: navigation.boxSize
                highlighted: selectedPageIndex === pitReversePageIndex
                text: "PIT - REVERSE"
                source: "/qt/qml/content/images/reverse.png"
            }

            HomeIcon {
                height: navigation.boxSize
                width: navigation.boxSize
                highlighted: selectedPageIndex === speedPageIndex
                text: "PERFORMANCE"
                source: "/qt/qml/content/images/hare.png"
            }

            HomeIcon {
                height: navigation.boxSize
                width: navigation.boxSize
                highlighted: selectedPageIndex === efficiencyPageIndex
                text: "EFFICIENCY"
                source: "/qt/qml/content/images/turtle.png"
            }
        }

        RowLayout {
            visible: !navigation.isSelected
            spacing: 15
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter

            HomeIcon {
                height: navigation.boxSize
                width: navigation.boxSize
                highlighted: selectedPageIndex === gamePageIndex
                text: "GAMES"
                source: "/qt/qml/content/images/game.png"
                visible: !gamePageOpen
            }

            ColumnLayout {
                visible: gamePageOpen
                spacing: 10
                Layout.fillHeight: true
                Layout.fillWidth: true

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
                height: navigation.boxSize
                width: navigation.boxSize
                highlighted: selectedPageIndex === themePageIndex
                text: "THEMES"
                source: "/qt/qml/content/images/themes.png"
                visible: !themeMenuOpen
            }

            ColumnLayout {
                visible: themeMenuOpen
                spacing: 10
                Layout.fillHeight: true
                Layout.fillWidth: true

                SubMenuIcon {
                    highlighted: selectedPageIndex === lightThemeIndex
                    text: "LIGHT"
                }

                SubMenuIcon {
                    highlighted: selectedPageIndex === darkThemeIndex
                    text: "DARK"
                }
            }

            HomeIcon {
                height: navigation.boxSize
                width: navigation.boxSize
                highlighted: selectedPageIndex === exitPageIndex
                text: "EXIT"
                source: "/qt/qml/content/images/exit.png"
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
    Connections {
        target: navigationController
        function onThemeChanged(theme) {
            Theme.setTheme(theme)
        }
    }
}
