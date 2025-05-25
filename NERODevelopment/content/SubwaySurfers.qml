import QtQuick
import QtQuick.Controls
import QtQuick3D
import SubwaySurfersGameState 1.0
import QtQuick3D.Helpers
import QtQuick3D.AssetUtils

View3D {
    id: view3D
    focus: true

    property int roadWidthScale: 500
    property int trainSpeed: 4000

    property int laneScale: roadWidthScale / SubwaySurfersGameState.laneCount

    environment: SceneEnvironment {
        clearColor: "skyblue"
        backgroundMode: SceneEnvironment.Color
    }

    camera: PerspectiveCamera {
        id: camera
        property real cameraDistance: 300
        property real cameraHeight: 300

        position: Qt.vector3d(playerWrapper.position.x,
                              playerWrapper.position.y + cameraHeight,
                              playerWrapper.position.z + cameraDistance)
        // Make the camera always look at the player
        onPositionChanged: {
            camera.lookAt(playerWrapper.position)
        }

        fieldOfView: 60
    }
    DirectionalLight {
        eulerRotation.x: -30
        eulerRotation.y: -70
    }

    Node {
        id: sceneRoot

        Model {
            source: "#Cube"
            scale: Qt.vector3d(roadWidthScale, 0.2, 200)
            position: Qt.vector3d(0, 0, 0)
            materials: DefaultMaterial {
                diffuseColor: "green"
            }
        }

        Node {
            id: playerWrapper
            position: Qt.vector3d(0, 100, 15)

            property int currentLane: 1
            property bool inJump: false

            RuntimeLoader {
                id: player
                source: "qrc:/content/models/naruto.glb"
            }
        }
    }

    Keys.onPressed: e => {
                        if (e.key === Qt.Key_Left
                            && playerWrapper.currentLane > 0) {
                            playerWrapper.currentLane--
                            playerWrapper.position.x = playerWrapper.currentLane
                            * laneScale - laneScale
                        } else if (e.key === Qt.Key_Right
                                   && playerWrapper.currentLane
                                   < SubwaySurfersGameState.laneCount - 1) {
                            playerWrapper.currentLane++
                            playerWrapper.position.x = playerWrapper.currentLane
                            * laneScale - laneScale
                        } else if (e.key === Qt.Key_Up
                                   && !playerWrapper.inJump) {
                            playerWrapper.inJump = true
                            jumpAnim.start()
                        }
                        if (e.key === Qt.Key_Return
                            && !SubwaySurfersGameState.gameRunning) {
                            SubwaySurfersGameState.gameRunning = true
                        }
                    }

    NumberAnimation {
        id: jumpAnim
        target: playerWrapper
        property: "position.y"
        from: 100
        to: 300
        duration: 300
        easing.type: Easing.OutQuad
        onStopped: dropAnim.start()
    }

    NumberAnimation {
        id: dropAnim
        target: playerWrapper
        property: "position.y"
        from: 300
        to: 100
        duration: 300
        easing.type: Easing.InQuad
        onStopped: playerWrapper.inJump = false
    }

    Component {
        id: obstacleComponent
        Model {
            source: "#Cube"
            scale: Qt.vector3d(1, 5, 10)
            materials: DefaultMaterial {
                diffuseColor: "black"
            }

            SequentialAnimation on position.z {
                loops: 1
                NumberAnimation {
                    from: -2000
                    to: 1000
                    duration: 4000
                    easing.type: Easing.Linear
                }
            }
        }
    }

    Item {
        id: spawner
        property var obstacles: []

        Timer {
            interval: 3000
            running: SubwaySurfersGameState.gameRunning
            repeat: true
            onTriggered: {
                const lane = Math.floor(Math.random(
                                            ) * 3) * laneScale - laneScale
                const obj = obstacleComponent.createObject(sceneRoot, {
                                                               "position": Qt.vector3d(
                                                                               lane, 50, -1000)
                                                           })
                spawner.obstacles.push(obj)
            }
        }

        Timer {
            interval: 16
            running: true
            repeat: true
            onTriggered: {
                for (var i = spawner.obstacles.length - 1; i >= 0; --i) {
                    const obs = spawner.obstacles[i]
                    if (!obs || !obs.position || !playerWrapper)
                        continue

                    if (obs.position.z === 1000) {
                        obs.destroy()
                        spawner.obstacles.splice(i, 1)
                        view3D.trainSpeed -= 10
                        continue
                    }

                    const halfPlayer = {
                        "x": player.scale.x / 2 * 100,
                        "y": player.scale.y / 2 * 100,
                        "z": player.scale.z / 2 * 100
                    }

                    const halfObs = {
                        "x": obs.scale.x / 2 * 100,
                        "y": obs.scale.y / 2 * 100,
                        "z": obs.scale.z / 2 * 100
                    }

                    const dx = Math.abs(
                                 obs.position.x - playerWrapper.position.x)
                    const dy = Math.abs(
                                 obs.position.y - playerWrapper.position.y)
                    const dz = Math.abs(
                                 obs.position.z - playerWrapper.position.z)

                    console.log(dx, halfObs.x + halfPlayer.x, dy,
                                halfObs.y + halfPlayer.y, dz,
                                halfObs.z + halfPlayer.z)
                    // Check for overlap on all axes
                    if (dx < (halfObs.x + halfPlayer.x)
                            && dy < (halfObs.y + halfPlayer.y)
                            && dz < (halfObs.z + halfPlayer.z)) {
                        console.log("💥 Collision!")
                        SubwaySurfersGameState.gameRunning = false
                        view3D.trainSpeed = 4000
                        obs.destroy()
                    }
                }
            }
        }
    }
}
