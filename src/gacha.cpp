#include "gacha.h"
#include "gameconfig.h"
#include "gameexception.h"
#include "inventory.h"
#include "trading.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <numeric>

/*==================================================
  MATERI: FUNCTION, NAMESPACE, STL VECTOR, ITERATOR, SORT, FIND, COUNT, LAMBDA, FILE HANDLING, EXCEPTION HANDLING
==================================================*/

namespace GachaModule {

    // ============================================================
    /*==================================================
      MATERI: FILE HANDLING, EXCEPTION HANDLING - MEMBACA DATA PELUANG ITEM GACHA DARI CSV.
    ==================================================*/
    // ============================================================
    std::vector<GachaReward> loadPool() {
        std::vector<GachaReward> pool;
        std::ifstream inFile(FileConfig::GACHA_POOL_FILE);

        if (!inFile.is_open()) {
            return pool; // LEVEL 3: Jika file hilang maka dikembalikan isi yang kosong.
        }

        std::string line;
        bool isHeader = true;

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;
            if (isHeader) { isHeader = false; continue; }

            /*==================================================
              MATERI: EXCEPTION HANDLING - MENANGANI FORMAT BARIS YANG MUNGKIN TIDAK SESUAI SKEMA.
            ==================================================*/
            try {
                std::stringstream ss(line);
                std::string idStr, name, type, rarity, weightStr;

                std::getline(ss, idStr, ',');
                std::getline(ss, name, ',');
                std::getline(ss, type, ',');
                std::getline(ss, rarity, ',');
                std::getline(ss, weightStr, ',');

                GachaReward reward;
                reward.id = std::stoi(idStr);
                reward.name = name;
                reward.type = type;
                reward.rarity = rarity;
                reward.weight = std::stoi(weightStr);
                pool.push_back(reward);
            } catch (...) {
                continue; // LEVEL 3: Melewati baris yang tidak valid.
            }
        }

        inFile.close();
        return pool;
    }

    // ============================================================
    /*==================================================
      MATERI: LAMBDA, STL VECTOR, ITERATOR - EKSEKUSI LOGIKA PROBABILITAS BERBOBOT SECARA ACAK.
    ==================================================*/
    // ============================================================
    GachaReward rollOnce(const std::vector<GachaReward>& pool) {
        /*==================================================
          MATERI: LAMBDA EXPRESSION - MENGALKULASI BATAS ATAS AKUMULASI BOBOT (WEIGHT).
        ==================================================*/
        int totalWeight = 0;
        std::for_each(pool.begin(), pool.end(), [&](const GachaReward& r) {
            if (r.weight > 0) totalWeight += r.weight;
        });

        // Mengamankan kode dari error membagi bilangan dengan nilai nol.
        if (totalWeight <= 0) {
            return pool.front();
        }

        int roll = std::rand() % totalWeight;
        int cumulative = 0;

        /*==================================================
          MATERI: ITERATOR - MENYUSURI SATU PER SATU KEMUNGKINAN PELUANG TERHADAP ANGKA ACAK.
        ==================================================*/
        for (auto it = pool.begin(); it != pool.end(); ++it) {
            if (it->weight <= 0) continue;
            cumulative += it->weight;
            if (roll < cumulative) {
                return *it;
            }
        }

        // Pilihan pengaman jika secara logika eksekusi terlewat, kemungkinan tidak akan terjadi.
        return pool.back();
    }

    // ============================================================
    /*==================================================
      MATERI: FILE HANDLING - KONVERSI WAKTU SAAT INI MENJADI BARIS STRING FORMAT TANGGAL.
    ==================================================*/
    // ============================================================
    std::string getCurrentDate() {
        std::time_t now = std::time(nullptr);
        std::tm* tm = std::localtime(&now);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
        return std::string(buf);
    }

    // ============================================================
    /*==================================================
      MATERI: FILE HANDLING, EXCEPTION HANDLING - PENCATATAN PENARIKAN SUKSES KE CATATAN RIWAYAT (CSV).
    ==================================================*/
    // ============================================================
    void saveHistory(const std::string& playerName, const GachaReward& reward) {
        // Mengetes keberadaan file guna menambahkan judul kolom (header) saat pertama kali dibuat.
        bool fileExists = false;
        {
            std::ifstream check(FileConfig::GACHA_HISTORY_FILE);
            if (check.is_open() && check.peek() != std::ifstream::traits_type::eof()) {
                fileExists = true;
            }
        }

        std::ofstream outFile(FileConfig::GACHA_HISTORY_FILE, std::ios::app);
        if (!outFile.is_open()) {
            throw FileException("Unable to open " + FileConfig::GACHA_HISTORY_FILE + " for writing.");
        }

        if (!fileExists) {
            outFile << "date,playerName,reward,rarity\n";
        }

        outFile << getCurrentDate() << ","
                << playerName << ","
                << reward.name << ","
                << reward.rarity << "\n";

        outFile.close();
    }

    // ============================================================
    /*==================================================
      MATERI: FILE HANDLING, EXCEPTION HANDLING, STL VECTOR - MEMBACA DATA HISTORY PER PEMAIN.
    ==================================================*/
    // ============================================================
    std::vector<GachaRecord> loadHistory(const std::string& playerName) {
        std::vector<GachaRecord> records;
        std::ifstream inFile(FileConfig::GACHA_HISTORY_FILE);

        if (!inFile.is_open()) {
            return records;
        }

        std::string line;
        bool isHeader = true;

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;
            if (isHeader) { isHeader = false; continue; }

            try {
                std::stringstream ss(line);
                std::string date, name, reward, rarity;

                std::getline(ss, date, ',');
                std::getline(ss, name, ',');
                std::getline(ss, reward, ',');
                std::getline(ss, rarity, ',');

                if (name == playerName) {
                    records.push_back({date, name, reward, rarity});
                }
            } catch (...) {
                continue;
            }
        }

        inFile.close();
        return records;
    }

    // ============================================================
    // Mengonversi struktur GachaReward agar kompatibel masuk ke struct Item pada inventory.
    // ============================================================
    Item rewardToItem(const GachaReward& reward) {
        Item item;
        item.id = reward.id;
        item.name = reward.name;
        item.type = reward.type;
        item.price = 0;
        item.effect = "";
        item.quantity = 1;
        item.stock = -1;
        return item;
    }

    // ============================================================
    // Penarikan acak tipe satu tiket (Single Pull).
    // ============================================================
    void singlePull(const std::vector<GachaReward>& pool, std::vector<Item>& inventory,
                    const std::string& playerName) {
        /*==================================================
          MATERI: FIND, LAMBDA - VERIFIKASI PERSEDIAAN GACHA TICKET PEMAIN PADA INVENTORY.
        ==================================================*/
        int ticketCount = InventoryModule::countItem(inventory, 5); // ID 5 = Gacha Ticket

        if (ticketCount < 1) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Not enough tickets. You need 1 Gacha Ticket." << GameColor::RESET << "\n";
            return;
        }

        // Mengurangi jumlah tiket pemain sebanyak satu keping.
        InventoryModule::removeItem(inventory, 5);

        // Menjalankan eksekusi penarikan peluang item.
        GachaReward reward = rollOnce(pool);
        std::string finalRewardName = reward.name;

        // Kasus pengecualian jika item yang didapat adalah bitcoin yang dihitung secara desimal.
        if (reward.type == "Crypto" && reward.name == "Bitcoin") {
            double btcDrop = 0.01 + (std::rand() % 10) / 100.0;
            
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << btcDrop;
            finalRewardName = "Bitcoin (" + stream.str() + " BTC)";
            
            try {
                Wallet w = TradingModule::loadWallet(playerName);
                w.btc += btcDrop;
                TradingModule::saveWallet(w);
            } catch (const std::exception& e) {
                std::cout << "\n  " << GameColor::TXT_ERROR << "[Wallet Error] " << e.what() << GameColor::RESET << "\n";
            }
        } else {
            /*==================================================
              MATERI: STL VECTOR - MENYISIPKAN REKAMAN ITEM BARU MASUK KE KANTONG (INVENTORY).
            ==================================================*/
            Item rewardItem = rewardToItem(reward);
            InventoryModule::addItem(inventory, rewardItem);
        }

        // Menampilkan pengumuman keberhasilan mendapat item.
        std::cout << "\n  ======================================================\n";
        std::cout << GameColor::MENU_TITLE << "                     GACHA RESULT                       \n" << GameColor::RESET;
        std::cout << "  ======================================================\n\n";
        std::cout << "  " << GameColor::TXT_SUCCESS << "Congratulations!" << GameColor::RESET << "\n\n";
        std::cout << "  Reward  : " << finalRewardName << "\n";
        std::cout << "  Rarity  : " << reward.rarity << "\n";

        // Menambahkan entri keberhasilan penarikan pada file riwayat (history).
        GachaReward historyReward = reward;
        historyReward.name = finalRewardName;
        
        try {
            saveHistory(playerName, historyReward);
        } catch (const std::exception& e) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Warning: " << e.what() << GameColor::RESET << "\n";
        }

        /*==================================================
          MATERI: FILE HANDLING - MEMASTIKAN PENYIMPANAN FILE INVENTORY SETELAHNYA AGAR DATA AMAN.
        ==================================================*/
        try {
            InventoryModule::saveInventory(playerName, inventory);
        } catch (const std::exception& e) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Warning: " << e.what() << GameColor::RESET << "\n";
        }
    }

    // ============================================================
    // Penarikan multi sekaligus memakai 5 tiket gacha berturut-turut.
    // ============================================================
    void multiPull(const std::vector<GachaReward>& pool, std::vector<Item>& inventory,
                   const std::string& playerName) {
        int ticketCount = InventoryModule::countItem(inventory, 5);

        if (ticketCount < 5) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Not enough tickets. You need 5 Gacha Tickets." << GameColor::RESET << "\n";
            std::cout << "  Current tickets: " << ticketCount << "\n";
            return;
        }

        // Mengurangi sebanyak lima keping tiket sekaligus.
        for (int i = 0; i < 5; ++i) {
            InventoryModule::removeItem(inventory, 5);
        }

        std::cout << "\n  ======================================================\n";
        std::cout << GameColor::MENU_TITLE << "                  MULTI PULL RESULT (x5)                \n" << GameColor::RESET;
        std::cout << "  ======================================================\n\n";

        // Iterasi mendapatkan sebanyak lima buah peluang secara acak.
        for (int i = 0; i < 5; ++i) {
            GachaReward reward = rollOnce(pool);
            std::string finalRewardName = reward.name;

            if (reward.type == "Crypto" && reward.name == "Bitcoin") {
                double btcDrop = 0.01 + (std::rand() % 10) / 100.0;
                
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << btcDrop;
                finalRewardName = "Bitcoin (" + stream.str() + " BTC)";
                
                try {
                    Wallet w = TradingModule::loadWallet(playerName);
                    w.btc += btcDrop;
                    TradingModule::saveWallet(w);
                } catch (...) {}
            } else {
                Item rewardItem = rewardToItem(reward);
                InventoryModule::addItem(inventory, rewardItem);
            }

            std::cout << "  " << (i + 1) << ". " << std::left << std::setw(24)
                      << finalRewardName << "(" << reward.rarity << ")\n";

            // Menulis jejak untuk item yang baru saja diperoleh dari sistem ini.
            GachaReward historyReward = reward;
            historyReward.name = finalRewardName;
            try {
                saveHistory(playerName, historyReward);
            } catch (...) {}
        }

        // Perbarui data inventory secara global setelah kelima peluang dihitung.
        try {
            InventoryModule::saveInventory(playerName, inventory);
        } catch (const std::exception& e) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Warning: " << e.what() << GameColor::RESET << "\n";
        }
    }

    // ============================================================
    /*==================================================
      MATERI: STL VECTOR, ITERATOR - TAMPILAN UNTUK MEMBACA RIWAYAT PEROLEHAN GACHA.
    ==================================================*/
    // ============================================================
    void viewHistory(const std::string& playerName) {
        std::vector<GachaRecord> records = loadHistory(playerName);

        std::cout << "\n  ======================================================\n";
        std::cout << GameColor::MENU_TITLE << "                    GACHA HISTORY                       \n" << GameColor::RESET;
        std::cout << "  ======================================================\n\n";

        if (records.empty()) {
            std::cout << "  No gacha history found.\n";
            return;
        }

        std::cout << "  " << std::left << std::setw(14) << "Date"
                  << std::setw(24) << "Reward"
                  << std::setw(12) << "Rarity" << "\n";
        std::cout << "  --------------------------------------------------\n";

        /*==================================================
          MATERI: ITERATOR - MENGEKSTRAKSI DAN MENCETAK MASING-MASING REKAMAN PEMAIN KE LAYAR KONSOL.
        ==================================================*/
        for (auto it = records.begin(); it != records.end(); ++it) {
            std::cout << "  " << std::left << std::setw(14) << it->date
                      << std::setw(24) << it->reward
                      << std::setw(12) << it->rarity << "\n";
        }

        std::cout << "\n  Total Pulls: " << records.size() << "\n";
    }

    // ============================================================
    /*==================================================
      MATERI: COUNT, LAMBDA EXPRESSION, STL VECTOR - AGREGASI DATA STATISTIK HASIL UNDIAN PEMAIN.
    ==================================================*/
    // ============================================================
    void viewStatistics(const std::string& playerName) {
        std::vector<GachaRecord> records = loadHistory(playerName);

        std::cout << "\n  ======================================================\n";
        std::cout << GameColor::MENU_TITLE << "                    GACHA STATS                         \n" << GameColor::RESET;
        std::cout << "  ======================================================\n\n";

        if (records.empty()) {
            std::cout << "  No gacha data available.\n";
            return;
        }

        // Fungsi anonim dalam blok untuk kemudahan iterasi hitung yang adaptif walau ada kombinasi kapital atau tidak pada kata pencarian.
        auto countRarity = [&](const std::string& target) {
            std::string lowerTarget = target;
            std::transform(lowerTarget.begin(), lowerTarget.end(), lowerTarget.begin(), ::tolower);
            
            return std::count_if(records.begin(), records.end(), [&](const GachaRecord& r) {
                std::string lowerRarity = r.rarity;
                std::transform(lowerRarity.begin(), lowerRarity.end(), lowerRarity.begin(), ::tolower);
                return lowerRarity == lowerTarget;
            });
        };

        int commonCount = countRarity("common");
        int rareCount = countRarity("rare");
        int epicCount = countRarity("epic");
        int legendaryCount = countRarity("legendary");

        std::cout << "  Total Pulls : " << records.size() << "\n\n";
        std::cout << "  Common      : " << commonCount << "\n";
        std::cout << "  Rare        : " << rareCount << "\n";
        std::cout << "  Epic        : " << epicCount << "\n";
        std::cout << "  Legendary   : " << legendaryCount << "\n";
    }

    // ============================================================
    /*==================================================
      MATERI: STL VECTOR, SORT, LAMBDA, ITERATOR - MEMBUKA TABEL TINGKAT KESULITAN GACHA.
    ==================================================*/
    // ============================================================
    void viewRewardPool(const std::vector<GachaReward>& pool) {
        std::cout << "\n  ======================================================\n";
        std::cout << GameColor::MENU_TITLE << "                    REWARD POOL                         \n" << GameColor::RESET;
        std::cout << "  ======================================================\n\n";

        if (pool.empty()) {
            std::cout << "  No rewards available.\n";
            return;
        }

        /*==================================================
          MATERI: SORT, LAMBDA - URUTKAN TABEL DARI YANG TERBANYAK MUNCUL KE YANG LANGKA AGAR LEBIH MENARIK DIPANDANG.
        ==================================================*/
        std::vector<GachaReward> sorted = pool;
        std::sort(sorted.begin(), sorted.end(), [](const GachaReward& a, const GachaReward& b) {
            return a.weight > b.weight;
        });

        std::cout << "  " << std::left << std::setw(18) << "Item"
                  << std::setw(14) << "Rarity"
                  << std::setw(8) << "Weight" << "\n";
        std::cout << "  ----------------------------------------\n";

        for (const auto& r : sorted) {
            std::cout << "  " << std::left << std::setw(18) << r.name
                      << std::setw(14) << r.rarity
                      << std::setw(8) << r.weight << "\n";
        }
        std::cout << "\n";
    }

    // ============================================================
    /*==================================================
      MATERI: FUNCTION, EXCEPTION HANDLING - KONFIGURASI ANTAR MUKA CLI MENU GACHA YANG DAPAT DISAJIKAN KEPADA PEMAIN.
    ==================================================*/
    // ============================================================
    void runGachaMenu(std::vector<Item>& inventory, const std::string& playerName) {
        /*==================================================
          MATERI: EXCEPTION HANDLING - MEMASTIKAN TABEL PROBABILITAS SUKSES DIMUAT SECARA AMAN ATAU MEMBERITAHU KE LAYAR BILA EROR MEMUAT FILE TSB TERJADI SECARA GRACEFULLY.
        ==================================================*/
        std::vector<GachaReward> pool;
        try {
            pool = loadPool();
        } catch (const std::exception& e) {
            std::cout << "\n  " << GameColor::TXT_ERROR << "Error loading gacha pool: " << e.what() << GameColor::RESET << "\n";
            std::cout << "  Press any key to return...";
            _getch();
            return;
        }

        if (pool.empty()) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Gacha pool is empty. No rewards available." << GameColor::RESET << "\n";
            std::cout << "  Press any key to return...";
            _getch();
            return;
        }

        bool inMenu = true;
        while (inMenu) {
            system("cls");

            /*==================================================
              MATERI: FIND, LAMBDA - MENDETEKSI ITEM NOMOR ID=5 (TIKET GACHA).
            ==================================================*/
            int tickets = InventoryModule::countItem(inventory, 5);

            std::cout << "\n  ======================================================\n";
            std::cout << GameColor::MENU_TITLE << "                        GACHA                           \n" << GameColor::RESET;
            std::cout << "  ======================================================\n\n";
            std::cout << "  Tickets Available: " << tickets << "\n\n";
            std::cout << "    [1] Single Pull (1 Ticket)\n";
            std::cout << "    [2] Multi Pull  (5 Tickets)\n";
            std::cout << "    [3] View Gacha History\n";
            std::cout << "    [4] View Gacha Statistics\n";
            std::cout << "    [5] View Reward Pool\n";
            std::cout << "    [0] Back\n\n";
            std::cout << "    Enter your choice: ";

            char choice = _getch();
            std::cout << choice << "\n";

            system("cls");

            switch (choice) {
                case '1':
                    singlePull(pool, inventory, playerName);
                    break;
                case '2':
                    multiPull(pool, inventory, playerName);
                    break;
                case '3':
                    viewHistory(playerName);
                    break;
                case '4':
                    viewStatistics(playerName);
                    break;
                case '5':
                    viewRewardPool(pool);
                    break;
                case '0':
                    inMenu = false;
                    continue;
                default:
                    std::cout << "    " << GameColor::TXT_WARNING << "Invalid choice. Please try again." << GameColor::RESET << "\n";
                    break;
            }

            if (inMenu) {
                std::cout << "\n  Press any key to return to Gacha Menu...";
                _getch();
            }
        }
    }

} // namespace GachaModule