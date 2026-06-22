#include "player.h"
#include "gameutils.h"

/*==================================================
  MODUL PLAYER
==================================================*/
namespace PlayerModule {

    void initPlayer(Player& player, const std::string& name) {
        player.name = name;
        player.position.x = GameConfig::PLAYER_START_X;
        player.position.y = GameConfig::PLAYER_START_Y;
        player.health = GameConfig::PLAYER_START_HEALTH;
        player.score = 0;
        player.coin = 0;
        player.destroyedEnemy = 0;
        player.loadout = {0, false, false, false, false, false, 0, 0};
    }

    void moveLeft(Player& player) {
        /*==================================================
          MATERI: FUNCTION TEMPLATE
        ==================================================*/
        player.position.x = GameUtils::clampValue(player.position.x - 1, 0, GameConfig::ARENA_WIDTH - 1);
    }

    void moveRight(Player& player) {
        player.position.x = GameUtils::clampValue(player.position.x + 1, 0, GameConfig::ARENA_WIDTH - 1);
    }

    /*==================================================
      MATERI: DEFAULT ARGUMENT
    ==================================================*/
    void addScore(Player& player, int points) {
        if (player.loadout.scoreBoostActive) {
            player.score += (points * 2);
        } else {
            player.score += points;
        }
    }

    void addCoin(Player& player, int coins) {
        if (player.loadout.coinBoostActive) {
            player.coin += (coins * 2);
        } else {
            player.coin += coins;
        }
    }

    void takeDamage(Player& player, int damage) {
        if (player.loadout.shieldActive) {
            player.loadout.shieldActive = false; // Menahan serangan.
            return;
        }
        player.health -= damage;
        if (player.health < 0) player.health = 0;
    }

    bool isAlive(const Player& player) {
        return player.health > 0;
    }

} // namespace PlayerModule
