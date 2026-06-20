#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

// Placeholder for Inventory module — will be implemented by Member 2
namespace InventoryModule {

    void addItem(std::vector<Item>& inventory, const Item& item);
    bool removeItem(std::vector<Item>& inventory, int itemId); // ganti jadi function boolean
    
    // mencari item dalam inventory berdasarkan id (menggunakan sort)
    Item* findItem(const std::vector<Item>& inventory, int itemId);
    
    // mmenghitung jumlah item tertentu dalam inventory (menggunakan count)
    int countItem(const std::vector<Item>& inventory, int itemId);
    
    void displayInventory(const std::vector<Item>& inventory);
    void saveInventory(const std::string& playerName, const std::vector<Item>& inventory);
    std::vector<Item> loadInventory(const std::string& playerName);

} // namespace InventoryModule
