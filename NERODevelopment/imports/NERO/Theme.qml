pragma Singleton
import QtQuick 6.2

QtObject {
    property string currentTheme: "dark"
    property var themes: {
        "dark": {
            background: "#000000",
            foreground: "#FFFFFF",
            inverseForeground: "#000000",
            border: "#FFFFFF",
            transparent: "transparent",
            
            currentRunBackground: "#47A7FF",
            lastRunBackground: "#FFFFFF",
            fastestRunBackground: "#AD00FF",
            
            fillBackground: "#000000",
            fillGradientStop: "#000000"
        },
        "light": {
            background: "#FFFFFF",
        }
    }

    function getColor(prop) {
        return themes[currentTheme][prop];
    }

    function setTheme(theme) {
        currentTheme = theme
    }
}
