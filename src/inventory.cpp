#include "inventory.h"
#include <iostream>

// Placeholder — full implementation by Member 2
namespace InventoryModule {

    void addItem(std::vector<Item>& inventory, const Item& item) {
        (void)inventory; (void)item;
    }

    void removeItem(std::vector<Item>& inventory, int itemId) {
        (void)inventory; (void)itemId;
    }

    void displayInventory(const std::vector<Item>& inventory) {
        (void)inventory;
        std::cout << "[Inventory] Coming soon..." << std::endl;
    }

    void saveInventory(const std::string& playerName, const std::vector<Item>& inventory) {
        (void)playerName; (void)inventory;
    }

    std::vector<Item> loadInventory(const std::string& playerName) {
        (void)playerName;
        return {};
    }

} // namespace InventoryModule
