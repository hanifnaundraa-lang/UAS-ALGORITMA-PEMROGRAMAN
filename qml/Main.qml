import QtQuick
import QtQuick.Controls
import AstroStrike

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Astro Strike")

    StackView {
        id: mainStack
        anchors.fill: parent
        initialItem: mainMenu
    }

    Component {
        id: mainMenu
        MainMenu {
            onStartGame: {
                mainStack.push(gameScreen)
                gameController.startGame()
            }
            onShowLeaderboard: mainStack.push(leaderboardScreen)
        }
    }

    Component {
        id: gameScreen
        GameScreen {
            onQuit: {
                gameController.quitGame()
                mainStack.pop()
            }
        }
    }

    Component {
        id: leaderboardScreen
        LeaderboardScreen {
            onBack: mainStack.pop()
        }
    }
}
