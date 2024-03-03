import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Item {
    id: timerDisplay
    property int dimension: 500

    width: dimension
    height: dimension * 0.8

    GridLayout {
        width: parent.width
        height: parent.height
        columns: 1

        anchors {
            top: parent.top
            left: parent.left
        }

        RunInfo {
            Layout.row: 0
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "#14ff00"
            dimension: timerDisplay.dimension
            label: "CURRENT RUN"
            value: "0"
            radius: timerDisplay.dimension / 20
            isTop: true
        }

        RunInfo {
            Layout.row: 1
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "#ffffff"
            dimension: timerDisplay.dimension
            label: "LAST RUN"
            value: "0"
        }

        RunInfo {
            Layout.row: 2
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
            backgroundColor: "#ad00ff"
            dimension: timerDisplay.dimension
            label: "FASTEST RUN"
            value: "0"
            radius: timerDisplay.dimension / 20
            isBottom: true
        }
    }
}
