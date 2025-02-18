import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
   id: thermometerComponent
   property int value // Main value property that will be passed in
   property string title: "MOTOR TEMP"
   property bool regen: false
   property string textColor: "#4DA6FF"
   property string componentColor: "#FF0000"
   height: 120
   width: 120
   color: "transparent"

   Rectangle {
       id: background
       anchors.fill: parent
       color: "#151515"
       radius: 20
   }

   LabelText {
       id: titleText
       anchors {
           top: parent.top
           horizontalCenter: parent.horizontalCenter
           margins: 10
       }
       text: title
       color: textColor
       font.bold: true
       font.pixelSize: 24
   }

   Rectangle {
       id: contentRow
       anchors {
           fill: parent
           margins: 10
       }
       color: "transparent"

       Thermometer {
           id: thermometer
           value: thermometerComponent.value  // Passing through the value
           height: parent.height * .65
           width: height * 1
           anchors {
               horizontalCenter: parent.horizontalCenter
               horizontalCenterOffset: -parent.width * 0.3
               verticalCenter: parent.verticalCenter
               verticalCenterOffset: parent.height * 0.1
           }
           regen: thermometerComponent.regen
           color: componentColor
       }

       ValueText {
           id: valueNumber
           text: thermometerComponent.value  // Using the same value
           font.pixelSize: 62
           color: white
           font.bold: true
           anchors {
               horizontalCenter: parent.horizontalCenter
               horizontalCenterOffset: parent.width * 0.15
               verticalCenter: parent.verticalCenter
               verticalCenterOffset: parent.height * 0.1
           }
       }

       Text {
           text: "°"
           color: '#777777'
           font.pixelSize: 48
           font.bold: true
           anchors {
               left: valueNumber.right
               top: valueNumber.top
               topMargin: -12
           }
       }
   }
}
