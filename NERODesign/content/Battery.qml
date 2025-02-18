import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts

Item {
   id: battery
   property int value: 0
   property int maxValue: 100
   property int minValue: 0
   property string color: "#00FF00"
   height: 500
   width: 200

   property real fillPercentage: (Math.max(0, value - minValue) / (maxValue - minValue))
   property bool increasing: value > previousValue
   property int previousValue: value

   onValueChanged: {
       increasing = value > previousValue;
       previousValue = value;
   }

   // Battery body
   Rectangle {
       id: outerRectangle
       anchors.centerIn: parent
       width: parent.width / 2
       height: parent.height * 0.8
       color: "transparent"
       border.width: width * 0.15  // Increased from 0.1
       border.color: battery.color
       radius: width * 0.15

       // Battery fill
       Rectangle {
           id: fillRect
           width: parent.width - parent.border.width * 2
           anchors.bottom: parent.bottom
           anchors.bottomMargin: parent.border.width
           anchors.horizontalCenter: parent.horizontalCenter
           height: (parent.height - parent.border.width * 2) * fillPercentage
           color: "transparent"
           clip: true

           Rectangle {
               id: gradientFill
               width: parent.width
               height: parent.height
               anchors.bottom: parent.bottom
               gradient: Gradient {
                   GradientStop { position: 0.0; color: battery.color }
                   GradientStop { position: 0.5; color: Qt.darker(battery.color, 2.5) }
                   GradientStop { position: 1.0; color: Qt.darker(battery.color, 3.5) }
               }

               Behavior on height {
                   NumberAnimation {
                       duration: 1000
                       easing.type: Easing.OutQuad
                       running: increasing
                   }
               }

               Behavior on height {
                   NumberAnimation {
                       duration: 1000
                       easing.type: Easing.InOutQuad
                       running: !increasing
                   }
               }
           }
       }
   }

   // Battery top with squared bottom corners
   Item {
       id: batteryTopContainer
       width: outerRectangle.width * 0.4
       height: outerRectangle.width * 0.2
       anchors.bottom: outerRectangle.top
       anchors.horizontalCenter: outerRectangle.horizontalCenter

       // Main nub with rounded top corners only
       Rectangle {
           id: batteryTop
           anchors.fill: parent
           color: battery.color
           radius: width * 0.15

           // Square bottom corners by overlaying rectangles
           Rectangle {
               width: parent.width
               height: parent.height / 2
               anchors.bottom: parent.bottom
               color: parent.color
           }
       }
   }
}
