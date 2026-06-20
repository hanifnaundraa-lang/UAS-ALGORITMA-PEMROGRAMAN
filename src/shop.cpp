#include "shop.h"
#include "inventory.h"
#include "gameconfig.h"
#include "gameexception.h"
#include "gameutils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <conio.h>

// Materi: Function, STL Vector, Iterator, File Handling, Exception Handling, Lambda
namespace ShopModule {

    // ============================================================
    // Load shop catalog dari data/shop_items.csv
    // Format: id,name,type,price,effect
    // ============================================================
    std::vector<Item> getShopItems() {
        std::vector<Item> items;
        std::ifstream inFile(FileConfig::SHOP_ITEMS_FILE);

        // Materi: Exception Handling, file katalog hilang adalah error fatal karena toko tidak bisa beroperasi
        if (!inFile.is_open()) {
            throw FileException("Unable to open " + FileConfig::SHOP_ITEMS_FILE + ".");
        }

        std::string line;
        bool isHeader = true;
        while (std::getline(inFile, line)) {
            if (isHeader) { isHeader = false; continue; } // skip header
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string idStr, name, type, priceStr, effect;

            std::getline(ss, idStr, ',');
            std::getline(ss, name, ',');
            std::getline(ss, type, ',');
            std::getline(ss, priceStr, ',');
            std::getline(ss, effect, ',');

            Item item;
            item.id = std::stoi(idStr);
            item.name = name;
            item.type = type;
            item.price = std::stoi(priceStr);
            item.effect = effect;
            item.quantity = 1; // entri katalog mewakili satu unit yang dapat dibeli

            items.push_back(item);
        }

        inFile.close();
        return items;
    }

    // ============================================================
    // Tampilkan katalog shop sebagai tabel terformat di terminal
    // ============================================================
    void displayShop() {
        std::vector<Item> items = getShopItems();

        std::cout << "\n  ========================================\n";
        std::cout << "                   SHOP                   \n";
        std::cout << "  ========================================\n\n";

        std::cout << "    " << std::left << std::setw(5)  << "ID"
                  << std::setw(16) << "Name"
                  << std::setw(14) << "Type"
                  << std::setw(8)  << "Price" << "\n";
        std::cout << "    ------------------------------------------\n";

        for (const auto& item : items) {
            std::cout << "    " << std::left << std::setw(5)  << item.id
                      << std::setw(16) << item.name
                      << std::setw(14) << item.type
                      << std::setw(8)  << item.price << "\n";
        }
        std::cout << "\n";
    }

    // ============================================================
    // Beli itemId untuk `player`, menambahkannya ke `inventory` jika berhasil.
    // ============================================================
    bool buyItem(Player& player, std::vector<Item>& inventory, int itemId) {
        std::vector<Item> catalog = getShopItems();

        // Materi: Lambda Expression, Find
        auto it = std::find_if(catalog.begin(), catalog.end(),
            [&](const Item& shopItem) { return shopItem.id == itemId; });

        // Materi: Exception Handling, item tidak ditemukan
        if (it == catalog.end()) {
            throw GameException("Item ID " + std::to_string(itemId) + " does not exist in the shop.");
        }

        // Materi: Exception Handling, koin tidak mencukupi
        if (player.coin < it->price) {
            throw InsufficientFundsException(
                "Not enough coin to buy " + it->name +
                " (need " + std::to_string(it->price) +
                ", have " + std::to_string(player.coin) + ").");
        }

        // kurangi coin
        player.coin -= it->price;

        // Tambahkan item yang dibeli ke inventory (jumlah 1 per purchase)
        Item purchased = *it;
        purchased.quantity = 1;
        InventoryModule::addItem(inventory, purchased);

        return true;
    }

    // ============================================================
    // Interactive shop screen, dipanggil dari the main menu.
    // ============================================================
    void runShop(Player& player, std::vector<Item>& inventory) {
        bool inShop = true;

        while (inShop) {
            system("cls");
            std::cout << "    Coin: " << player.coin << "\n";
            displayShop();

            std::cout << "    Enter item ID to buy, or 0 to return to menu: ";

            int choice = 0;
            if (!(std::cin >> choice)) {
                // Materi: Exception Handling, input non-numeric
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "\n    Invalid input. Press any key to continue...";
                _getch();
                continue;
            }
            std::cin.ignore(10000, '\n');

            if (choice == 0) {
                inShop = false;
                continue;
            }

            try {
                buyItem(player, inventory, choice);
                std::cout << "\n    Purchase successful! Remaining coin: " << player.coin << "\n";
            } catch (const InsufficientFundsException& e) {
                std::cout << "\n  " << e.what() << "\n";
            } catch (const GameException& e) {
                std::cout << "\n  " << e.what() << "\n";
            }

            std::cout << "\n    Press any key to continue...";
            _getch();
        }
    }

} // namespace ShopModule
