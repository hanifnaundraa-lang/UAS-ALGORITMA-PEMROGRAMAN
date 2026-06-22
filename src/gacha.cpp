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

// Material: Function, Namespace, STL Vector, Iterator, Sort, Find, Count, Lambda, File Handling, Exception Handling

namespace GachaModule {

    // ============================================================
    // Material: File Handling, Exception Handling — Load reward pool from CSV
    // ============================================================
    std::vector<GachaReward> loadPool() {
        std::vector<GachaReward> pool;
        std::ifstream inFile(FileConfig::GACHA_POOL_FILE);

        if (!inFile.is_open()) {
            return pool; // Return empty pool if file missing
        }

        std::string line;
        bool isHeader = true;

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;
            if (isHeader) { isHeader = false; continue; }

            // Material: Exception Handling — skip corrupted rows
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
                continue; // Skip invalid rows
            }
        }

        inFile.close();
        return pool;
    }

    // ============================================================
    // Material: Lambda, STL Vector, Iterator — Weighted random roll
    // ============================================================
    GachaReward rollOnce(const std::vector<GachaReward>& pool) {
        // Material: Lambda Expression — calculate total weight
        int totalWeight = 0;
        std::for_each(pool.begin(), pool.end(), [&](const GachaReward& r) {
            if (r.weight > 0) totalWeight += r.weight;
        });

        // Prevent division by zero if all weights are 0 or negative
        if (totalWeight <= 0) {
            return pool.front();
        }

        int roll = std::rand() % totalWeight;
        int cumulative = 0;

        // Material: Iterator — walk through pool with cumulative weight
        for (auto it = pool.begin(); it != pool.end(); ++it) {
            if (it->weight <= 0) continue;
            cumulative += it->weight;
            if (roll < cumulative) {
                return *it;
            }
        }

        // Fallback (should never happen)
        return pool.back();
    }

    // ============================================================
    // Material: File Handling — Get current date as string
    // ============================================================
    std::string getCurrentDate() {
        std::time_t now = std::time(nullptr);
        std::tm* tm = std::localtime(&now);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
        return std::string(buf);
    }

    // ============================================================
    // Material: File Handling, Exception Handling — Save history record
    // ============================================================
    void saveHistory(const std::string& playerName, const GachaReward& reward) {
        // Check if file exists, if not create with header
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
    // Material: File Handling, Exception Handling, STL Vector — Load history
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
    // Convert GachaReward to Item for InventoryModule integration
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
    // Feature: Single Pull (1 Ticket)
    // ============================================================
    void singlePull(const std::vector<GachaReward>& pool, std::vector<Item>& inventory,
                    const std::string& playerName) {
        // Material: Find & Lambda — check ticket count
        int ticketCount = InventoryModule::countItem(inventory, 5); // ID 5 = Gacha Ticket

        if (ticketCount < 1) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Not enough tickets. You need 1 Gacha Ticket." << GameColor::RESET << "\n";
            return;
        }

        // Deduct 1 ticket
        InventoryModule::removeItem(inventory, 5);

        // Roll
        GachaReward reward = rollOnce(pool);
        std::string finalRewardName = reward.name;

        // Custom intercept for Bitcoin
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
            // Material: STL Vector — add reward to inventory
            Item rewardItem = rewardToItem(reward);
            InventoryModule::addItem(inventory, rewardItem);
        }

        // Display result
        std::cout << "\n  ======================================================\n";
        std::cout << GameColor::MENU_TITLE << "                     GACHA RESULT                       \n" << GameColor::RESET;
        std::cout << "  ======================================================\n\n";
        std::cout << "  " << GameColor::TXT_SUCCESS << "Congratulations!" << GameColor::RESET << "\n\n";
        std::cout << "  Reward  : " << finalRewardName << "\n";
        std::cout << "  Rarity  : " << reward.rarity << "\n";

        // Save history with exact formatted name
        GachaReward historyReward = reward;
        historyReward.name = finalRewardName;
        
        try {
            saveHistory(playerName, historyReward);
        } catch (const std::exception& e) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Warning: " << e.what() << GameColor::RESET << "\n";
        }

        // Material: File Handling — save inventory immediately
        try {
            InventoryModule::saveInventory(playerName, inventory);
        } catch (const std::exception& e) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Warning: " << e.what() << GameColor::RESET << "\n";
        }
    }

    // ============================================================
    // Feature: Multi Pull (5 Tickets)
    // ============================================================
    void multiPull(const std::vector<GachaReward>& pool, std::vector<Item>& inventory,
                   const std::string& playerName) {
        int ticketCount = InventoryModule::countItem(inventory, 5);

        if (ticketCount < 5) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Not enough tickets. You need 5 Gacha Tickets." << GameColor::RESET << "\n";
            std::cout << "  Current tickets: " << ticketCount << "\n";
            return;
        }

        // Deduct 5 tickets
        for (int i = 0; i < 5; ++i) {
            InventoryModule::removeItem(inventory, 5);
        }

        std::cout << "\n  ======================================================\n";
        std::cout << GameColor::MENU_TITLE << "                  MULTI PULL RESULT (x5)                \n" << GameColor::RESET;
        std::cout << "  ======================================================\n\n";

        // Roll 5 times
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

            // Log history
            GachaReward historyReward = reward;
            historyReward.name = finalRewardName;
            try {
                saveHistory(playerName, historyReward);
            } catch (...) {}
        }

        // Save inventory once after all pulls
        try {
            InventoryModule::saveInventory(playerName, inventory);
        } catch (const std::exception& e) {
            std::cout << "\n  " << GameColor::TXT_WARNING << "Warning: " << e.what() << GameColor::RESET << "\n";
        }
    }

    // ============================================================
    // Feature: View Gacha History
    // Material: STL Vector, Iterator
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

        // Material: Iterator — display all records
        for (auto it = records.begin(); it != records.end(); ++it) {
            std::cout << "  " << std::left << std::setw(14) << it->date
                      << std::setw(24) << it->reward
                      << std::setw(12) << it->rarity << "\n";
        }

        std::cout << "\n  Total Pulls: " << records.size() << "\n";
    }

    // ============================================================
    // Feature: Gacha Statistics
    // Material: Count, Lambda Expression, STL Vector
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

        // Helper lambda for case-insensitive counting
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
    // Feature: View Reward Pool
    // Material: STL Vector, Sort, Lambda, Iterator
    // ============================================================
    void viewRewardPool(const std::vector<GachaReward>& pool) {
        std::cout << "\n  ======================================================\n";
        std::cout << GameColor::MENU_TITLE << "                    REWARD POOL                         \n" << GameColor::RESET;
        std::cout << "  ======================================================\n\n";

        if (pool.empty()) {
            std::cout << "  No rewards available.\n";
            return;
        }

        // Material: Sort & Lambda — sort by weight descending for display
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
    // Main Gacha Menu Runner
    // Material: Function, Exception Handling
    // ============================================================
    void runGachaMenu(std::vector<Item>& inventory, const std::string& playerName) {
        // Material: Exception Handling — load pool safely
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

            // Material: Find & Lambda — get current ticket count
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