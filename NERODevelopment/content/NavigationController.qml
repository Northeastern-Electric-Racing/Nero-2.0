pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import Main
import NERO

Item {
    id: navigation
    anchors.fill: parent
    focus: true

    height: 480
    width: 800

    property int boxSize: Math.min(height / 3, width / 6)
    property int itemSpacing: 15

    // One row per VCU nero_index (Cerberus-2.0 nero_menu_t). A page opens full screen, entries expand
    // the tile, and a row with neither keeps the menu up.
    readonly property var pages: [
        { index: 0, title: "OFF", icon: "zzz.png", page: "OffScreen2.qml" },
        { index: 1, title: "PIT - DRIVE", icon: "flag.png", page: "Pit.qml" },
        { index: 2, title: "PIT - KEVIN", icon: "kevin.jpg", page: "Pit.qml" },
        { index: 3, title: "PERFORMANCE", icon: "hare.png", page: "SpeedMode.qml" },
        { index: 4, title: "ENDURANCE", icon: "turtle.png", page: "EnduranceScreen.qml" },
        {
            index: 5, title: "GAMES", icon: "game.png", entries: [
                { title: "FLAPPY BIRD", page: "FlappyBird.qml" },
                { title: "SNAKE", page: "Snake.qml" },
                { title: "2048", page: "Game2048.qml" },
                { title: "DOOM", page: "DoomView.qml" }
            ]
        },
        {
            index: 6, title: "THEMES", icon: "themes.png", entries: [
                { title: "LIGHT", theme: "light" },
                { title: "DARK", theme: "dark" }
            ]
        },
        { index: 7, title: "EXIT", icon: "exit.png" }
    ]
    readonly property var current: pages.find(p => p.index === vcu.navIndex)

    property string entryPage: ""
    readonly property string openPage: vcu.homeMenu || current === undefined ? "" : (current.page ?? entryPage)
    readonly property bool expanded: !vcu.homeMenu && current?.entries !== undefined && entryPage === ""
    readonly property bool showingPage: openPage !== ""

    readonly property var keyButtons: ({
            [Qt.Key_Escape]: Buttons.Esc,
            [Qt.Key_Left]: Buttons.Left,
            [Qt.Key_Right]: Buttons.Right,
            [Qt.Key_Up]: Buttons.UpRegen,
            [Qt.Key_Down]: Buttons.DownRegen,
            [Qt.Key_Return]: Buttons.Enter,
            [Qt.Key_Enter]: Buttons.Enter
        })

    // Deferred so the list isn't destroyed inside its own handler; skipped if the VCU moved meanwhile
    function openLater(page: string) {
        const index = vcu.navIndex;
        Qt.callLater(() => {
            if (vcu.navIndex === index && !vcu.homeMenu)
                entryPage = page;
        });
    }

    Connections {
        target: vcu
        function onNavIndexChanged() {
            navigation.entryPage = "";
        }
        function onHomeMenuChanged() {
            navigation.entryPage = "";
        }
    }

    Keys.onPressed: event => {
        const button = keyButtons[event.key];
        if (button !== undefined && !event.isAutoRepeat)
            buttons.press(button);
    }

    HeaderView {
        id: header
        ownsDialog: true
    }

    LabelText {
        id: tsmsIndicator
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height * 0.1
        text: vcu.tsOn ? "TSMS - ON" : "TSMS - OFF"
        color: vcu.tsOn ? Theme.goodStatus : Theme.criticalStatus
    }

    VcuStateComponent {
        id: vcuStateComponent
        anchors {
            top: parent.top
            topMargin: parent.height * 0.01
            left: parent.left
            leftMargin: parent.width * 0.61
            right: parent.right
            rightMargin: parent.width * 0.05
        }
        visible: !navigation.showingPage
        // An index firmware has but this table doesn't yet keeps the menu up
        notice: navigation.current === undefined ? "NO PAGE " + vcu.navIndex : ""
    }

    ColumnLayout {
        id: navContainer
        anchors {
            top: header.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: parent.width * 0.02
            bottomMargin: parent.height * 0.1
        }
        visible: !navigation.showingPage
        spacing: itemSpacing

        property int maxPerRow: Math.max(1, Math.floor((width + itemSpacing) / (navigation.boxSize + itemSpacing)))

        Repeater {
            model: Math.ceil(navigation.pages.length / navContainer.maxPerRow)

            RowLayout {
                id: row
                required property int index
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                spacing: itemSpacing

                Repeater {
                    model: navigation.pages.slice(row.index * navContainer.maxPerRow, (row.index + 1) * navContainer.maxPerRow)

                    Item {
                        id: tile
                        required property var modelData
                        readonly property bool open: navigation.expanded && vcu.navIndex === modelData.index
                        Layout.preferredWidth: navigation.boxSize
                        Layout.preferredHeight: navigation.boxSize

                        HomeIcon {
                            anchors.fill: parent
                            visible: !tile.open
                            highlighted: vcu.navIndex === tile.modelData.index
                            text: tile.modelData.title
                            source: "/qt/qml/content/images/" + tile.modelData.icon
                        }

                        Loader {
                            anchors.centerIn: parent
                            active: tile.open
                            sourceComponent: MenuList {
                                entries: tile.modelData.entries
                                onChosen: entry => entry.page ? navigation.openLater(entry.page) : Theme.setTheme(entry.theme)
                            }
                        }
                    }
                }
            }
        }
    }

    Loader {
        anchors.fill: parent
        active: navigation.openPage !== ""
        source: active ? "/qt/qml/content/" + navigation.openPage : ""
    }
}
