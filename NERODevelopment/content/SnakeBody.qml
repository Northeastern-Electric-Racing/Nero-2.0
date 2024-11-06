import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: food
    property int dimension: 20

    width: dimension
    height: dimension

    Rectangle {
        id: foodColor
        height: dimension
        width: dimension
        color: "lime"
    }
}
