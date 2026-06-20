#include "inventory.h"
#include "gameconfig.h"
#include "gameexception.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

// Materi: Function, STL Vector, Iterator, File Handling, Exception Handling, Lambda
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
                  << std::setw(8)  << "Qty" << "\n";
        std::cout << "    ------------------------------------\n";

        for (const auto& item : inventory) {
            std::cout << "    " << std::left << std::setw(5)  << item.id
                      << std::setw(18) << item.name
                      << std::setw(14) << item.type
                      << std::setw(8)  << item.quantity << "\n";
        }
        std::cout << "\n";
    }

    // ============================================================
    // Save inventory ke data/inventory.csv
    // Format: playerName,itemId,itemName,type,quantity
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

        outFile << "playerName,itemId,itemName,type,quantity\n";

        for (const auto& line : otherPlayerLines) {
            outFile << line << "\n";
        }

        for (const auto& item : inventory) {
            outFile << playerName << ","
                    << item.id << ","
                    << item.name << ","
                    << item.type << ","
                    << item.quantity << "\n";
        }

        outFile.close();
    }

    // ============================================================
    // Load inventory rows milik playerName from data/inventory.csv
    // ============================================================
    std::vector<Item> loadInventory(const std::string& playerName) {
        std::vector<Item> result;
        std::ifstream inFile(FileConfig::INVENTORY_FILE);

        // Materi: Exception Handling, file hilang tidak fatal, hanya inventory kosong yang dikembalikan
        if (!inFile.is_open()) {
            return result;
        }

        std::string line;
        bool isHeader = true;
        while (std::getline(inFile, line)) {
            if (isHeader) { isHeader = false; continue; } // skip header
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string ownerName, idStr, name, type, qtyStr;

            std::getline(ss, ownerName, ',');
            std::getline(ss, idStr, ',');
            std::getline(ss, name, ',');
            std::getline(ss, type, ',');
            std::getline(ss, qtyStr, ',');

            if (ownerName != playerName) continue;

            Item item;
            item.id = std::stoi(idStr);
            item.name = name;
            item.type = type;
            item.price = 0;       // price tidak disimpan di inventory.csv
            item.effect = "";     // effect tidak disimpan di inventory.csv
            item.quantity = std::stoi(qtyStr);

            result.push_back(item);
        }

        inFile.close();
        return result;
    }

} // namespace InventoryModule
