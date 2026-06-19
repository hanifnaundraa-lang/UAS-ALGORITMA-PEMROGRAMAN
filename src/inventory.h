#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

// Placeholder for Inventory module — will be implemented by Member 2
namespace InventoryModule {

    void addItem(std::vector<Item>& inventory, const Item& item);
    void removeItem(std::vector<Item>& inventory, int itemId);
    void displayInventory(const std::vector<Item>& inventory);
    void saveInventory(const std::string& playerName, const std::vector<Item>& inventory);
    std::vector<Item> loadInventory(const std::string& playerName);

} // namespace InventoryModule
