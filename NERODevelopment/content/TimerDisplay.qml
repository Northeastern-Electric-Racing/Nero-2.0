import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NERO

Item {
    id: timerDisplay
    property int currentRunTime: 0
    property int lastRunTime: 0
    property int fastestRunTime: 0
    property int radius: Math.min(timerDisplay.height / 7,
                                  timerDisplay.width / 20)
    property bool vertical: false

    RowLayout {
        width: timerDisplay.width
        height: timerDisplay.height
        spacing: 0
        visible: !timerDisplay.vertical

        anchors {
            top: parent.top
            left: parent.left
        }

        RunInfo {
            Layout.preferredWidth: 6
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: Theme.currentRunBackground
            label: "CURRENT RUN"
            value: timerDisplay.currentRunTime
            radius: timerDisplay.radius
            isLeft: true
        }

        RunInfo {
            Layout.preferredWidth: 5
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: Theme.lastRunBackground
            label: "LAST RUN"
            value: timerDisplay.lastRunTime
        }

        RunInfo {
            Layout.preferredWidth: 6
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: Theme.fastestRunBackground
            label: "FASTEST RUN"
            value: timerDisplay.fastestRunTime
            radius: timerDisplay.radius
            isRight: true
        }
    }

    ColumnLayout {
        visible: timerDisplay.vertical
        spacing: 0
        anchors.fill: parent

        RunInfo {
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: Theme.currentRunBackground
            label: "CURRENT RUN"
            value: timerDisplay.currentRunTime
            radius: timerDisplay.radius
            isTop: true
            vertical: true
        }

        RunInfo {
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: Theme.lastRunBackground
            label: "LAST RUN"
            value: timerDisplay.lastRunTime
            vertical: true
        }

        RunInfo {
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: Theme.fastestRunBackground
            label: "FASTEST RUN"
            value: timerDisplay.fastestRunTime
            radius: timerDisplay.radius
            isBottom: true
            vertical: true
        }
    }
}
