import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.6
import NERO

Item {
    id: chartItem
    width: 800
    height: 480

    property string chartTitle: ""

    property int minX: 0
    property int maxX: debugGraphController.graphData.length
    property string xLabel: "TIME (MS)"

    property int minY: debugGraphController.minY
    property int maxY: debugGraphController.maxY
    property string yLabel: ""

    property var dummyData: debugGraphController.graphData

    onDummyDataChanged: {
        console.log(maxX)
        series.clear()
    }

    onVisibleChanged: {
        debugGraphController.setTitle(chartTitle)
    }
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
            tickCount: chartItem.maxX - chartItem.minX + 1
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
            tickCount: 5
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
                series.append(modelData.x, modelData.y)
            }
        }
    }
}
