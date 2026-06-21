#include "gacha.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

// Materi: Custom Exception
class GachaException : public std::runtime_error {
public:
    GachaException(const std::string& msg) : std::runtime_error("Gacha Error: " + msg) {}
};

namespace GachaModule {

    const std::string FILE_POOL   = "data/gacha_pool.csv";
    const std::string FILE_WALLET = "data/wallet.csv";
    const std::string FILE_INV    = "data/inventory.csv";

    // 1. Baca Data Pool
    std::vector<Item> loadGachaPool() {
        std::vector<Item> pool;
        std::ifstream file(FILE_POOL);
        if (!file.is_open()) throw GachaException("File gacha_pool.csv tidak ditemukan!");

        std::string line;
        std::getline(file, line); // Skip header

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string id, name, type, rarity, valueStr;

            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, type, ',');
            std::getline(ss, rarity, ',');
            std::getline(ss, valueStr, ',');

            Item item;
            item.id = std::stoi(id);
            item.name = name;
            item.type = type;
            item.quantity = std::stoi(valueStr);
            pool.push_back(item);
        }
        return pool;
    }

    // 2. Helper Update Koin Mandiri
    void updateCoinCSV(const std::string& playerName, int amountChange) {
        std::vector<Wallet> wallets;
        bool playerFound = false;

        std::ifstream fileIn(FILE_WALLET);
        if (fileIn.is_open()) {
            std::string line;
            std::getline(fileIn, line);

            while (std::getline(fileIn, line)) {
                if (line.empty()) continue;
                std::stringstream ss(line);
                std::string name, coin, btc, eth;

                std::getline(ss, name, ',');
                std::getline(ss, coin, ',');
                std::getline(ss, btc, ',');
                std::getline(ss, eth, ',');

                Wallet w = {name, std::stoi(coin), std::stod(btc), std::stod(eth)};
                if (name == playerName) {
                    if (w.coin + amountChange < 0) throw GachaException("Saldo koin tidak cukup (Butuh 150 Coin)!");
                    w.coin += amountChange;
                    playerFound = true;
                }
                wallets.push_back(w);
            }
            fileIn.close();
        }

        if (!playerFound && amountChange < 0) {
            throw GachaException("Data player tidak ditemukan di wallet.csv! Mainkan game dulu.");
        }

        std::ofstream fileOut(FILE_WALLET);
        fileOut << "playerName,coin,btc,eth\n";
        for (const auto& w : wallets) {
            fileOut << w.playerName << "," << w.coin << "," << w.btc << "," << w.eth << "\n";
        }
    }

    // 3. Helper Simpan/Kurangi Item Mandiri
    void updateInventoryCSV(const std::string& playerName, const Item& item, bool isDeductingTicket = false) {
        std::vector<std::string> allRecords;
        bool itemFound = false;

        std::ifstream fileIn(FILE_INV);
        if (fileIn.is_open()) {
            std::string line;
            std::getline(fileIn, line); 
            while (std::getline(fileIn, line)) {
                if (line.empty()) continue;
                std::stringstream ss(line);
                std::string pName, idStr, name, type, qtyStr;
                
                std::getline(ss, pName, ',');
                std::getline(ss, idStr, ',');
                std::getline(ss, name, ',');
                std::getline(ss, type, ',');
                std::getline(ss, qtyStr, ',');

                // Jika mencari tiket untuk dikurangi
                if (isDeductingTicket && pName == playerName && name == "Gacha Ticket") {
                    int qty = std::stoi(qtyStr);
                    if (qty > 0) {
                        itemFound = true;
                        qty--; 
                        if (qty > 0) {
                            line = pName + "," + idStr + "," + name + "," + type + "," + std::to_string(qty);
                            allRecords.push_back(line);
                        }
                        continue; 
                    }
                } 
                // Jika menyimpan hasil gacha baru
                else if (!isDeductingTicket && pName == playerName && std::stoi(idStr) == item.id) {
                    int newQty = std::stoi(qtyStr) + item.quantity;
                    line = pName + "," + idStr + "," + name + "," + type + "," + std::to_string(newQty);
                    itemFound = true;
                }
                allRecords.push_back(line);
            }
            fileIn.close();
        }

        if (isDeductingTicket && !itemFound) {
            throw GachaException("Kamu tidak punya Gacha Ticket di inventory!");
        }

        std::ofstream fileOut(FILE_INV);
        fileOut << "playerName,itemId,itemName,type,quantity\n";
        for (const auto& record : allRecords) {
            fileOut << record << "\n";
        }
        
        if (!isDeductingTicket && !itemFound) {
            fileOut << playerName << "," << item.id << "," << item.name << "," << item.type << "," << item.quantity << "\n";
        }
    }

    // 4. Logika Core Gacha
    Item processRoll(const std::string& playerName) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        std::vector<Item> fullPool = loadGachaPool();

        Item prize = fullPool[std::rand() % fullPool.size()];

        if (prize.type == "COIN") {
            updateCoinCSV(playerName, prize.quantity);
            std::cout << "\n[JACKPOT] Kamu mendapat koin sebesar " << prize.quantity << "!\n";
        } else {
            updateInventoryCSV(playerName, prize, false);
            std::cout << "\n[ITEM] Kamu mendapat: " << prize.name << " x" << prize.quantity << "\n";
        }

        return prize;
    }

    // ==========================================
    // FUNGSI EXPOSED KE MAIN MENU
    // ==========================================
    Item rollGacha(const std::string& playerName, int cost) {
        updateCoinCSV(playerName, -cost); 
        return processRoll(playerName);
    }

    Item rollWithTicket(const std::string& playerName) {
        updateInventoryCSV(playerName, {0, "Gacha Ticket", "", 0, 0}, true); 
        return processRoll(playerName);
    }

    void displayGachaPool() {
        try {
            std::vector<Item> pool = loadGachaPool();
            std::cout << "\n=== DAFTAR HADIAH GACHA ===\n";
            for (const auto& item : pool) {
                std::cout << "- " << item.name << " (" << item.type << ")\n";
            }
            std::cout << "===========================\n";
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }

} // namespace GachaModule