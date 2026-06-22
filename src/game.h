/*==================================================
  MODUL : game.h
  FUNGSI:
  - Deklarasi class Game
==================================================*/
#pragma once
#include "gamedata.h"
#include "gameconfig.h"
#include "gameutils.h"
#include "gameexception.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "inventory.h"
#include "shop.h"
#include "trading.h"
#include "scoremanager.h"
#include "gacha.h"
#include <vector>
#include <list>
#include <string>

/*==================================================
  MATERI: STL Vector & List
  Class Game mengatur seluruh game loop terminal.
==================================================*/
class Game {
public:
    Game();

    /*==================================================
      MATERI: Function
      Titik masuk utama eksekusi loop game.
    ==================================================*/
    void run();

private:
    // --- Menu and flow ---
    void showMainMenu();
    void inputPlayerName();
    void showLoadoutAndStart();
    void startGame();
    void showGameOver();
    void showControls();
    void showInventory();
    void showShop();
    void showTrading();
    void showLeaderboard();
    void showGacha();

    // --- Game loop functions ---
    void handleInput();
    void updateBullets();
    void updateEnemies();
    void spawnEnemy();
    void checkCollisions();
    void render();

    // --- State ---
    Player player;
    /*==================================================
      MATERI: Struct
      Menyimpan data wallet trading pemain.
    ==================================================*/
    Wallet wallet;

    /*==================================================
      MATERI: STL Vector
      Menyimpan daftar item yang dimiliki pemain.
    ==================================================*/
    std::vector<Item> inventory;

    /*==================================================
      MATERI: STL Vector
      Menyimpan daftar peluru aktif.
    ==================================================*/
    std::vector<Bullet> bullets;

    /*==================================================
      MATERI: STL List
      Menyimpan daftar musuh.
    ==================================================*/
    std::list<Enemy> enemies;
    bool isRunning;
    bool isPaused;
    int frameCounter;
};
