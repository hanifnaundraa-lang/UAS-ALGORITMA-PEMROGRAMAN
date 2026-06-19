#pragma once
#include "gamedata.h"
#include "gameconfig.h"

// Material: Function declarations for Player module
namespace PlayerModule {

    // Initialize player with name and default values
    // Material: Struct & Reference
    void initPlayer(Player& player, const std::string& name);

    // Move player left (decrease x)
    void moveLeft(Player& player);

    // Move player right (increase x)
    void moveRight(Player& player);

    // Material: Default Argument
    void addScore(Player& player, int points = GameConfig::SCORE_PER_ENEMY);
    void addCoin(Player& player, int coins = GameConfig::COIN_PER_ENEMY);

    // Reduce health
    void takeDamage(Player& player, int damage = 1);

    // Check if player is alive
    bool isAlive(const Player& player);

} // namespace PlayerModule
