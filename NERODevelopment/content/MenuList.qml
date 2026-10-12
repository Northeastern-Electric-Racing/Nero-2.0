import QtQuick
import Main

Column {
    id: list

    required property var entries
    property int selected: 0

    signal chosen(var entry)

    spacing: 10

    Repeater {
        model: list.entries

        SubMenuIcon {
            required property var modelData
            required property int index
            highlighted: index === list.selected
            text: modelData.title
        }
    }

    Connections {
        target: buttons
        function onPressed(button) {
            const step = {
                [Buttons.UpRegen]: -1,
                [Buttons.Left]: -1,
                [Buttons.DownRegen]: 1,
                [Buttons.Right]: 1
            }[button];
            if (step !== undefined)
                list.selected = (list.selected + step + list.entries.length) % list.entries.length;
            else if (button === Buttons.Enter)
                list.chosen(list.entries[list.selected]);
        }
    }
}
