#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QString>
#include <QDateTime>

// Material: Struct
struct Vector2D {
    float x;
    float y;
};

struct PlayerScore {
    QString name;
    int score;
    int destroyedEnemy;
    QDateTime dateTime;
};

struct GameStats {
    int currentScore;
    int enemiesDestroyed;
    int livesRemaining;
};

#endif // GAMEDATA_H
