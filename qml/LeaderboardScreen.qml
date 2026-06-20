import QtQuick
import QtQuick.Controls

Item {
    id: root
    signal back()

    Rectangle {
        anchors.fill: parent
        color: "#101020"

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "LEADERBOARD"
                color: "white"
                font.pixelSize: 32
            }

            // ListView would go here for scores

            Button {
                text: "Back"
                onClicked: root.back()
            }
        }
    }
}
