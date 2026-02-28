pragma Singleton
import QtQuick

QtObject {
            property string currentTheme: "dark"
            property var themes: {
                        "dark": {
                                    background: "#000000",
                                    primaryForeground: "#FFFFFF",
                                    accentForeground: "#47A7FF",
                                    inverseForeground: "#000000",
                                    blackForeground: "#000000",
                                    mutedForeground: "#777777",
                                    offCarForeground: "#ff0101",
                                    onGLVMSForeground: "#1cff00",

                                    transparent: "transparent",

                                    currentRunBackground: "#47A7FF",
                                    lastRunBackground: "#FFFFFF",
                                    fastestRunBackground: "#AD00FF",

                                    directionBackground: "#55AAFF",

                                    iconHighlightedBackground: "#6E6E6E",
                                    iconBackground: "#191919",
                                    iconHighlightedGradientStop: "#333333",
                                    iconGradientStop:"#111111",

                                    goodStatus: "#55FF00",
                                    cautionStatus: "orange",
                                    criticalStatus: "red",

                                    redThermoStatus: "red",
                                    orangeThermoStatus: "orange",
                                    yellowThermoStatus: "#FFF500",
                                    blueThermoStatus: "blue",
                                    purpleThermoStatus: "purple",

                                    fillGradientStop: "#000000",

                                    backgroundPicture: "#CBCACA",
                                    popoverBackground: "#FFFFFF",
                                    descriptionButtonBackground: "#000000",
                                    descriptionButtonForeground: "#FFFFFF",

                                    accentBlue: "blue",
                                    accentGreen: "#14F504",
                                    accentPurple: "purple",

                                    primaryMicrophone: "#00AA00",
                                    secondaryMicrophone: "#00FF000",

                                    primaryDetailGradient: "black",
                                    secondaryDetailGradient: "#969696",

                                    nonCriticalBackground: "#ffca62",
                                    nonCriticalForeground: "white"
                        },
                        "light": {
                                    background: "#FFFFFF",
                                    primaryForeground: "#000000",
                                    accentForeground: "#47A7FF",
                                    inverseForeground: "#FFFFFF",
                                    blackForeground: "#000000",
                                    mutedForeground: "#777777",
                                    offCarForeground: "#ff0101",
                                    onGLVMSForeground: "#1cff00",

                                    transparent: "transparent",

                                    currentRunBackground: "#47A7FF",
                                    lastRunBackground: "#000000",
                                    fastestRunBackground: "#AD00FF",

                                    directionBackground: "#55AAFF",

                                    iconHighlightedBackground: "#DCDCDC",
                                    iconBackground: "#1919190A",
                                    iconHighlightedGradientStop: "#C4C4C4",
                                    iconGradientStop:"#1111110A",

                                    goodStatus: "#55FF00",
                                    cautionStatus: "orange",
                                    criticalStatus: "red",

                                    redThermoStatus: "red",
                                    orangeThermoStatus: "orange",
                                    yellowThermoStatus: "#FFF500",
                                    blueThermoStatus: "blue",
                                    purpleThermoStatus: "purple",

                                    fillGradientStop: "#FFFFFF",

                                    backgroundPicture: "#CBCACA",
                                    popoverBackground: "#FFFFFF",
                                    descriptionButtonBackground: "#000000",
                                    descriptionButtonForeground: "#FFFFFF",

                                    accentBlue: "blue",
                                    accentGreen: "#14F504",
                                    accentPurple: "purple",

                                    primaryMicrophone: "#00AA00",
                                    secondaryMicrophone: "#00FF000",

                                    primaryDetailGradient: "black",
                                    secondaryDetailGradient: "#969696",

                                    nonCriticalBackground: "#ffca62",
                                    nonCriticalForeground: "white"
                        }
            }
            function getColor(prop) {
                        return themes[currentTheme][prop];
            }

            function setTheme(theme) {
                        currentTheme = theme
            }
}
