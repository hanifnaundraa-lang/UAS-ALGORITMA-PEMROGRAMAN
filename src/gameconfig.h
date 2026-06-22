#pragma once
#include <string>

// Material: Namespace
namespace GameConfig {
    // Arena dimensions (inner playable area, excluding borders)
    constexpr int ARENA_WIDTH = 40;
    constexpr int ARENA_HEIGHT = 20;

    // Player settings
    constexpr int PLAYER_START_HEALTH = 3;
    constexpr int PLAYER_START_X = ARENA_WIDTH / 2;
    constexpr int PLAYER_START_Y = ARENA_HEIGHT - 1;

    // Game timing
    constexpr int GAME_LOOP_DELAY_MS = 80;
    constexpr int ENEMY_SPAWN_INTERVAL = 15; // frames between spawns
    constexpr int ENEMY_MOVE_INTERVAL = 3;    // enemies move every N frames (higher = slower)

    // Scoring
    constexpr int SCORE_PER_ENEMY = 10;
    constexpr int COIN_PER_ENEMY = 15;

    // Symbols
    constexpr char PLAYER_SYMBOL = 'A';
    constexpr char BULLET_SYMBOL = '|';
    constexpr char ENEMY_SYMBOL = 'V';
    constexpr char ARMORED_SYMBOL = 'W';
    constexpr char EMPTY_SYMBOL = ' ';
    constexpr char BORDER_SYMBOL = '#';
}

// Material: Namespace — File paths
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

// Material: Namespace — Color definitions
namespace GameColor {
    const std::string RESET = "\033[0m";
    const std::string PLAYER = "\033[96m";    // Bright Cyan
    const std::string BULLET = "\033[93m";    // Bright Yellow
    const std::string ENEMY = "\033[91m";     // Bright Red
    const std::string ARMORED = "\033[95m";   // Bright Magenta
    const std::string WALL = "\033[90m";      // Bright Black / Gray
    const std::string TXT_SUCCESS = "\033[92m"; // Bright Green
    const std::string TXT_WARNING = "\033[93m"; // Bright Yellow
    const std::string TXT_ERROR = "\033[91m";   // Bright Red
    
    const std::string MENU_TITLE = "\033[96m"; // Bright Cyan
    const std::string HIGHLIGHT = "\033[97m";  // White
}
