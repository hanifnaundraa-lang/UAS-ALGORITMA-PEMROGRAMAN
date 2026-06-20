#include "inventory.h"
#include "gameconfig.h"
#include "gameexception.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <conio.h>

// Materi: Function, STL Vector, Iterator, File Handling, Exception Handling, Lambda, Find, Count, Sort
namespace InventoryModule {

    // ============================================================
    // Add item, menggabungkan jumlah if the item (by id) already exists
    // ============================================================
    void addItem(std::vector<Item>& inventory, const Item& item) {
        // Materi: Lambda Expression, Find
        auto it = std::find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == item.id; });

        if (it != inventory.end()) {
            it->quantity += item.quantity;
        } else {
            inventory.push_back(item);
        }
    }

    // ============================================================
    // Menghapus satu unit dari itemId. hapus row if quantity hits 0.
    // ============================================================
    bool removeItem(std::vector<Item>& inventory, int itemId) {
        // Materi: Lambda Expression, Find
        auto it = std::find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == itemId; });

        if (it == inventory.end()) {
            return false;
        }

        it->quantity -= 1;
        if (it->quantity <= 0) {
            inventory.erase(it);
        }
        return true;
    }

    // ============================================================
    // Cari item berdasarkan id (mutable pointer, nullptr if not found)
    // ============================================================
    Item* findItem(std::vector<Item>& inventory, int itemId) {
        // Material: Lambda Expression, Find
        auto it = std::find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == itemId; });
        return (it != inventory.end()) ? &(*it) : nullptr;
    }

    // ============================================================
    // Hitung total jumlah yang dimiliki untuk itemId tertentu
    // ============================================================
    int countItem(const std::vector<Item>& inventory, int itemId) {
        // Material: Lambda Expression, Find
        auto it = std::find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == itemId; });
        return (it != inventory.end()) ? it->quantity : 0;
    }

    // ============================================================
    // Feature 6: Search item berdasarkan nama (keyword)
    // Material: Find, Lambda, Iterator
    // ============================================================
    std::vector<Item> searchByName(const std::vector<Item>& inventory, const std::string& keyword) {
        std::vector<Item> results;
        std::string lowerKeyword = keyword;
        // Material: Lambda — transform to lowercase for case-insensitive search
        std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(),
            [](unsigned char c) { return std::tolower(c); });

        // Material: Iterator — iterate and filter matching items
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            std::string lowerName = it->name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                [](unsigned char c) { return std::tolower(c); });

            // Material: Find — check if keyword appears in name
            if (lowerName.find(lowerKeyword) != std::string::npos) {
                results.push_back(*it);
            }
        }
        return results;
    }

    // ============================================================
    // Feature 7: Filter item berdasarkan type
    // Material: Iterator, Lambda
    // ============================================================
    std::vector<Item> filterByType(const std::vector<Item>& inventory, const std::string& type) {
        std::vector<Item> results;
        std::string lowerType = type;
        std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(),
            [](unsigned char c) { return std::tolower(c); });

        // Material: Iterator, Lambda — filter by type
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            std::string lowerItemType = it->type;
            std::transform(lowerItemType.begin(), lowerItemType.end(), lowerItemType.begin(),
                [](unsigned char c) { return std::tolower(c); });

            if (lowerItemType == lowerType) {
                results.push_back(*it);
            }
        }
        return results;
    }

    // ============================================================
    // Tampilkan inventory sebagai tabel yang diformat
    // ============================================================
    void displayInventory(const std::vector<Item>& inventory) {
        std::cout << "\n  ========================================\n";
        std::cout << "                INVENTORY                \n";
        std::cout << "  ========================================\n\n";

        if (inventory.empty()) {
            std::cout << "    (Inventory is empty)\n\n";
            return;
        }

        std::cout << "    " << std::left << std::setw(5)  << "ID"
                  << std::setw(18) << "Name"
                  << std::setw(14) << "Type"
                  << std::setw(8)  << "Price"
                  << std::setw(8)  << "Qty" << "\n";
        std::cout << "    -----------------------------------------------\n";

        for (const auto& item : inventory) {
            std::cout << "    " << std::left << std::setw(5)  << item.id
                      << std::setw(18) << item.name
                      << std::setw(14) << item.type
                      << std::setw(8)  << item.price
                      << std::setw(8)  << item.quantity << "\n";
        }
        std::cout << "\n";
    }

    // ============================================================
    // Feature 6 & 7: Interactive Inventory Menu
    // ============================================================
    void runInventory(std::vector<Item>& inventory, const std::string& playerName) {
        char choice;
        do {
            system("cls");
            displayInventory(inventory);

            std::cout << "    [1] Search Item by Name\n";
            std::cout << "    [2] Filter by Type\n";
            std::cout << "    [0] Back to Main Menu\n";
            std::cout << "\n    Your choice: ";
            choice = _getch();
            std::cout << choice << "\n\n";

            if (choice == '1') {
                // Feature 6: Search
                std::cout << "    Enter search keyword: ";
                std::string keyword;
                std::getline(std::cin, keyword);

                if (keyword.empty()) {
                    std::cout << "    No keyword entered.\n";
                } else {
                    auto results = searchByName(inventory, keyword);
                    if (results.empty()) {
                        std::cout << "\n    No items found matching \"" << keyword << "\".\n";
                    } else {
                        std::cout << "\n    Found " << results.size() << " item(s):\n";
                        displayInventory(results);
                    }
                }
                std::cout << "    Press any key to continue...";
                _getch();
            } else if (choice == '2') {
                // Feature 7: Filter
                std::cout << "    Available types: Consumable, Currency, Buff, Weapon\n";
                std::cout << "    Enter type to filter: ";
                std::string type;
                std::getline(std::cin, type);

                if (type.empty()) {
                    std::cout << "    No type entered.\n";
                } else {
                    auto results = filterByType(inventory, type);
                    if (results.empty()) {
                        std::cout << "\n    No items of type \"" << type << "\" found.\n";
                    } else {
                        std::cout << "\n    Showing " << results.size() << " item(s) of type \"" << type << "\":\n";
                        displayInventory(results);
                    }
                }
                std::cout << "    Press any key to continue...";
                _getch();
            }
        } while (choice != '0');
    }

    // ============================================================
    // Feature 2: Save inventory ke data/inventory.csv
    // Format: playerName,itemId,itemName,type,price,effect,quantity
    // Mengganti semua baris milik playerName, mempertahankan baris pemain lain.
    // ============================================================
    void saveInventory(const std::string& playerName, const std::vector<Item>& inventory) {
        // Materi: File Handling, baca baris yang ada dari pemain lain terlebih dahulu
        std::vector<std::string> otherPlayerLines;
        std::ifstream inFile(FileConfig::INVENTORY_FILE);

        if (inFile.is_open()) {
            std::string line;
            bool isHeader = true;
            while (std::getline(inFile, line)) {
                if (isHeader) { isHeader = false; continue; } // skip header
                if (line.empty()) continue;

                std::stringstream ss(line);
                std::string ownerName;
                std::getline(ss, ownerName, ',');

                if (ownerName != playerName) {
                    otherPlayerLines.push_back(line);
                }
            }
            inFile.close();
        }

        // Materi: Exception Handling (gagal if file tidak dapat ditulis)
        std::ofstream outFile(FileConfig::INVENTORY_FILE);
        if (!outFile.is_open()) {
            throw FileException("Unable to open " + FileConfig::INVENTORY_FILE + " for writing.");
        }

        // Feature 2: Updated header with price and effect columns
        outFile << "playerName,itemId,itemName,type,price,effect,quantity\n";

        for (const auto& line : otherPlayerLines) {
            outFile << line << "\n";
        }

        for (const auto& item : inventory) {
            outFile << playerName << ","
                    << item.id << ","
                    << item.name << ","
                    << item.type << ","
                    << item.price << ","
                    << item.effect << ","
                    << item.quantity << "\n";
        }

        outFile.close();
    }

    // ============================================================
    // Feature 2 & 3: Load inventory with price/effect and CSV validation
    // Backward compatible: handles both old (5-column) and new (7-column) formats
    // ============================================================
    std::vector<Item> loadInventory(const std::string& playerName) {
        std::vector<Item> result;
        std::ifstream inFile(FileConfig::INVENTORY_FILE);

        // Materi: Exception Handling, file hilang tidak fatal
        if (!inFile.is_open()) {
            return result;
        }

        std::string line;
        bool isHeader = true;
        while (std::getline(inFile, line)) {
            if (isHeader) { isHeader = false; continue; } // skip header
            if (line.empty()) continue;

            // Feature 3: CSV Validation — wrap each row in try/catch
            try {
                std::stringstream ss(line);
                std::string ownerName, idStr, name, type;
                std::getline(ss, ownerName, ',');
                std::getline(ss, idStr, ',');
                std::getline(ss, name, ',');
                std::getline(ss, type, ',');

                if (ownerName != playerName) continue;

                // Feature 2 & 3: Try reading new format (7 columns)
                // Backward compatible with old format (5 columns)
                std::string field5, field6, field7;
                std::getline(ss, field5, ',');
                std::getline(ss, field6, ',');
                std::getline(ss, field7, ',');

                Item item;
                item.id = std::stoi(idStr);
                item.name = name;
                item.type = type;
                item.stock = -1;

                if (!field7.empty()) {
                    // New format: price,effect,quantity
                    item.price = std::stoi(field5);
                    item.effect = field6;
                    item.quantity = std::stoi(field7);
                } else if (!field6.empty()) {
                    // Could be either format — try as new format first
                    try {
                        item.price = std::stoi(field5);
                        item.effect = field6;
                        item.quantity = 1; // default
                    } catch (...) {
                        // Old format: quantity was in field5
                        item.price = 0;
                        item.effect = "";
                        item.quantity = std::stoi(field5);
                    }
                } else {
                    // Old format: playerName,itemId,itemName,type,quantity
                    item.price = 0;
                    item.effect = "";
                    item.quantity = std::stoi(field5);
                }

                result.push_back(item);
            } catch (const std::exception&) {
                // Feature 3: Skip corrupted rows, continue loading
                continue;
            }
        }

        inFile.close();
        return result;
    }

} // namespace InventoryModule
