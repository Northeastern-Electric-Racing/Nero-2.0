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

    readonly property string reasonText: root.rejectionReasons.join("  |  ")

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
        wrapMode: Text.NoWrap
        text: root.reasonText
        font.pixelSize: 14
        color: root.cautionColor
        visible: root.rejected
    }
}
