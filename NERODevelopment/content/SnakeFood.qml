import QtQuick
import QtQuick.Controls

Item {
    id: food
    property int dimension: 20

    width: dimension
    height: dimension

    Rectangle {
        id: foodColor
        height: dimension
        width: dimension
        color: "red"
    }
}
