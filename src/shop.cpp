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
#include <chrono>
#include <ctime>

// Materi: Function, STL Vector, Iterator, File Handling, Exception Handling, Lambda, Sort, Find, Count
namespace ShopModule {

    // ============================================================
    // Feature 4: Load shop catalog sekali dan cache di memory
    // Feature 3: CSV Validation — skip baris corrupt
    // Feature 8: Format CSV: id,name,type,price,effect,stock
    // ============================================================
    std::vector<Item> loadShopCatalog() {
        std::vector<Item> items;
        std::ifstream inFile(FileConfig::SHOP_ITEMS_FILE);

        // Materi: Exception Handling — file katalog hilang
        if (!inFile.is_open()) {
            throw FileException("Unable to open " + FileConfig::SHOP_ITEMS_FILE + ".");
        }

        std::string line;
        bool isHeader = true;
        while (std::getline(inFile, line)) {
            if (isHeader) { isHeader = false; continue; } // skip header
            if (line.empty()) continue;

            // Feature 3: CSV Validation — wrap setiap row di try/catch
            try {
                std::stringstream ss(line);
                std::string idStr, name, type, priceStr, effect, stockStr;

                std::getline(ss, idStr, ',');
                std::getline(ss, name, ',');
                std::getline(ss, type, ',');
                std::getline(ss, priceStr, ',');
                std::getline(ss, effect, ',');
                std::getline(ss, stockStr, ',');

                Item item;
                item.id = std::stoi(idStr);
                item.name = name;
                item.type = type;
                item.price = std::stoi(priceStr);
                item.effect = effect;
                item.quantity = 0; // catalog entry, bukan inventory

                // Feature 8: Stock — jika kolom stock tidak ada, default unlimited (-1)
                if (!stockStr.empty()) {
                    item.stock = std::stoi(stockStr);
                } else {
                    item.stock = -1; // unlimited
                }

                items.push_back(item);
            } catch (const std::exception&) {
                // Feature 3: Skip invalid/corrupted rows, continue
                continue;
            }
        }

        inFile.close();
        return items;
    }

    // ============================================================
    // Feature 8: Save updated stock back ke shop_items.csv
    // ============================================================
    void saveShopCatalog(const std::vector<Item>& catalog) {
        std::ofstream outFile(FileConfig::SHOP_ITEMS_FILE);
        if (!outFile.is_open()) {
            throw FileException("Unable to write to " + FileConfig::SHOP_ITEMS_FILE + ".");
        }

        // Feature 8: Updated CSV header with stock column
        outFile << "id,name,type,price,effect,stock\n";

        // Material: Iterator — write setiap item
        for (const auto& item : catalog) {
            outFile << item.id << ","
                    << item.name << ","
                    << item.type << ","
                    << item.price << ","
                    << item.effect << ","
                    << item.stock << "\n";
        }
        outFile.close();
    }

    // ============================================================
    // Feature 8: Tampilkan katalog shop dengan stok
    // Feature 1: Tampilkan saldo wallet
    // ============================================================
    void displayShop(const std::vector<Item>& catalog, int walletCoin) {
        std::cout << "\n  ========================================\n";
        std::cout << "                   SHOP                   \n";
        std::cout << "  ========================================\n\n";
        std::cout << "    Wallet Coin: " << walletCoin << "\n\n";

        std::cout << "    " << std::left << std::setw(5)  << "ID"
                  << std::setw(16) << "Name"
                  << std::setw(12) << "Type"
                  << std::setw(8)  << "Price"
                  << std::setw(10) << "Stock" << "\n";
        std::cout << "    -------------------------------------------------\n";

        // Material: Iterator — tampilkan setiap item
        for (const auto& item : catalog) {
            std::cout << "    " << std::left << std::setw(5) << item.id
                      << std::setw(16) << item.name
                      << std::setw(12) << item.type
                      << std::setw(8)  << item.price;

            // Feature 8: Tampilkan stock status
            if (item.stock == -1) {
                std::cout << std::setw(10) << "Unlimited";
            } else if (item.stock == 0) {
                std::cout << std::setw(10) << "SOLD OUT";
            } else {
                std::cout << std::setw(10) << item.stock;
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    // ============================================================
    // Feature 1, 5, 8: Buy item using Wallet with quantity and stock
    // ============================================================
    bool buyItem(Wallet& wallet, std::vector<Item>& catalog,
                 std::vector<Item>& inventory, int itemId, int quantity) {

        // Materi: Lambda Expression, Find — cari item di catalog
        auto it = std::find_if(catalog.begin(), catalog.end(),
            [&](const Item& shopItem) { return shopItem.id == itemId; });

        // Materi: Exception Handling — item tidak ditemukan
        if (it == catalog.end()) {
            throw GameException("Item ID " + std::to_string(itemId) + " does not exist in the shop.");
        }

        // Feature 5: Validasi quantity
        if (quantity <= 0) {
            throw GameException("Quantity must be greater than 0.");
        }

        // Feature 8: Validasi stock
        if (it->stock == 0) {
            throw GameException(it->name + " is out of stock!");
        }
        if (it->stock != -1 && it->stock < quantity) {
            throw GameException("Not enough stock for " + it->name +
                " (available: " + std::to_string(it->stock) +
                ", requested: " + std::to_string(quantity) + ").");
        }

        // Feature 5: Hitung total cost
        int totalCost = it->price * quantity;

        // Feature 1: Validasi coin wallet (bukan player.coin)
        if (wallet.coin < totalCost) {
            throw InsufficientFundsException(
                "Not enough coin to buy " + std::to_string(quantity) + "x " + it->name +
                " (need " + std::to_string(totalCost) +
                ", have " + std::to_string(wallet.coin) + ").");
        }

        // Feature 1: Kurangi dari wallet
        wallet.coin -= totalCost;

        // Feature 8: Kurangi stock (jika bukan unlimited)
        if (it->stock != -1) {
            it->stock -= quantity;
        }

        // Tambahkan item ke inventory dengan quantity, price, dan effect
        // Feature 2: Price dan effect disimpan ke inventory
        Item purchased = *it;
        purchased.quantity = quantity;
        InventoryModule::addItem(inventory, purchased);

        // Feature 9: Save purchase history
        saveShopHistory(wallet.playerName, it->name, quantity, totalCost);

        return true;
    }

    // ============================================================
    // Feature 10: Sorting System
    // Material: Sort, Lambda Expression
    // ============================================================

    // Material: Lambda — Sort harga terendah
    void sortByPriceAsc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.price < b.price; });
    }

    // Material: Lambda — Sort harga tertinggi
    void sortByPriceDesc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.price > b.price; });
    }

    // Material: Lambda — Sort nama A-Z
    void sortByNameAsc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.name < b.name; });
    }

    // Material: Lambda — Sort nama Z-A
    void sortByNameDesc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.name > b.name; });
    }

    // Material: Lambda — Sort ID (Low to High)
    void sortByIdAsc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.id < b.id; });
    }

    // ============================================================
    // Feature 9: Save purchase history ke data/shop_history.csv
    // Format: date,player,item,quantity,total
    // ============================================================
    void saveShopHistory(const std::string& playerName, const std::string& itemName,
                         int quantity, int totalCost) {
        // Cek apakah file sudah ada, jika belum buat header
        bool fileExists = false;
        {
            std::ifstream check(FileConfig::SHOP_HISTORY_FILE);
            fileExists = check.is_open();
            check.close();
        }

        std::ofstream outFile(FileConfig::SHOP_HISTORY_FILE, std::ios::app);
        if (!outFile.is_open()) return;

        // Tulis header jika file baru
        if (!fileExists) {
            outFile << "date,player,item,quantity,total\n";
        }

        // Dapatkan tanggal hari ini
        auto now = std::chrono::system_clock::now();
        time_t t = std::chrono::system_clock::to_time_t(now);
        tm* local = localtime(&t);
        std::stringstream dateStr;
        dateStr << std::put_time(local, "%Y-%m-%d");

        outFile << dateStr.str() << ","
                << playerName << ","
                << itemName << ","
                << quantity << ","
                << totalCost << "\n";
        outFile.close();
    }

    // ============================================================
    // Feature 1, 4, 5, 8, 9, 10: Interactive shop screen
    // ============================================================
    void runShop(Wallet& wallet, std::vector<Item>& inventory) {
        // Feature 4: Load catalog once and cache
        std::vector<Item> catalog;
        try {
            catalog = loadShopCatalog();
        } catch (const FileException& e) {
            std::cout << "\n  " << e.what() << "\n";
            std::cout << "    Press any key to continue...";
            _getch();
            return;
        }

        char choice;
        do {
            system("cls");
            displayShop(catalog, wallet.coin);

            std::cout << "    [1-5] Enter item ID to buy\n";
            std::cout << "    [S]   Sort Options\n";
            std::cout << "    [0]   Back to Main Menu\n";
            std::cout << "\n    Your choice: ";
            choice = _getch();
            std::cout << choice << "\n";

            if (choice == '0') {
                // Feature 8: Save stock sebelum keluar
                try {
                    saveShopCatalog(catalog);
                } catch (const FileException& e) {
                    std::cout << "\n  " << e.what() << "\n";
                }
                break;
            }

            // Feature 10: Sort menu
            if (choice == 's' || choice == 'S') {
                std::cout << "\n    Sort Options:\n";
                std::cout << "    [1] Price: Lowest First\n";
                std::cout << "    [2] Price: Highest First\n";
                std::cout << "    [3] Name: A-Z\n";
                std::cout << "    [4] Name: Z-A\n";
                std::cout << "    [5] ID: Low to High\n";
                std::cout << "\n    Sort by: ";
                char sortChoice = _getch();
                std::cout << sortChoice << "\n";

                switch (sortChoice) {
                    case '1': sortByPriceAsc(catalog);  std::cout << "    Sorted by price (low to high).\n"; break;
                    case '2': sortByPriceDesc(catalog); std::cout << "    Sorted by price (high to low).\n"; break;
                    case '3': sortByNameAsc(catalog);   std::cout << "    Sorted by name (A-Z).\n"; break;
                    case '4': sortByNameDesc(catalog);  std::cout << "    Sorted by name (Z-A).\n"; break;
                    case '5': sortByIdAsc(catalog);     std::cout << "    Sorted by ID (low to high).\n"; break;
                    default: std::cout << "    Invalid sort option.\n"; break;
                }
                std::cout << "    Press any key to continue...";
                _getch();
                continue;
            }

            // Purchase flow
            int itemId = choice - '0';
            if (itemId < 1 || itemId > 9) {
                std::cout << "\n    Invalid choice. Press any key to continue...";
                _getch();
                continue;
            }

            // Feature 5: Ask for quantity
            std::cout << "\n    Quantity (enter amount): ";
            int qty = 0;
            if (!(std::cin >> qty)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "    Invalid input. Press any key to continue...";
                _getch();
                continue;
            }
            std::cin.ignore(10000, '\n');

            try {
                buyItem(wallet, catalog, inventory, itemId, qty);
                std::cout << "\n    Purchase successful! Bought " << qty << " item(s)."
                          << " Remaining coin: " << wallet.coin << "\n";
            } catch (const InsufficientFundsException& e) {
                std::cout << "\n  " << e.what() << "\n";
            } catch (const GameException& e) {
                std::cout << "\n  " << e.what() << "\n";
            }

            std::cout << "\n    Press any key to continue...";
            _getch();
        } while (true);
    }

} // namespace ShopModule
