// #pragma once
// #include "gamedata.h"
// #include <vector>
// #include <string>
// using namespace std;

// namespace InventoryModule {

    // void addItem(std::vector<Item>& inventory, const Item& item);
    // void removeItem(std::vector<Item>& inventory, int itemId);
    // void displayInventory(const std::vector<Item>& inventory);
    // void saveInventory(const std::string& playerName, const std::vector<Item>& inventory);
    // std::vector<Item> loadInventory(const std::string& playerName);
// } // namespace InventoryModule




#pragma once
#include "gamedata.h"
#include <vector>
#include <string>
using namespace std;

namespace InventoryModule {
    
    void addItem(vector<Item>& inventory, const Item& item);    
    bool removeItem(vector<Item>& inventory, int itemId); 
    Item* findItem(vector<Item>& inventory, int itemId);
    int countItem(const vector<Item>& inventory, int itemId);
    void displayInventory(const vector<Item>& inventory);
    void saveInventory(const string& playerName, const vector<Item>& inventory);
    vector<Item> loadInventory(const string& playerName);

} // namespace InventoryModule