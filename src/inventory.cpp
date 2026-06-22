#include "inventory.h"
#include "gameconfig.h"
#include "gameexception.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <conio.h>

/*==================================================
  MATERI: FUNCTION, STL VECTOR, ITERATOR, FILE HANDLING, EXCEPTION HANDLING, LAMBDA, FIND, COUNT, SORT
==================================================*/
namespace InventoryModule {

    // ============================================================
    // Menambahkan item ke inventory. Jika item sudah ada, jumlahnya ditambahkan.
    // ============================================================
    void addItem(std::vector<Item>& inventory, const Item& item) {
        /*==================================================
          MATERI: LAMBDA EXPRESSION, FIND
        ==================================================*/
        auto it = std::find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == item.id; });

        if (it != inventory.end()) {
            it->quantity += item.quantity;
        } else {
            inventory.push_back(item);
        }
    }

    // ============================================================
    // Mengurangi satu item berdasarkan ID. Jika jumlah habis, item dihapus dari inventory.
    // ============================================================
    bool removeItem(std::vector<Item>& inventory, int itemId) {
        /*==================================================
          MATERI: LAMBDA EXPRESSION, FIND
        ==================================================*/
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
    // Mencari item berdasarkan ID dan mengembalikan pointer mutable (nullptr jika tidak ada).
    // ============================================================
    Item* findItem(std::vector<Item>& inventory, int itemId) {
        /*==================================================
          MATERI: LAMBDA EXPRESSION, FIND
        ==================================================*/
        auto it = std::find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == itemId; });
        return (it != inventory.end()) ? &(*it) : nullptr;
    }

    // ============================================================
    // Menghitung total jumlah dari suatu item di dalam inventory.
    // ============================================================
    int countItem(const std::vector<Item>& inventory, int itemId) {
        /*==================================================
          MATERI: LAMBDA EXPRESSION, FIND
        ==================================================*/
        auto it = std::find_if(inventory.begin(), inventory.end(),
            [&](const Item& existing) { return existing.id == itemId; });
        return (it != inventory.end()) ? it->quantity : 0;
    }

    // ============================================================
    /*==================================================
      MATERI: FIND, LAMBDA, ITERATOR - PENCARIAN ITEM BERDASARKAN KATA KUNCI NAMA.
    ==================================================*/
    // ============================================================
    std::vector<Item> searchByName(const std::vector<Item>& inventory, const std::string& keyword) {
        std::vector<Item> results;
        std::string lowerKeyword = keyword;
        /*==================================================
          MATERI: LAMBDA - MENGUBAH TEKS MENJADI HURUF KECIL UNTUK PENCARIAN YANG TIDAK SENSITIF KAPITAL.
        ==================================================*/
        std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(),
            [](unsigned char c) { return std::tolower(c); });

        /*==================================================
          MATERI: ITERATOR - MELAKUKAN ITERASI DAN MEMFILTER ITEM YANG COCOK.
        ==================================================*/
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            std::string lowerName = it->name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                [](unsigned char c) { return std::tolower(c); });

            /*==================================================
              MATERI: FIND - MEMERIKSA APAKAH KATA KUNCI TERDAPAT DI DALAM NAMA ITEM.
            ==================================================*/
            if (lowerName.find(lowerKeyword) != std::string::npos) {
                results.push_back(*it);
            }
        }
        return results;
    }

    // ============================================================
    /*==================================================
      MATERI: ITERATOR, LAMBDA - MEMFILTER ITEM BERDASARKAN TIPENYA.
    ==================================================*/
    // ============================================================
    std::vector<Item> filterByType(const std::vector<Item>& inventory, const std::string& type) {
        std::vector<Item> results;
        std::string lowerType = type;
        std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(),
            [](unsigned char c) { return std::tolower(c); });

        /*==================================================
          MATERI: ITERATOR, LAMBDA - MELAKUKAN FILTER BERDASARKAN STRING TIPE.
        ==================================================*/
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
    // Menampilkan data inventory dalam bentuk tabel ke console.
    // ============================================================
    void displayInventory(const std::vector<Item>& inventory) {
        std::cout << "\n  ========================================\n";
        std::cout << GameColor::MENU_TITLE << "                INVENTORY                \n" << GameColor::RESET;
        std::cout << "  ========================================\n\n";

        if (inventory.empty()) {
            std::cout << "    " << GameColor::TXT_WARNING << "(Inventory is empty)" << GameColor::RESET << "\n\n";
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
    // Menu interaktif untuk melihat dan mengelola inventory pemain.
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
                // Fitur pencarian.
                std::cout << "    Enter search keyword: ";
                std::string keyword;
                std::getline(std::cin, keyword);

                if (keyword.empty()) {
                    std::cout << "    " << GameColor::TXT_WARNING << "No keyword entered." << GameColor::RESET << "\n";
                } else {
                    auto results = searchByName(inventory, keyword);
                    if (results.empty()) {
                        std::cout << "\n    " << GameColor::TXT_WARNING << "No items found matching \"" << keyword << "\"." << GameColor::RESET << "\n";
                    } else {
                        std::cout << "\n    " << GameColor::TXT_SUCCESS << "Found " << results.size() << " item(s):" << GameColor::RESET << "\n";
                        displayInventory(results);
                    }
                }
                std::cout << "    Press any key to continue...";
                _getch();
            } else if (choice == '2') {
                // Fitur filter.
                std::cout << "    Available types: Consumable, Currency, Buff, Weapon\n";
                std::cout << "    Enter type to filter: ";
                std::string type;
                std::getline(std::cin, type);

                if (type.empty()) {
                    std::cout << "    " << GameColor::TXT_WARNING << "No type entered." << GameColor::RESET << "\n";
                } else {
                    auto results = filterByType(inventory, type);
                    if (results.empty()) {
                        std::cout << "\n    " << GameColor::TXT_WARNING << "No items of type \"" << type << "\" found." << GameColor::RESET << "\n";
                    } else {
                        std::cout << "\n    " << GameColor::TXT_SUCCESS << "Showing " << results.size() << " item(s) of type \"" << type << "\":" << GameColor::RESET << "\n";
                        displayInventory(results);
                    }
                }
                std::cout << "    Press any key to continue...";
                _getch();
            }
        } while (choice != '0');
    }

    // ============================================================
    // Menyimpan data inventory pemain ke file CSV. Baris milik pemain lain dipertahankan.
    // ============================================================
    void saveInventory(const std::string& playerName, const std::vector<Item>& inventory) {
        /*==================================================
          MATERI: FILE HANDLING - MEMBACA DATA INVENTORY DARI PEMAIN LAIN AGAR TIDAK TERTIMPA.
        ==================================================*/
        std::vector<std::string> otherPlayerLines;
        std::ifstream inFile(FileConfig::INVENTORY_FILE);

        if (inFile.is_open()) {
            std::string line;
            bool isHeader = true;
            while (std::getline(inFile, line)) {
                if (isHeader) { isHeader = false; continue; } // LEVEL 3: Mengabaikan baris header.
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

        /*==================================================
          MATERI: EXCEPTION HANDLING - MELEMPAR EXCEPTION JIKA FILE TIDAK DAPAT DIBUKA UNTUK DITULIS.
        ==================================================*/
        std::ofstream outFile(FileConfig::INVENTORY_FILE);
        if (!outFile.is_open()) {
            throw FileException("Unable to open " + FileConfig::INVENTORY_FILE + " for writing.");
        }

        // Menulis baris header dengan penambahan kolom price dan effect.
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
    // Membaca file inventory dari CSV. Mendukung format lama (5 kolom) dan baru (7 kolom).
    // ============================================================
    std::vector<Item> loadInventory(const std::string& playerName) {
        std::vector<Item> result;
        std::ifstream inFile(FileConfig::INVENTORY_FILE);

        /*==================================================
          MATERI: EXCEPTION HANDLING - MENGEMBALIKAN LIST KOSONG ALIH-ALIH CRASH JIKA FILE TIDAK ADA.
        ==================================================*/
        if (!inFile.is_open()) {
            return result;
        }

        std::string line;
        bool isHeader = true;
        while (std::getline(inFile, line)) {
            if (isHeader) { isHeader = false; continue; } // LEVEL 3: Mengabaikan baris header.
            if (line.empty()) continue;

            // Validasi CSV dengan menempatkan pembacaan tiap baris di dalam blok try-catch.
            try {
                std::stringstream ss(line);
                std::string ownerName, idStr, name, type;
                std::getline(ss, ownerName, ',');
                std::getline(ss, idStr, ',');
                std::getline(ss, name, ',');
                std::getline(ss, type, ',');

                if (ownerName != playerName) continue;

                // Membaca data dengan kompatibilitas terhadap format tabel lama dan baru.
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
                    // Format baru (termasuk kolom price, effect, quantity).
                    item.price = std::stoi(field5);
                    item.effect = field6;
                    item.quantity = std::stoi(field7);
                } else if (!field6.empty()) {
                    // Format ambigu, dicoba sebagai format baru terlebih dahulu.
                    try {
                        item.price = std::stoi(field5);
                        item.effect = field6;
                        item.quantity = 1; // default
                    } catch (...) {
                        // Jika gagal, diasumsikan format lama di mana jumlah (quantity) berada di kolom kelima.
                        item.price = 0;
                        item.effect = "";
                        item.quantity = std::stoi(field5);
                    }
                } else {
                    // Format lama (playerName, itemId, itemName, type, quantity).
                    item.price = 0;
                    item.effect = "";
                    item.quantity = std::stoi(field5);
                }

                result.push_back(item);
            } catch (const std::exception&) {
                // Mengabaikan baris yang korup dan melanjutkan pemuatan data.
                continue;
            }
        }

        inFile.close();
        return result;
    }

} // namespace InventoryModule
