pragma Singleton
import QtQuick

QtObject {
    id: root

    property string currentTheme: "dark"
    readonly property bool isDark: currentTheme === "dark"

    // Core foregrounds
    readonly property color background:           isDark ? "#000000" : "#FFFFFF"
    readonly property color primaryForeground:     isDark ? "#FFFFFF" : "#000000"
    readonly property color accentForeground:      "#47A7FF"
    readonly property color inverseForeground:     isDark ? "#000000" : "#FFFFFF"
    readonly property color blackForeground:       "#000000"
    readonly property color mutedForeground:       "#777777"
    readonly property color offCarForeground:      "#ff0101"
    readonly property color onGLVMSForeground:     "#1cff00"

    // Run timer backgrounds
    readonly property color currentRunBackground:  "#47A7FF"
    readonly property color lastRunBackground:     isDark ? "#FFFFFF" : "#000000"
    readonly property color fastestRunBackground:  "#AD00FF"

    // Direction
    readonly property color directionBackground:   "#55AAFF"

    // Icon backgrounds
    readonly property color iconHighlightedBackground:   isDark ? "#6E6E6E" : "#DCDCDC"
    readonly property color iconBackground:              isDark ? "#191919" : "#1919190A"
    readonly property color iconHighlightedGradientStop: isDark ? "#333333" : "#C4C4C4"
    readonly property color iconGradientStop:            isDark ? "#111111" : "#1111110A"

    // Status colors
    readonly property color goodStatus:     "#55FF00"
    readonly property color cautionStatus:  "orange"
    readonly property color criticalStatus: "red"

    // Thermometer status
    readonly property color redThermoStatus:    "red"
    readonly property color orangeThermoStatus: "orange"
    readonly property color yellowThermoStatus: "#FFF500"
    readonly property color blueThermoStatus:   "blue"
    readonly property color purpleThermoStatus: "purple"

    // Gradients / fills
    readonly property color fillGradientStop: isDark ? "#000000" : "#FFFFFF"

    // Popover / modal
    readonly property color backgroundPicture:           "#CBCACA"
    readonly property color popoverBackground:           "#FFFFFF"
    readonly property color descriptionButtonBackground: "#000000"
    readonly property color descriptionButtonForeground: "#FFFFFF"

    // Accent colors
    readonly property color accentBlue:   "blue"
    readonly property color accentGreen:  "#14F504"
    readonly property color accentPurple: "purple"

    // Microphone
    readonly property color primaryMicrophone:   "#00AA00"
    readonly property color secondaryMicrophone: "#00FF00"

    // Detail display gradient
    readonly property color primaryDetailGradient:   "black"
    readonly property color secondaryDetailGradient: "#969696"

    // Non-critical warning
    readonly property color nonCriticalBackground: "#ffca62"
    readonly property color nonCriticalForeground: "white"

    // Theme switching
    function setTheme(theme: string) {
        currentTheme = theme
    }
}
