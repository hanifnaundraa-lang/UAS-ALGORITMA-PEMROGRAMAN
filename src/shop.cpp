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

/*==================================================
  MATERI: FUNCTION, STL VECTOR, ITERATOR, FILE HANDLING, EXCEPTION HANDLING, LAMBDA, SORT, FIND, COUNT
==================================================*/
namespace ShopModule {

    // ============================================================
    // Memuat katalog toko ke dalam cache memori dari file CSV dengan mengabaikan baris yang korup.
    // ============================================================
    std::vector<Item> loadShopCatalog() {
        std::vector<Item> items;
        std::ifstream inFile(FileConfig::SHOP_ITEMS_FILE);

        /*==================================================
          MATERI: EXCEPTION HANDLING - MELEMPAR ERROR JIKA FILE KATALOG TIDAK DITEMUKAN.
        ==================================================*/
        if (!inFile.is_open()) {
            throw FileException("Unable to open " + FileConfig::SHOP_ITEMS_FILE + ".");
        }

        std::string line;
        bool isHeader = true;
        while (std::getline(inFile, line)) {
            if (isHeader) { isHeader = false; continue; } // LEVEL 3: Mengabaikan baris header.
            if (line.empty()) continue;

            // Melakukan parse baris CSV dalam try-catch untuk menoleransi kegagalan data.
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

                // Jika kolom stok kosong, diatur secara default sebagai tidak terbatas (-1).
                if (!stockStr.empty()) {
                    item.stock = std::stoi(stockStr);
                } else {
                    item.stock = -1; // unlimited
                }

                items.push_back(item);
            } catch (const std::exception&) {
                // Melompati baris yang rusak secara elegan.
                continue;
            }
        }

        inFile.close();
        return items;
    }

    // ============================================================
    // Menyimpan katalog (beserta stok yang diperbarui) kembali ke dalam file CSV.
    // ============================================================
    void saveShopCatalog(const std::vector<Item>& catalog) {
        std::ofstream outFile(FileConfig::SHOP_ITEMS_FILE);
        if (!outFile.is_open()) {
            throw FileException("Unable to write to " + FileConfig::SHOP_ITEMS_FILE + ".");
        }

        // Menulis header dengan menyertakan kolom stok.
        outFile << "id,name,type,price,effect,stock\n";

        /*==================================================
          MATERI: ITERATOR - MENULIS SETIAP ATRIBUT ITEM KE FILE.
        ==================================================*/
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
    // Menampilkan katalog beserta nilai stok dan koin di dalam dompet pemain.
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

        /*==================================================
          MATERI: ITERATOR - MELAKUKAN LOOP UNTUK MENCETAK DETAIL KATALOG.
        ==================================================*/
        for (const auto& item : catalog) {
            std::cout << "    " << std::left << std::setw(5) << item.id
                      << std::setw(16) << item.name
                      << std::setw(12) << item.type
                      << std::setw(8)  << item.price;

            // Menerjemahkan angka ke dalam status tampilan stok di layar.
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
    // Membeli item menggunakan wallet dengan memastikan jumlah kuantitas dan stok yang cukup.
    // ============================================================
    bool buyItem(Wallet& wallet, std::vector<Item>& catalog,
                 std::vector<Item>& inventory, int itemId, int quantity) {

        /*==================================================
          MATERI: LAMBDA EXPRESSION, FIND - MEMERIKSA KEBERADAAN ITEM DI DALAM KATALOG.
        ==================================================*/
        auto it = std::find_if(catalog.begin(), catalog.end(),
            [&](const Item& shopItem) { return shopItem.id == itemId; });

        /*==================================================
          MATERI: EXCEPTION HANDLING - MENGHASILKAN EXCEPTION BILA ITEM TIDAK ADA.
        ==================================================*/
        if (it == catalog.end()) {
            throw GameException("Item ID " + std::to_string(itemId) + " does not exist in the shop.");
        }

        // Memastikan angka kuantitas lebih dari nol.
        if (quantity <= 0) {
            throw GameException("Quantity must be greater than 0.");
        }

        // Memastikan persediaan stok cukup untuk memenuhi pesanan.
        if (it->stock == 0) {
            throw GameException(it->name + " is out of stock!");
        }
        if (it->stock != -1 && it->stock < quantity) {
            throw GameException("Not enough stock for " + it->name +
                " (available: " + std::to_string(it->stock) +
                ", requested: " + std::to_string(quantity) + ").");
        }

        // Menghitung jumlah koin yang dibutuhkan.
        int totalCost = it->price * quantity;

        // Memastikan dompet pemain mempunyai koin yang mencukupi.
        if (wallet.coin < totalCost) {
            throw InsufficientFundsException(
                "Not enough coin to buy " + std::to_string(quantity) + "x " + it->name +
                " (need " + std::to_string(totalCost) +
                ", have " + std::to_string(wallet.coin) + ").");
        }

        // Mengurangi koin di dalam dompet secara langsung.
        wallet.coin -= totalCost;

        // Memperbarui stok toko bila sifatnya tidak tak terbatas.
        if (it->stock != -1) {
            it->stock -= quantity;
        }

        // Menambahkan barang ke dalam inventory pemain lengkap dengan informasi dasar.
        Item purchased = *it;
        purchased.quantity = quantity;
        InventoryModule::addItem(inventory, purchased);

        // Menambahkan rekaman riwayat transaksi.
        saveShopHistory(wallet.playerName, it->name, quantity, totalCost);

        return true;
    }

    // ============================================================
    /*==================================================
      MATERI: SORT, LAMBDA EXPRESSION - SISTEM PENGURUTAN KATALOG TOKO.
    ==================================================*/
    // ============================================================

    /*==================================================
      MATERI: LAMBDA - MENGURUTKAN HARGA TERENDAH KE TERTINGGI.
    ==================================================*/
    void sortByPriceAsc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.price < b.price; });
    }

    /*==================================================
      MATERI: LAMBDA - MENGURUTKAN HARGA TERTINGGI KE TERENDAH.
    ==================================================*/
    void sortByPriceDesc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.price > b.price; });
    }

    /*==================================================
      MATERI: LAMBDA - MENGURUTKAN BERDASARKAN ALFABET NAMA A KE Z.
    ==================================================*/
    void sortByNameAsc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.name < b.name; });
    }

    /*==================================================
      MATERI: LAMBDA - MENGURUTKAN BERDASARKAN ALFABET NAMA Z KE A.
    ==================================================*/
    void sortByNameDesc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.name > b.name; });
    }

    /*==================================================
      MATERI: LAMBDA - MENGURUTKAN SECARA SEKUENSIAL BERDASAR ID NUMERIK.
    ==================================================*/
    void sortByIdAsc(std::vector<Item>& catalog) {
        std::sort(catalog.begin(), catalog.end(),
            [](const Item& a, const Item& b) { return a.id < b.id; });
    }

    // ============================================================
    // Menyimpan baris rekaman transaksi pembelian ke dalam file CSV khusus.
    // ============================================================
    void saveShopHistory(const std::string& playerName, const std::string& itemName,
                         int quantity, int totalCost) {
        // Membuat header otomatis apabila file belum pernah terbuat.
        bool fileExists = false;
        {
            std::ifstream check(FileConfig::SHOP_HISTORY_FILE);
            fileExists = check.is_open();
            check.close();
        }

        std::ofstream outFile(FileConfig::SHOP_HISTORY_FILE, std::ios::app);
        if (!outFile.is_open()) return;

        // Menambahkan kolom header.
        if (!fileExists) {
            outFile << "date,player,item,quantity,total\n";
        }

        // Memproses format tanggal waktu saat ini secara akurat.
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
    // Membuka layar interaktif utama toko bagi pemain.
    // ============================================================
    void runShop(Wallet& wallet, std::vector<Item>& inventory) {
        // Memuat data dari disk hanya sekali saja ke dalam memory cache.
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
                // Menyimpan perubahan data stok saat pemain keluar dari menu.
                try {
                    saveShopCatalog(catalog);
                } catch (const FileException& e) {
                    std::cout << "\n  " << e.what() << "\n";
                }
                break;
            }

            // Menyediakan sub-menu untuk mengurutkan katalog dengan berbagai opsi.
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

            // Alur pembelian barang.
            int itemId = choice - '0';
            if (itemId < 1 || itemId > 9) {
                std::cout << "\n    Invalid choice. Press any key to continue...";
                _getch();
                continue;
            }

            // Meminta pengguna memberikan input kuantitas yang diinginkan.
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
