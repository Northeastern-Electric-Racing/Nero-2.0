import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: snakeGame

    anchors.fill: parent
    focus: snakeGame.isFocused
    visible: true

    property bool isFocused: false

    width: 800
    height: 480
    color: "black"

    property int tileSize: 20
    property int gridWidth: width / tileSize
    property int gridHeight: height / tileSize

    property var snakeBody: []
    property var food: { 'x': 0, 'y': 0 }
    property int direction: 1  // 0: Up, 1: Right, 2: Down, 3: Left
    property bool gameOver: false
    property int score: 0

    // Initialize the game when the component is ready
    Component.onCompleted: startGame()

    // Function to start or restart the game
    function startGame() {
        snakeBody = [{ x: Math.floor(gridWidth / 2), y: Math.floor(gridHeight / 2) }]
        direction = 1  // Start moving right
        gameOver = false
        score = 0
        placeFood()
        gameTimer.start()
        updateSnakeModel()
    }

    // Place food at a random position not occupied by the snake
    function placeFood() {
        do {
            food.x = Math.floor(Math.random() * gridWidth)
            food.y = Math.floor(Math.random() * gridHeight)
        } while (isSnakePosition(food.x, food.y))
    }

    // Check if a given position is occupied by the snake
    function isSnakePosition(x, y) {
        for (let i = 0; i < snakeBody.length; i++) {
            if (snakeBody[i].x === x && snakeBody[i].y === y) {
                return true
            }
        }
        return false
    }

    // Check for collisions with walls or self
    function checkCollision(x, y) {
        // Check walls
        if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
            return true
        }
        // Check self-collision
        for (let i = 1; i < snakeBody.length; i++) {
            if (snakeBody[i].x === x && snakeBody[i].y === y) {
                return true
            }
        }
        return false
    }

    // Update the game state on each timer tick
    function updateGame() {
        if (gameOver) {
            gameTimer.stop()
            snakeController.saveScore(score)
            return
        }

        let head = { x: snakeBody[0].x, y: snakeBody[0].y }

        switch (direction) {
            case 0: // Up
                head.y -= 1
                break
            case 1: // Right
                head.x += 1
                break
            case 2: // Down
                head.y += 1
                break
            case 3: // Left
                head.x -= 1
                break
        }

        if (checkCollision(head.x, head.y)) {
            gameOver = true
            return
        }

        snakeBody.unshift(head)

        if (head.x === food.x && head.y === food.y) {
            score += 1
            placeFood()
        } else {
            snakeBody.pop()
        }

        updateSnakeModel()
    }

    // Update the snake model for the Repeater
    function updateSnakeModel() {
        snakeModel.clear()
        for (let segment of snakeBody) {
            snakeModel.append({ "x": segment.x, "y": segment.y })
        }
    }

    // Timer to drive the game loop
    Timer {
        id: gameTimer
        interval: 100
        repeat: true
        running: false
        onTriggered: updateGame()
    }

    // Handle key presses via snakeController
    Keys.onPressed: {
        snakeController.handleKeyPress(event.key)
        if (event.key === Qt.Key_Return && gameOver) {
            startGame()
        }
    }

    // ListModel to store snake segments
    ListModel {
        id: snakeModel
    }

    // Display the snake using the SnakeBody component
    Repeater {
        model: snakeModel
        delegate: SnakeBody {
            x: model.x * tileSize
            y: model.y * tileSize
            dimension: tileSize
        }
    }

    // Display the food using the SnakeFood component
    SnakeFood {
        x: food.x * tileSize
        y: food.y * tileSize
        dimension: tileSize
    }

    // Display game over text when the game ends
    Text {
        id: gameOverText
        anchors.centerIn: parent
        text: gameOver ? "Game Over\nPress Enter to Restart" : ""
        color: "white"
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: gameOver
    }

    // Display the current score
    Text {
        id: scoreText
        text: "Score: " + score
        font.pixelSize: 24
        color: "white"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
    }

    // Integrate with the snakeController
    Connections {
        target: snakeController
        onDirectionChanged: {
            direction = newDirection
        }
    }
}
