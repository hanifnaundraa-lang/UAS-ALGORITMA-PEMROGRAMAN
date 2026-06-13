import QtQuick
import QtQuick.Controls

Item {
    id: root
    signal quit()

    Rectangle {
        anchors.fill: parent
        color: "black"

        // Space Background Placeholder
        Image {
            source: "qrc:/AstroStrike/assets/background.png"
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
        }

        // Player Placeholder
        Rectangle {
            id: playerItem
            width: 50
            height: 50
            color: "blue"
            x: gameController.player.x - width/2
            y: gameController.player.y - height/2

            Image {
                source: "qrc:/AstroStrike/assets/player.png"
                anchors.fill: parent
                visible: source !== ""
            }
        }

        Text {
            text: "Score: " + gameController.score
            color: "white"
            font.pixelSize: 24
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 20
        }

        Button {
            text: "Quit"
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 20
            onClicked: root.quit()
        }
    }
}
