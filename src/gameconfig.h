#pragma once
#include <string>

/*==================================================
  MATERI: NAMESPACE
==================================================*/
namespace GameConfig {
    /*==================================================
      FUNGSI: Dimensi arena (area yang dapat dimainkan, tidak termasuk batas)
    ==================================================*/
    constexpr int ARENA_WIDTH = 40;
    constexpr int ARENA_HEIGHT = 20;

    /*==================================================
      FUNGSI: Pengaturan pemain
    ==================================================*/
    constexpr int PLAYER_START_HEALTH = 3;
    constexpr int PLAYER_START_X = ARENA_WIDTH / 2;
    constexpr int PLAYER_START_Y = ARENA_HEIGHT - 1;

    /*==================================================
      FUNGSI: Pengaturan waktu permainan
    ==================================================*/
    constexpr int GAME_LOOP_DELAY_MS = 80;
    constexpr int ENEMY_SPAWN_INTERVAL = 15; // [LEVEL 3] Jeda frame antar spawn
    constexpr int ENEMY_MOVE_INTERVAL = 3;    // [LEVEL 3] Musuh bergerak setiap N frame (lebih tinggi = lebih lambat)

    /*==================================================
      FUNGSI: Pengaturan skor
    ==================================================*/
    constexpr int SCORE_PER_ENEMY = 10;
    constexpr int COIN_PER_ENEMY = 15;

    /*==================================================
      FUNGSI: Simbol karakter
    ==================================================*/
    constexpr char PLAYER_SYMBOL = 'A';
    constexpr char BULLET_SYMBOL = '|';
    constexpr char ENEMY_SYMBOL = 'V';
    constexpr char EMPTY_SYMBOL = ' ';
    constexpr char BORDER_SYMBOL = '#';
}

/*==================================================
  MATERI: NAMESPACE
==================================================*/
/*==================================================
  FUNGSI: Jalur file data
==================================================*/
namespace FileConfig {
    const std::string DATA_DIR = "data/";
    const std::string LEADERBOARD_FILE = DATA_DIR + "leaderboard.csv";
    const std::string INVENTORY_FILE = DATA_DIR + "inventory.csv";
    const std::string WALLET_FILE = DATA_DIR + "wallet.csv";
    const std::string SHOP_ITEMS_FILE = DATA_DIR + "shop_items.csv";
    const std::string GACHA_POOL_FILE = DATA_DIR + "gacha_pool.csv";
    const std::string MARKET_HISTORY_FILE = DATA_DIR + "market_history.csv";
    const std::string TRADE_HISTORY_FILE = DATA_DIR + "trade_history.csv";
    const std::string SHOP_HISTORY_FILE = DATA_DIR + "shop_history.csv";
    const std::string GACHA_HISTORY_FILE = DATA_DIR + "gacha_history.csv";
}
