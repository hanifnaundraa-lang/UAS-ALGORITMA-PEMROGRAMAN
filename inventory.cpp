// #include "inventory.h"
// #include <iostream>

// // Placeholder — full implementation by Member 2
// namespace InventoryModule {

//     void addItem(std::vector<Item>& inventory, const Item& item) {
//         (void)inventory; (void)item;
//     }

//     void removeItem(std::vector<Item>& inventory, int itemId) {
//         (void)inventory; (void)itemId;
//     }

//     void displayInventory(const std::vector<Item>& inventory) {
//         (void)inventory;
//         std::cout << "[Inventory] Coming soon..." << std::endl;
//     }

//     void saveInventory(const std::string& playerName, const std::vector<Item>& inventory) {
//         (void)playerName; (void)inventory;
//     }

//     std::vector<Item> loadInventory(const std::string& playerName) {
//         (void)playerName;
//         return {};
//     }

// } // namespace InventoryModule


#include "inventory.h"
#include "gameconfig.h"
#include "gameexception.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;

// Materi: Function, STL Vector, Iterator, File Handling, Exception Handling, Lambda
namespace InventoryModule {
    // Add item, menggabungkan jumlah if the item (by id) already exists
    void addItem(vector<Item>& inventory, const Item& item) {
        // Materi: Lambda Expression, Find
        auto it = find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == item.id; });

        if (it != inventory.end()) {
            it->quantity += item.quantity;
        } else {
            inventory.push_back(item);
        }
    }

    // Menghapus satu unit dari itemId. hapus row if quantity hits 0.
    bool removeItem(vector<Item>& inventory, int itemId) {
        // Materi: Lambda Expression, Find
        auto it = find_if(inventory.begin(), inventory.end(),
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

    // Cari item berdasarkan id (mutable pointer, nullptr if not found)
    Item* findItem(vector<Item>& inventory, int itemId) {
        // Material: Lambda Expression, Find
        auto it = find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == itemId; });
        return (it != inventory.end()) ? &(*it) : nullptr;
    }

    // Hitung total jumlah yang dimiliki untuk itemId tertentu
    int countItem(const vector<Item>& inventory, int itemId) {
        // Material: Lambda Expression, Find
        auto it = find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == itemId; });
        return (it != inventory.end()) ? it->quantity : 0;
    }

    // Tampilkan inventory sebagai tabel yang diformat
    void displayInventory(const vector<Item>& inventory) {
        cout << "\n  ========================================\n";
        cout << "                INVENTORY                \n";
        cout << "  ========================================\n\n";

        if (inventory.empty()) {
            cout << "    (Inventory is empty)\n\n";
            return;
        }

        cout << "    " << left << setw(5)  << "ID"
             << setw(18) << "Name"
             << setw(14) << "Type"
             << setw(8)  << "Qty" << "\n";
        cout << "    ------------------------------------\n";

        for (const auto& item : inventory) {
            cout << "    " << left << setw(5)  << item.id
                 << setw(18) << item.name
                 << setw(14) << item.type
                 << setw(8)  << item.quantity << "\n";
        }
        cout << "\n";
    }

    // Save inventory ke data/inventory.csv
    void saveInventory(const string& playerName, const vector<Item>& inventory) {
        // Materi: File Handling, baca baris yang ada dari pemain lain terlebih dahulu
        vector<string> otherPlayerLines;
        ifstream inFile(FileConfig::INVENTORY_FILE);

        if (inFile.is_open()) {
            string line;
            bool isHeader = true;
            while (getline(inFile, line)) {
                if (isHeader) { isHeader = false; continue; } // skip header
                if (line.empty()) continue;

                stringstream ss(line);
                string ownerName;
                getline(ss, ownerName, ',');

                if (ownerName != playerName) {
                    otherPlayerLines.push_back(line);
                }
            }
            inFile.close();
        }

        // Materi: Exception Handling (gagal if file tidak dapat ditulis)
        ofstream outFile(FileConfig::INVENTORY_FILE);
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

    // Load inventory rows milik playerName from data/inventory.csv
    vector<Item> loadInventory(const string& playerName) {
        vector<Item> result;
        ifstream inFile(FileConfig::INVENTORY_FILE);

        // Materi: Exception Handling, file hilang tidak fatal
        if (!inFile.is_open()) {
            return result;
        }

        string line;
        bool isHeader = true;
        while (getline(inFile, line)) {
            if (isHeader) { isHeader = false; continue; } // skip header
            if (line.empty()) continue;

            stringstream ss(line);
            string ownerName, idStr, name, type, qtyStr;

            getline(ss, ownerName, ',');
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, type, ',');
            getline(ss, qtyStr, ',');

            if (ownerName != playerName) continue;

            Item item;
            item.id = stoi(idStr);
            item.name = name;
            item.type = type;
            item.price = 0;       
            item.quantity = stoi(qtyStr);

            result.push_back(item);
        }

        inFile.close();
        return result;
    }

} // namespace InventoryModule