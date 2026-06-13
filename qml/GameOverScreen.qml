import QtQuick
import QtQuick.Controls

Item {
    anchors.fill: parent
    Rectangle {
        anchors.fill: parent
        color: "#101020"
        Text {
            text: "GAME OVER"
            anchors.centerIn: parent
            color: "red"
            font.pixelSize: 48
        }
    }
}
