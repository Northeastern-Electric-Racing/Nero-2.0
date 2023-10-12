import QtQuick

Item {
    id: itm

    height: 100
    width: 100

    Rectangle{
        id: rct

        color: "white"
        height: itm.height
        width: itm.width
    }

    Rectangle{
        id: circle

        color: "green"
        height: itm.height
        width: itm.width
        radius: itm.width
    }
}
