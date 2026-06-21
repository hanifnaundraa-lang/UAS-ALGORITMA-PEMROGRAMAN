#pragma once
#include <string>

// Material: Struct, Pointer & Reference
// Shared data structures for Astro Strike

struct Position {
    int x;
    int y;
};

struct Loadout {
    int healthPotionUsed;
    bool shieldActive;
    bool doubleBulletActive;
    bool scoreBoostActive;
    bool coinBoostActive;
    bool empEquipped;
    int empCharges;
    int empTimer;
};

struct Player {
    std::string name;
    Position position;
    int health;
    int score;
    int coin;
    int destroyedEnemy;
    Loadout loadout;
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
    int stock;              // jumlah stok di shop, -1 = unlimited
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

// Gacha reward pool entry (loaded from gacha_pool.csv)
struct GachaReward {
    int id;
    std::string name;
    std::string type;
    std::string rarity;
    int weight;
};

// Gacha history record (saved to gacha_history.csv)
struct GachaRecord {
    std::string date;
    std::string playerName;
    std::string reward;
    std::string rarity;
};
