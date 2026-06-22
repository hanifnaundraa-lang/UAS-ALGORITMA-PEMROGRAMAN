#pragma once
#include "gamedata.h"
#include "gameconfig.h"

/*==================================================
  MODUL PLAYER
==================================================*/
namespace PlayerModule {

    // Menginisialisasi pemain dengan nama dan nilai default.
    /*==================================================
      MATERI: STRUCT & REFERENCE
    ==================================================*/
    void initPlayer(Player& player, const std::string& name);

    // Menggerakkan pemain ke kiri (mengurangi nilai x).
    void moveLeft(Player& player);

    // Menggerakkan pemain ke kanan (menambah nilai x).
    void moveRight(Player& player);

    /*==================================================
      MATERI: DEFAULT ARGUMENT
    ==================================================*/
    void addScore(Player& player, int points = GameConfig::SCORE_PER_ENEMY);
    void addCoin(Player& player, int coins = GameConfig::COIN_PER_ENEMY);

    // Mengurangi health pemain.
    void takeDamage(Player& player, int damage = 1);

    // Memeriksa apakah pemain masih hidup.
    bool isAlive(const Player& player);

} // namespace PlayerModule
