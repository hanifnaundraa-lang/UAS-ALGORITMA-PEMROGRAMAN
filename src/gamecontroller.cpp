#include "gamecontroller.h"
#include <QDebug>

GameController::GameController(QObject *parent) 
    : QObject(parent), m_isRunning(false) {
    m_player = new Player(this);
    m_scoreManager = new ScoreManager(this);
    m_gameTimer = new QTimer(this);
    
    connect(m_gameTimer, &QTimer::timeout, this, &GameController::gameLoop);
    
    m_gameStats = {0, 0, 3};
}

void GameController::startGame() {
    if (!m_isRunning) {
        m_isRunning = true;
        m_gameTimer->start(16); // ~60 FPS
        emit isRunningChanged();
        qDebug() << "Game Started";
    }
}

void GameController::pauseGame() {
    if (m_isRunning) {
        m_isRunning = false;
        m_gameTimer->stop();
        emit isRunningChanged();
        qDebug() << "Game Paused";
    }
}

void GameController::quitGame() {
    pauseGame();
    // Add logic to save score before quitting
    qDebug() << "Game Quit";
}

void GameController::gameLoop() {
    // Basic game loop logic will be added here
    // Material: Iterator (for enemy/bullet lists)
}
