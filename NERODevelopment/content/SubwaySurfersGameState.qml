pragma Singleton

import QtQuick 2.15

QtObject {
    property int currentLane: 1
    property bool gameRunning: true
    property int laneCount: 3
    property int score: 0
    property string playerState: "running" // "jumping", "ducking"
}
