#pragma once
#include <string>

/*==================================================
  MATERI: STRUCT, POINTER & REFERENCE
==================================================*/
/*==================================================
  FUNGSI: Struktur data bersama untuk Astro Strike
==================================================*/

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

/*==================================================
  FUNGSI: Struktur data untuk modul inventory, shop, trading, gacha, dan leaderboard
==================================================*/
struct Item {
    int id;
    std::string name;
    std::string type;
    int price;
    std::string effect;     // [LEVEL 3] Efek item, misalnya "heal", "damage boost", dll
    int quantity;
    int stock;              // [LEVEL 3] Jumlah stok di shop, -1 = tidak terbatas
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

/*==================================================
  FUNGSI: Entri pool hadiah gacha (dimuat dari gacha_pool.csv)
==================================================*/
struct GachaReward {
    int id;
    std::string name;
    std::string type;
    std::string rarity;
    int weight;
};

/*==================================================
  FUNGSI: Catatan riwayat gacha (disimpan ke gacha_history.csv)
==================================================*/
struct GachaRecord {
    std::string date;
    std::string playerName;
    std::string reward;
    std::string rarity;
};
