// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import NERO

Window {
    width: 800
    height: 480
    color: "black"

    visible: true
    title: "NERO"

    Spedometer {
        anchors.centerIn: parent
        value: 100
        width: 400
        verticalPadding: 50
        mainTextTopPadding: 10
    }

}

