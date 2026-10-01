import QtQuick
import NERO

Item {
    id: root

    readonly property color absentColor: Theme.mutedForeground
    readonly property color activeColor: Theme.goodStatus
    readonly property color cautionColor: Theme.cautionStatus
    readonly property color faultColor: Theme.criticalStatus

    readonly property string stateLabel: navigationController ? navigationController.functionalStateLabel : ""
    readonly property bool stateKnown: navigationController ? navigationController.functionalStateKnown : false
    readonly property bool stateFaulted: navigationController ? navigationController.functionalStateFaulted : false
    readonly property bool rejected: navigationController ? navigationController.stateRejectionActive : false
    readonly property var rejectionReasons: navigationController ? navigationController.stateRejectionReasons : []

    readonly property int reasonFontSize: 14
    readonly property int reasonLineCount: 2
    readonly property string reasonSeparator: "  |  "

    readonly property string reasonText: {
        const all = root.rejectionReasons;
        if (all.length === 0)
            return "";
        const budget = root.width * root.reasonLineCount;
        let shown = all[0];
        let count = 1;
        while (count < all.length) {
            const merged = shown + root.reasonSeparator + all[count];
            const hidden = all.length - count - 1;
            const trial = hidden > 0 ? merged + root.reasonSeparator + "+" + hidden : merged;
            if (reasonMetrics.advanceWidth(trial) > budget)
                break;
            shown = merged;
            count += 1;
        }
        if (count === all.length)
            return shown;
        return shown + root.reasonSeparator + "+" + (all.length - count);
    }

    FontMetrics {
        id: reasonMetrics
        font.family: webFont.name
        font.pixelSize: root.reasonFontSize
    }

    implicitHeight: reasonLine.visible ? reasonLine.y + reasonLine.height : stateText.height

    LabelText {
        id: stateText
        anchors.right: parent.right
        y: 0
        width: parent.width
        horizontalAlignment: Text.AlignRight
        elide: Text.ElideRight
        text: root.stateLabel
        font.pixelSize: 20
        color: root.stateFaulted ? root.faultColor : root.rejected ? root.cautionColor : root.stateKnown ? root.activeColor : root.absentColor
    }

    LabelText {
        id: reasonLine
        anchors.right: parent.right
        y: 19
        width: parent.width
        horizontalAlignment: Text.AlignRight
        elide: Text.ElideRight
        wrapMode: Text.WordWrap
        maximumLineCount: 2
        text: root.reasonText
        font.pixelSize: root.reasonFontSize
        color: root.cautionColor
        visible: root.rejected
    }
}
