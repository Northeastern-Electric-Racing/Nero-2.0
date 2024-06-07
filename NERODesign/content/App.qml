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

    // Pit {
    //     id: pitScreen
    // }
    // EfficiencyScreen {}
    OffScreen {}
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
            target: pitScreen
            property: "currentSpeed"
            Keyframe {
                value: 0
                frame: 0
            }

            Keyframe {
                value: 5
                frame: 5000
            }
        }

        KeyframeGroup {
            target: pitScreen
            property: "stateOfChargePercentage"
            Keyframe {
                value: 0
                frame: 0
            }

            Keyframe {
                value: 100
                frame: 4988
            }
        }

        KeyframeGroup {
            target: pitScreen
            property: "packTempValue"
            Keyframe {
                value: -20
                frame: 0
            }

            Keyframe {
                value: 50
                frame: 4988
            }
        }

        KeyframeGroup {
            target: pitScreen
            property: "motorTempValue"
            Keyframe {
                value: -20
                frame: 0
            }

            Keyframe {
                value: 50
                frame: 4988
            }
        }

        KeyframeGroup {
            target: pitScreen
            property: "isTalking"
            Keyframe {
                value: true
                frame: 0
            }
            Keyframe {
                value: false
                frame: 4988
            }
        }

        KeyframeGroup {
            target: pitScreen
            property: "stateOfChargePercentage"
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
