import QtQuick

Rectangle {
    id: game2048

    anchors.fill: parent
    focus: game2048.isFocused

    property bool isFocused: false
    property var board: []
    property int score: 0
    property int bestScore: 0
    property bool hasWon: false
    property bool animating: false
    property var slotMap: ({})

    color: "black"

    // Tile color map
    property var tileColors: ({
            0: "#cdc1b4",
            2: "#eee4da",
            4: "#ede0c8",
            8: "#f2b179",
            16: "#f59563",
            32: "#f67c5f",
            64: "#f65e3b",
            128: "#edcf72",
            256: "#edcc61",
            512: "#edc850",
            1024: "#edc53f",
            2048: "#edc22e"
        })

    function tileColor(value) {
        return tileColors[value] || "#3c3a32";
    }

    function textColor(value) {
        return value <= 4 ? "#776e65" : "#f9f6f2";
    }

    function fontSize(value, tileSize) {
        var base = tileSize * 0.38;
        if (value >= 4096)
            return base * 0.55;
        if (value >= 1024)
            return base * 0.64;
        if (value >= 128)
            return base * 0.82;
        return base;
    }

    function idx(row, col) {
        return row * 4 + col;
    }

    function spawnTile() {
        var empty = [];
        for (var i = 0; i < 16; i++) {
            if (board[i] === 0)
                empty.push(i);
        }
        if (empty.length === 0)
            return;
        var index = empty[Math.floor(Math.random() * empty.length)];
        board[index] = Math.random() < 0.9 ? 2 : 4;
    }

    function acquireSlot(row, col, value) {
        for (var i = 0; i < 16; i++) {
            var t = tilePool.itemAt(i);
            if (!t.visible) {
                t.animate = false;
                t.gridRow = row;
                t.gridCol = col;
                t.value = value;
                t.scale = 1;
                t.visible = true;
                return i;
            }
        }
        return -1;
    }

    function releaseSlot(slotIndex) {
        var t = tilePool.itemAt(slotIndex);
        t.visible = false;
        t.animate = false;
    }

    function clearAllSlots() {
        for (var i = 0; i < 16; i++) {
            var t = tilePool.itemAt(i);
            t.visible = false;
            t.animate = false;
        }
        slotMap = {};
    }

    function slideLineWithMeta(line, sourceIndices) {
        var compact = [];
        var compactSrc = [];
        for (var i = 0; i < 4; i++) {
            if (line[i] !== 0) {
                compact.push(line[i]);
                compactSrc.push(sourceIndices[i]);
            }
        }

        var result = [];
        var sources = [];
        var mergeScore = 0;

        var k = 0;
        while (k < compact.length) {
            if (k + 1 < compact.length && compact[k] === compact[k + 1]) {
                result.push(compact[k] * 2);
                sources.push({
                    primary: compactSrc[k],
                    consumed: compactSrc[k + 1]
                });
                mergeScore += compact[k] * 2;
                k += 2;
            } else {
                result.push(compact[k]);
                sources.push({
                    primary: compactSrc[k],
                    consumed: null
                });
                k++;
            }
        }

        while (result.length < 4) {
            result.push(0);
            sources.push(null);
        }

        return {
            result: result,
            sources: sources,
            mergeScore: mergeScore
        };
    }

    function move(direction) {
        var oldBoard = board.slice();
        var totalMergeScore = 0;
        var allSources = [];

        for (var i = 0; i < 4; i++) {
            var line = [];
            var srcIndices = [];

            for (var j = 0; j < 4; j++) {
                switch (direction) {
                case 0:
                    line.push(board[idx(j, i)]);
                    srcIndices.push(idx(j, i));
                    break;
                case 1:
                    line.push(board[idx(i, 3 - j)]);
                    srcIndices.push(idx(i, 3 - j));
                    break;
                case 2:
                    line.push(board[idx(3 - j, i)]);
                    srcIndices.push(idx(3 - j, i));
                    break;
                case 3:
                    line.push(board[idx(i, j)]);
                    srcIndices.push(idx(i, j));
                    break;
                }
            }

            var meta = slideLineWithMeta(line, srcIndices);
            totalMergeScore += meta.mergeScore;

            for (j = 0; j < 4; j++) {
                var destIdx;
                switch (direction) {
                case 0:
                    destIdx = idx(j, i);
                    break;
                case 1:
                    destIdx = idx(i, 3 - j);
                    break;
                case 2:
                    destIdx = idx(3 - j, i);
                    break;
                case 3:
                    destIdx = idx(i, j);
                    break;
                }

                if (meta.sources[j]) {
                    allSources.push({
                        dest: destIdx,
                        primary: meta.sources[j].primary,
                        consumed: meta.sources[j].consumed,
                        value: meta.result[j]
                    });
                }
                board[destIdx] = meta.result[j];
            }
        }

        // Check if anything changed
        var changed = false;
        for (i = 0; i < 16; i++) {
            if (board[i] !== oldBoard[i]) {
                changed = true;
                break;
            }
        }
        if (!changed)
            return;
        animating = true;

        // Build new slotMap
        var newSlotMap = {};
        var toRelease = [];

        for (i = 0; i < allSources.length; i++) {
            var src = allSources[i];
            var destRow = Math.floor(src.dest / 4);
            var destCol = src.dest % 4;

            var primarySlot = slotMap[src.primary];
            var tile = tilePool.itemAt(primarySlot);
            tile.animate = true;
            tile.gridRow = destRow;
            tile.gridCol = destCol;
            newSlotMap[src.dest] = primarySlot;

            if (src.consumed !== null) {
                var consumedSlot = slotMap[src.consumed];
                var consumedTile = tilePool.itemAt(consumedSlot);
                consumedTile.animate = true;
                consumedTile.gridRow = destRow;
                consumedTile.gridCol = destCol;
                toRelease.push({
                    slot: consumedSlot,
                    value: src.value,
                    destSlot: primarySlot
                });
            }
        }

        slotMap = newSlotMap;
        score += totalMergeScore;
        if (score > bestScore)
            bestScore = score;

        animTimer.toRelease = toRelease;
        animTimer.start();
    }

    function canMove() {
        for (var i = 0; i < 16; i++) {
            if (board[i] === 0)
                return true;
        }
        for (var row = 0; row < 4; row++) {
            for (var col = 0; col < 4; col++) {
                var v = board[idx(row, col)];
                if (col < 3 && v === board[idx(row, col + 1)])
                    return true;
                if (row < 3 && v === board[idx(row + 1, col)])
                    return true;
            }
        }
        return false;
    }

    function startGame() {
        clearAllSlots();
        board = [];
        for (var i = 0; i < 16; i++)
            board.push(0);
        score = 0;
        hasWon = false;
        showWin = false;
        animating = false;
        animTimer.stop();
        spawnTile();
        spawnTile();
        for (var i = 0; i < 16; i++) {
            if (board[i] !== 0) {
                var slot = acquireSlot(Math.floor(i / 4), i % 4, board[i]);
                slotMap[i] = slot;
            }
        }
        game2048Controller.setScore(0);
        game2048Controller.setGameOver(false);
    }

    Component.onCompleted: startGame()

    property bool showWin: false

    Timer {
        id: animTimer
        interval: 120
        repeat: false
        property var toRelease: []

        onTriggered: {
            for (var i = 0; i < toRelease.length; i++) {
                releaseSlot(toRelease[i].slot);
                var mergedTile = tilePool.itemAt(toRelease[i].destSlot);
                mergedTile.value = toRelease[i].value;
                mergedTile.pop();
            }
            toRelease = [];

            spawnTile();
            for (var i = 0; i < 16; i++) {
                if (board[i] !== 0 && slotMap[i] === undefined) {
                    var row = Math.floor(i / 4);
                    var col = i % 4;
                    var slot = acquireSlot(row, col, board[i]);
                    slotMap[i] = slot;
                    tilePool.itemAt(slot).spawn();
                    break;
                }
            }

            game2048Controller.setScore(score);

            if (!hasWon) {
                for (var i = 0; i < 16; i++) {
                    if (board[i] === 2048) {
                        hasWon = true;
                        showWin = true;
                        game2048Controller.setGameOver(true);
                        break;
                    }
                }
            }

            if (!canMove()) {
                game2048Controller.setGameOver(true);
                game2048Controller.saveScore(score);
            }

            animating = false;
        }
    }

    property real scoreAreaHeight: 48
    property real gridSize: parent.height - scoreAreaHeight

    // Score area
    Row {
        id: scoreRow
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: container.width
        height: game2048.scoreAreaHeight
        spacing: 0

        Rectangle {
            width: parent.width / 2
            height: parent.height
            color: "#cdc1b4"

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                text: "SCORE"
                font.pixelSize: 14
                font.bold: true
                color: "#776e65"
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                text: game2048.score
                font.pixelSize: 18
                font.bold: true
                color: "white"
            }
        }

        Rectangle {
            width: parent.width / 2
            height: parent.height
            color: "#cdc1b4"

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                text: "BEST"
                font.pixelSize: 14
                font.bold: true
                color: "#776e65"
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                text: game2048.bestScore
                font.pixelSize: 18
                font.bold: true
                color: "white"
            }
        }
    }

    // Game container
    Rectangle {
        id: container
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: game2048.gridSize
        height: game2048.gridSize
        color: "#bbada0"

        property real spacing: 8
        property real tileSize: (width - spacing * 5) / 4

        // Background grid
        Grid {
            id: bgGrid
            anchors.fill: parent
            anchors.margins: container.spacing
            columns: 4
            spacing: container.spacing

            Repeater {
                model: 16
                Rectangle {
                    width: container.tileSize
                    height: container.tileSize
                    radius: 3
                    color: "#cdc1b4"
                }
            }
        }

        // Tile layer
        Item {
            id: tileLayer
            anchors.fill: parent

            Repeater {
                id: tilePool
                model: 16

                Rectangle {
                    id: tile
                    visible: false

                    property int gridRow: 0
                    property int gridCol: 0
                    property int value: 2
                    property bool animate: false

                    x: container.spacing + gridCol * (container.tileSize + container.spacing)
                    y: container.spacing + gridRow * (container.tileSize + container.spacing)
                    width: container.tileSize
                    height: container.tileSize
                    radius: 3
                    color: tileColor(value)
                    z: 2

                    Behavior on x {
                        enabled: animate
                        NumberAnimation {
                            duration: 100
                            easing.type: Easing.InOutQuad
                        }
                    }
                    Behavior on y {
                        enabled: animate
                        NumberAnimation {
                            duration: 100
                            easing.type: Easing.InOutQuad
                        }
                    }

                    SequentialAnimation {
                        id: popAnim
                        NumberAnimation {
                            target: tile
                            property: "scale"
                            from: 0
                            to: 1.2
                            duration: 100
                            easing.type: Easing.OutQuad
                        }
                        NumberAnimation {
                            target: tile
                            property: "scale"
                            to: 1.0
                            duration: 100
                            easing.type: Easing.InQuad
                        }
                    }

                    NumberAnimation {
                        id: spawnAnim
                        target: tile
                        property: "scale"
                        from: 0
                        to: 1
                        duration: 200
                        easing.type: Easing.OutQuad
                    }

                    function pop() {
                        popAnim.start();
                    }
                    function spawn() {
                        scale = 0;
                        spawnAnim.start();
                    }

                    Text {
                        anchors.centerIn: parent
                        text: parent.value > 0 ? parent.value : ""
                        font.pixelSize: fontSize(parent.value, container.tileSize)
                        font.bold: true
                        color: textColor(parent.value)
                    }
                }
            }
        }
    }

    // Game over overlay
    Rectangle {
        anchors.fill: container
        color: Qt.rgba(0.93, 0.89, 0.85, 0.5)
        visible: game2048Controller.gameOver && score > 0
        z: 10

        Text {
            anchors.centerIn: parent
            text: "Game Over!\nPress Enter to restart"
            font.pixelSize: 24
            font.bold: true
            color: "#776e65"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    // Win overlay
    Rectangle {
        anchors.fill: container
        color: Qt.rgba(0.93, 0.76, 0.18, 0.5)
        visible: game2048.showWin
        z: 10

        Text {
            anchors.centerIn: parent
            text: "You Win!\nPress Enter to continue"
            font.pixelSize: 30
            font.bold: true
            color: "#f9f6f2"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    function handleMove(direction) {
        if (animating)
            return;
        if (showWin)
            return;
        move(direction);
    }

    function dismissWin() {
        showWin = false;
        game2048Controller.setGameOver(false);
    }

    Keys.onUpPressed: game2048Controller.upButtonPressed()
    Keys.onDownPressed: game2048Controller.downButtonPressed()
    Keys.onLeftPressed: game2048Controller.leftButtonPressed()
    Keys.onRightPressed: game2048Controller.rightButtonPressed()
    Keys.onReturnPressed: game2048Controller.enterButtonPressed()

    Connections {
        target: game2048Controller
        function onMoveMade(direction) {
            handleMove(direction);
        }
        function onRestartRequested() {
            if (showWin)
                dismissWin();
            else
                startGame();
        }
    }
}
