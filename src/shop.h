#pragma once
#include "gamedata.h"
#include <vector>

// Placeholder for Shop module — will be implemented by Member 3
namespace ShopModule {

    void displayShop();
    bool buyItem(Player& player, int itemId);
    std::vector<Item> getShopItems();

} // namespace ShopModule
