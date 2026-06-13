import QtQuick
import QtQuick.Controls

Item {
    id: root
    signal startGame()
    signal showLeaderboard()

    Rectangle {
        anchors.fill: parent
        color: "#050510"

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "ASTRO STRIKE"
                font.pixelSize: 48
                color: "white"
                font.bold: true
            }

            Button {
                text: "Start Game"
                width: 200
                onClicked: root.startGame()
            }

            Button {
                text: "Leaderboard"
                width: 200
                onClicked: root.showLeaderboard()
            }

            Button {
                text: "Exit"
                width: 200
                onClicked: Qt.quit()
            }
        }
    }
}
