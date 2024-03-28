import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts

Item {
    id: debugGraph
    width: 800
    height: 480

    ChartView {
        title: "AVERAGE CELL TEMPERATURE"
        anchors.fill: parent
        antialiasing: true
        LineSeries {

        }
    }
}
