import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Item {
    id: timerDisplay
    property int currentRunTime: 0
    property int lastRunTime: 0
    property int fastestRunTime: 0

    RowLayout {
        width: timerDisplay.width
        height: timerDisplay.height
        spacing: 0

        anchors {
            top: parent.top
            left: parent.left
        }

        RunInfo {
            Layout.row: 0
            Layout.column: 0
            Layout.preferredWidth: 6
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "#47A7FF"
            label: "CURRENT RUN"
            value: timerDisplay.currentRunTime
            radius: Math.min(timerDisplay.height / 7, timerDisplay.width / 20)
            isLeft: true
        }

        RunInfo {
            Layout.row: 0
            Layout.column: 1
            Layout.preferredWidth: 5
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "#ffffff"
            label: "LAST RUN"
            value: timerDisplay.lastRunTime
        }

        RunInfo {
            Layout.row: 0
            Layout.column: 2
            Layout.preferredWidth: 6
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "#AD00FF"
            label: "FASTEST RUN"
            value: timerDisplay.fastestRunTime
            radius: Math.min(timerDisplay.height / 7, timerDisplay.width / 20)
            isRight: true
        }
    }
}
