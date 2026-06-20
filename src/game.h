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
#include <vector>
#include <list>
#include <string>

// Material: STL Vector & List
// The Game class orchestrates the entire terminal game loop.

class Game {
public:
    Game();

    // Material: Function — main entry points
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

    // --- Game loop functions ---
    void handleInput();
    void updateBullets();
    void updateEnemies();
    void spawnEnemy();
    void checkCollisions();
    void render();

    // --- State ---
    Player player;
    Wallet wallet;                  // Material: Struct — wallet for trading
    std::vector<Item> inventory;    // Material: STL Vector - item yang dimiliki pemain
    std::vector<Bullet> bullets;    // Material: STL Vector
    std::list<Enemy> enemies;       // Material: STL List
    bool isRunning;
    bool isPaused;
    int frameCounter;
};
