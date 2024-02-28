import QtQuick 2.15

Item {
    id: root

    // public
    property bool password: false

    signal accepted(string text)

    // onAccepted: print('onAccepted', text)
    function show() {
        keyboard = keyboardComponent.createObject(null, {
                                                      "password": root.password
                                                  })

        var rootObject = null, object = parent // search up the parent chain to find QQuickView::rootObject()
        while (object) {
            if (object)
                rootObject = object
            object = object.parent
        }

        keyboard.parent = rootObject
        keyboard.width = rootObject.width // resize
        keyboard.height = rootObject.height
        keyboard.focus = true
    }

    // private
    property Item keyboard: null
    Component {
        id: keyboardComponent
        Keyboard {}
    }

    Connections {
        target: keyboard

        onAccepted: {
            root.accepted(text) // emit
            keyboard.destroy() // hide
        }

        onRejected: keyboard.destroy() // hide
    }
}
