#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

// Materi: Function, STL Vector, File Handling, Exception Handling
// Shop module, memungkinkan pemain membeli item dengan koin.
namespace ShopModule {

    // Load katalog dari data/shop_items.csv (id,name,type,price,effect)
    std::vector<Item> getShopItems();

    // Print shop catalog ke terminal
    void displayShop();

    // Beli itemId dari katalog toko untuk `player`.
    // Memvalidasi koin, mengurangi koin, dan menambahkan item ke `inventory`.
    // Melempar InsufficientFundsException jika pemain tidak mampu membayar,
    // dan GameException jika itemId tidak ada di katalog.
    bool buyItem(Player& player, std::vector<Item>& inventory, int itemId);

    // Interactive shop screen: menampilkan katalog, membaca pilihan pemain, membeli
    // melaporkan hasil, dan loop sampai pemain memilih exit.
    // Ini adalah entry point yang dipangil dari main menu.
    void runShop(Player& player, std::vector<Item>& inventory);

} // namespace ShopModule
