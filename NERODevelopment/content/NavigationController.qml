import QtQuick 2.15
import NERO

Item {
    id: navigation
    width: parent.width
    height: parent.height
    focus: !isSelected
    property int selectedPageIndex: navigationController.selectedPageIndex
    property bool isSelected: navigationController.isSelected

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
                                console.log("changing")
                                navigationController.enterButtonPressed()
                            }
                        }
                    }

    Column {
        visible: !isSelected
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
            highlighted: selectedPageIndex === 0
            text: "Off"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === 1
            text: "Pit"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === 2
            text: "Debug"
        }

        HomeMenuItem {
            highlighted: selectedPageIndex === 3
            text: "Flappy Bird"
        }
    }

    OffScreen {
        visible: selectedPageIndex === 0 && isSelected
        focus: selectedPageIndex === 0 && isSelected
    }

    Pit {
        visible: selectedPageIndex === 1 && isSelected
        focus: selectedPageIndex === 1 && isSelected
    }

    DebugTable {
        visible: selectedPageIndex === 2 && isSelected
        focus: selectedPageIndex === 2 && isSelected
    }

    FlappyBird {
        visible: selectedPageIndex === 3 && isSelected
        focus: selectedPageIndex === 3 && isSelected
    }
}
