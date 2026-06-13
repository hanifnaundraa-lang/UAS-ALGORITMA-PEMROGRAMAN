#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QQmlListProperty>
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "scoremanager.h"

class GameController : public QObject {
    Q_OBJECT
    Q_PROPERTY(Player* player READ player CONSTANT)
    Q_PROPERTY(int score READ score NOTIFY scoreChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)

public:
    explicit GameController(QObject *parent = nullptr);

    Player* player() const { return m_player; }
    int score() const { return m_gameStats.currentScore; }
    bool isRunning() const { return m_isRunning; }

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void pauseGame();
    Q_INVOKABLE void quitGame();

signals:
    void scoreChanged();
    void isRunningChanged();
    void gameOver();

private slots:
    void gameLoop();

private:
    Player *m_player;
    ScoreManager *m_scoreManager;
    GameStats m_gameStats;
    bool m_isRunning;
    QTimer *m_gameTimer;
};

#endif // GAMECONTROLLER_H
