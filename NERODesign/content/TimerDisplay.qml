import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Item {
    width: 500
    height: 400

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
            backgroundColor: "green"
            width: parent.width
            height: parent.height * 0.3
            label: "CURRENT RUN"
            value: "0"


        }

        RunInfo {
            Layout.row: 1
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            backgroundColor: "white"
            width: parent.width
            height: parent.height * 0.3
            label: "LAST RUN"
            value: "0"


        }

        RunInfo {
            Layout.row: 2
            Layout.column: 0
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            backgroundColor: "purple"
            width: parent.width
            height: parent.height * 0.3
            label: "FASTEST RUN"
            value: "0"

        }
    }

}
