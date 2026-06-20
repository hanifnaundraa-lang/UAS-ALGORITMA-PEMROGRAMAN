#pragma once
#include "gamedata.h"
#include <vector>

// Placeholder for Gacha module – will be implemented by Member 5
namespace GachaModule {

    // Helper tambahan agar modul tahu siapa player yang sedang membuka gacha
    void setPlayerName(const std::string& name);

    void displayGachaPool();
    Item rollGacha(int cost);
    bool rollWithTicket();

} // namespace GachaModule
