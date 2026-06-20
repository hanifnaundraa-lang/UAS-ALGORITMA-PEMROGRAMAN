#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

// Materi: Namespace, Function, STL Vector, Sort, Lambda, File Handling, Exception Handling
// Shop module — upgraded with wallet integration, stock, sorting, quantity purchase, and history
namespace ShopModule {

    // --- Catalog Management (Feature 4: Cache) ---
    // Load dan cache shop catalog dari data/shop_items.csv
    // Format: id,name,type,price,effect,stock
    std::vector<Item> loadShopCatalog();

    // Save updated stock back to shop_items.csv (Feature 8)
    void saveShopCatalog(const std::vector<Item>& catalog);

    // --- Display ---
    // Tampilkan katalog shop, menunjukkan stok (Feature 8)
    void displayShop(const std::vector<Item>& catalog, int walletCoin);

    // --- Purchase (Feature 1, 5, 8) ---
    // Feature 1: Uses Wallet instead of Player.coin
    // Feature 5: Supports quantity purchase
    // Feature 8: Validates and reduces stock
    bool buyItem(Wallet& wallet, std::vector<Item>& catalog,
                 std::vector<Item>& inventory, int itemId, int quantity = 1);

    // --- Sorting (Feature 10) ---
    // Material: Sort, Lambda
    void sortByPriceAsc(std::vector<Item>& catalog);   // harga terendah
    void sortByPriceDesc(std::vector<Item>& catalog);   // harga tertinggi
    void sortByNameAsc(std::vector<Item>& catalog);     // A-Z
    void sortByNameDesc(std::vector<Item>& catalog);     // Z-A
    void sortByIdAsc(std::vector<Item>& catalog);       // ID 1,2,3...

    // --- History (Feature 9) ---
    void saveShopHistory(const std::string& playerName, const std::string& itemName,
                         int quantity, int totalCost);

    // --- Menu Entry Point ---
    // Feature 1: Wallet& instead of Player&
    void runShop(Wallet& wallet, std::vector<Item>& inventory);

} // namespace ShopModule
