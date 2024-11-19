import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels
import NERO

Item {
    id: debugTable
    anchors.fill: parent

    property variant topics: debugTableController.topics
    property variant values: debugTableController.selectedValues
    property int selectedTopicIndex: debugTableController.selectedTopicsIndex
    property int selectedValueIndex: debugTableController.selectedValuesIndex
    property bool scrollingTopics: debugTableController.scrollingTopics
    property bool showGraph: debugTableController.showGraph

    property int rowHeight: 30

    Keys.onPressed: event => {
                        switch (event.key) {
                            case Qt.Key_Up:
                            debugTableController.upButtonPressed()
                            break
                            case Qt.Key_Left:
                            debugTableController.leftButtonPressed()
                            break
                            case Qt.Key_Right:
                            debugTableController.rightButtonPressed()
                            break
                            case Qt.Key_Down:
                            debugTableController.downButtonPressed()
                            break
                            case Qt.Key_Return:
                            debugTableController.enterButtonPressed()
                            break
                            default:
                            break
                        }
                    }

    Row {
        anchors.fill: parent
        spacing: 1

        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width / 4
            color: 'black'

            HorizontalHeaderView {
                id: topicsHeader
                anchors.left: parent.left
                anchors.top: parent.top
                syncView: topicsTableView
                model: ["Topic"]
                clip: true
            }

            TableView {
                id: topicsTableView
                anchors.top: topicsHeader.bottom
                anchors.bottom: parent.bottom
                width: parent.width
                columnSpacing: 1
                rowSpacing: 1
                clip: true

                model: TableModel {
                    TableModelColumn {
                        display: "topic"
                    }

                    rows: debugTable.topics
                }

                delegate: Rectangle {
                    implicitWidth: topicsTableView.width
                    implicitHeight: debugTable.rowHeight
                    border.width: scrollingTopics
                                  && model.index === debugTable.selectedTopicIndex ? 3 : 1
                    color: 'black'
                    border.color: 'white'

                    LabelText {
                        text: display
                        anchors.centerIn: parent
                    }
                }
            }
        }

        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width * 3 / 4
            color: 'black'

            HorizontalHeaderView {
                id: valuesHeader
                anchors.left: parent.left
                anchors.top: parent.top
                syncView: valuesTableView
                model: ["Name", "Value", "Unit"]
                clip: true
            }

            TableView {
                id: valuesTableView
                anchors.top: valuesHeader.bottom
                anchors.bottom: parent.bottom
                width: parent.width
                columnSpacing: 1
                rowSpacing: 1
                clip: true

                model: TableModel {
                    TableModelColumn {
                        display: "name"
                    }
                    TableModelColumn {
                        display: "value"
                    }
                    TableModelColumn {
                        display: "unit"
                    }

                    rows: debugTable.values
                }

                delegate: Rectangle {
                    implicitWidth: parent.width / 3
                    implicitHeight: debugTable.rowHeight
                    border.width: !scrollingTopics
                                  && model.index === debugTable.selectedValueIndex ? 3 : 1
                    color: 'black'
                    border.color: 'white'

                    LabelText {
                        anchors.centerIn: parent
                        text: display
                    }
                }

                property var columnWidths: [valuesTableView.width
                    / 2, valuesTableView.width / 4, valuesTableView.width / 4]
                columnWidthProvider: function (column) {
                    return columnWidths[column]
                }
            }
        }
    }

    DebugGraph {
        visible: showGraph
        chartTitle: values[selectedValueIndex]["id"]
        yLabel: values[selectedValueIndex]["unit"]
        anchors.fill: parent
    }
}
