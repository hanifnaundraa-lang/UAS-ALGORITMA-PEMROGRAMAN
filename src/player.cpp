#include "player.h"
#include "gameutils.h"

// Material: Function, Struct, Pointer & Reference
namespace PlayerModule {

    void initPlayer(Player& player, const std::string& name) {
        player.name = name;
        player.position.x = GameConfig::PLAYER_START_X;
        player.position.y = GameConfig::PLAYER_START_Y;
        player.health = GameConfig::PLAYER_START_HEALTH;
        player.score = 0;
        player.coin = 0;
        player.destroyedEnemy = 0;
    }

    void moveLeft(Player& player) {
        // Material: Function Template — clampValue
        player.position.x = GameUtils::clampValue(player.position.x - 1, 0, GameConfig::ARENA_WIDTH - 1);
    }

    void moveRight(Player& player) {
        player.position.x = GameUtils::clampValue(player.position.x + 1, 0, GameConfig::ARENA_WIDTH - 1);
    }

    // Material: Default Argument
    void addScore(Player& player, int points) {
        player.score += points;
    }

    void addCoin(Player& player, int coins) {
        player.coin += coins;
    }

    void takeDamage(Player& player, int damage) {
        player.health -= damage;
        if (player.health < 0) player.health = 0;
    }

    bool isAlive(const Player& player) {
        return player.health > 0;
    }

} // namespace PlayerModule
