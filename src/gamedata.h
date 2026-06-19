#pragma once
#include <string>

// Material: Struct, Pointer & Reference
// Shared data structures for Astro Strike

struct Position {
    int x;
    int y;
};

struct Player {
    std::string name;
    Position position;
    int health;
    int score;
    int coin;
    int destroyedEnemy;
};

struct Bullet {
    Position position;
    bool active;
};

struct Enemy {
    Position position;
    int health;
    bool active;
};

// Structs for future modules (inventory, shop, trading, gacha, leaderboard)
struct Item {
    int id;
    std::string name;
    std::string type;
    int price;
    std::string effect;     // untuk efek item, misal "heal", "damage boost", dll
    int quantity;
};

struct PlayerScore {
    std::string name;
    int score;
    int destroyedEnemy;
    std::string dateTime;
};

struct Wallet {
    std::string playerName;
    int coin;
    double btc;
    double eth;
};

struct MarketRate {
    int day;
    int btcPrice;
    int ethPrice;
};

struct TradeRecord {
    int day;
    std::string playerName;
    std::string type;
    std::string asset;
    double amount;
    int price;
    int total;
};
