import QtQuick 2.15
import QtQuick.Layouts
import NERO

Item {
    id: navigation
    anchors.fill: parent
    focus: true

    height: 480
    width: 800

    property int boxSize: Math.min(height / 3, width / 6)

    Keys.onPressed: event => {
        switch (event.key) {
        case Qt.Key_Escape:
            navigationController.goHome()
            break
        case Qt.Key_Right:
        case Qt.Key_Down:
            if (!navigationController.isPageActive) navigationController.moveNext()
            break
        case Qt.Key_Left:
        case Qt.Key_Up:
            if (!navigationController.isPageActive) navigationController.movePrev()
            break
        case Qt.Key_Return:
            if (!navigationController.isPageActive) navigationController.activate()
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
        visible: !navigationController.isPageActive
        spacing: 15

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 15
            Layout.alignment: Qt.AlignHCenter

            Repeater {
                id: row1Repeater
                model: navigationController.getRow1Items()

                HomeIcon {
                    height: navigation.boxSize
                    width: navigation.boxSize
                    highlighted: navigationController.selectedIndex === modelData.index
                    text: modelData.label
                    source: modelData.icon || "/qt/qml/content/images/zzz.png"
                }
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 15
            Layout.alignment: Qt.AlignHCenter

            Repeater {
                id: row2Repeater
                model: navigationController.getRow2Items()

                Item {
                    width: navigation.boxSize
                    height: navigation.boxSize

                    property int itemIndex: modelData.index
                    property bool isItemExpanded: navigationController.expandedIndex === itemIndex

                    HomeIcon {
                        anchors.fill: parent
                        visible: !isItemExpanded
                        highlighted: navigationController.selectedIndex === itemIndex
                        text: modelData.label
                        source: modelData.icon || "/qt/qml/content/images/zzz.png"
                    }

                    ColumnLayout {
                        anchors.centerIn: parent
                        visible: isItemExpanded
                        spacing: 10

                        Repeater {
                            model: isItemExpanded ? navigationController.getChildrenOf(itemIndex) : []

                            SubMenuIcon {
                                highlighted: navigationController.selectedIndex === modelData.index
                                text: modelData.label
                            }
                        }
                    }
                }
            }
        }
    }

    // Dynamic page loader
    Loader {
        id: pageLoader
        anchors.fill: parent
        active: navigationController.isPageActive

        source: {
            if (!navigationController.isPageActive) return ""
            var qmlFile = navigationController.qmlFor(navigationController.activePageIndex)
            return qmlFile ? "/qt/qml/content/" + qmlFile : ""
        }

        onLoaded: {
            if (item) {
                item.forceActiveFocus()
                if (typeof item.isFocused !== "undefined") {
                    item.isFocused = true
                }
            }
        }
    }

    Connections {
        target: navigationController

        function onActivePageChanged() {
            if (!navigationController.isPageActive) {
                navigation.forceActiveFocus()
            }
        }

        function onThemeChanged(theme) {
            Theme.setTheme(theme)
        }

        function onExitRequested() {
            Qt.quit()
        }

        function onExpandedChanged() {
            row2Repeater.model = navigationController.getRow2Items()
        }
    }
}
