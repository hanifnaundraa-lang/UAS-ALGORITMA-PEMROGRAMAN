#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

// Materi: Namespace, Function, STL Vector, Find, Count, Sort, Lambda, Iterator
// Inventory module — manages player items with search, filter, and CSV persistence
namespace InventoryModule {

    // --- Core CRUD ---
    void addItem(std::vector<Item>& inventory, const Item& item);
    bool removeItem(std::vector<Item>& inventory, int itemId);

    // --- Search & Count ---
    // Material: Find, Lambda — mencari item berdasarkan id
    Item* findItem(std::vector<Item>& inventory, int itemId);

    // Material: Count, Lambda — menghitung jumlah item tertentu
    int countItem(const std::vector<Item>& inventory, int itemId);

    // Material: Find, Lambda — mencari item berdasarkan nama (Feature 6)
    std::vector<Item> searchByName(const std::vector<Item>& inventory, const std::string& keyword);

    // Material: Iterator, Lambda — filter item berdasarkan type (Feature 7)
    std::vector<Item> filterByType(const std::vector<Item>& inventory, const std::string& type);

    // --- Display ---
    void displayInventory(const std::vector<Item>& inventory);

    // --- Interactive Inventory Menu ---
    void runInventory(std::vector<Item>& inventory, const std::string& playerName);

    // --- Persistence ---
    // Format: playerName,itemId,itemName,type,price,effect,quantity
    void saveInventory(const std::string& playerName, const std::vector<Item>& inventory);
    std::vector<Item> loadInventory(const std::string& playerName);

} // namespace InventoryModule
