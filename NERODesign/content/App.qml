// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import NERO
import QtQuick.Timeline 1.0

Window {
    width: 800
    height: 480
    color: "black"

    visible: true
    title: "NERO"

    FontLoader {
            id: webFont
            source: "fonts/Roboto-Black.ttf"
        }
    Column {
        anchors.centerIn: parent
        Spedometer {
            id: spedometer
            value: 20
            width: 400
            visible: true
            verticalPadding: 50
            mainTextTopPadding: 10
        }

        Battery {
            id: battery
            height: 100
            width: 50
            value: 100
        }
    }

    Timeline {
        id: appTimeline
        animations: [
            TimelineAnimation {
                id: appAnimation
                pingPong: true
                running: true
                loops: -1
                duration: 5000
                to: 5000
                from: 0
            }
        ]
        startFrame: 0
        endFrame: 5000
        enabled: true

        KeyframeGroup {
            target: spedometer
            property: "value"
            Keyframe {
                value: 0
                frame: 0
            }

            Keyframe {
                value: 100
                frame: 5000
            }
        }

        KeyframeGroup {
            target: battery
            property: "value"
            Keyframe {
                value: 0
                frame: 0
            }

            Keyframe {
                value: 100
                frame: 4988
            }
        }
    }

}

