import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.6

Item {
    id: chartItem
    width: 800
    height: 480

    property string chartTitle: "AVERAGE CELL TEMPERATURE"

    property int minX: 0
    property int maxX: 20
    property string xLabel: "TIME (MS)"

    property int minY: 0
    property int maxY: 50
    property string yLabel: "TEMPERATURE (C)"

    property var dummyData: [
        {"x": 0, "y": 30},
        {"x": 4, "y": 15},
        {"x": 6, "y": 30},
        {"x": 8, "y": 15},
        {"x": 13, "y": 40},
        {"x": 18, "y": 15},
        {"x": 20, "y": 27}
    ]

    ChartView {
        anchors.fill: parent
        backgroundColor: "black"
        title: chartItem.chartTitle
        titleColor: "white"
        legend.visible: false

        ValuesAxis {
            id: xAxis
            min: chartItem.minX
            max: chartItem.maxX
            gridVisible: false
            color: "white"
            labelsColor: "white"
            titleText: "<font color='white'>" + chartItem.xLabel + "</font>"
            tickCount: chartItem.maxX + 1
            labelFormat: "%d"
        }

        ValuesAxis {
            id: yAxis
            min: chartItem.minY
            max: chartItem.maxY
            gridVisible: false
            labelsColor: "white"
            color: "white"
            titleText: "<font color='white'>" + chartItem.yLabel + "</font>"
            tickCount: chartItem.maxY / 5 + 1
            labelFormat: "%d"
        }

        LineSeries {
            id: series
            name: "Temperature"
            color: "red"
            axisX: xAxis
            axisY: yAxis
        }
    }

    Repeater {
        model: dummyData
        delegate: Item {
            Component.onCompleted: {
                series.append(modelData.x, modelData.y);
            }
        }
    }
}
