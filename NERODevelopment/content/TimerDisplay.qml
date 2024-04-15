import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Item {
    id: timerDisplay
    property int widthValue: 500
    property int heightValue: 400
    property int currentRunTime: 0
    property int lastRunTime: 0
    property int fastestRunTime: 0

    width: widthValue
    height: heightValue

    ColumnLayout {
        width: timerDisplay.width
        height: timerDisplay.height

        anchors {
            top: parent.top
            left: parent.left
        }

        RunInfo {
            Layout.row: 0
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "#14ff00"
            widthValue: timerDisplay.width
            heightValue: timerDisplay.height / 3.3
            label: "CURRENT RUN"
            value: timerDisplay.currentRunTime
            radius: Math.min(timerDisplay.height / 7, timerDisplay.width / 20)
            isTop: true
        }

        RunInfo {
            Layout.row: 1
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "#ffffff"
            widthValue: timerDisplay.width
            heightValue: timerDisplay.height / 3.3
            label: "LAST RUN"
            value: timerDisplay.lastRunTime
        }

        RunInfo {
            Layout.row: 2
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "#ad00ff"
            widthValue: timerDisplay.width
            heightValue: timerDisplay.height / 3.3
            label: "FASTEST RUN"
            value: timerDisplay.fastestRunTime
            radius: Math.min(timerDisplay.height / 7, timerDisplay.width / 20)
            isBottom: true
        }
    }
}
