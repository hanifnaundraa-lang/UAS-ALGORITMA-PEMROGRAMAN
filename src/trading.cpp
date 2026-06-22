#include "trading.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <conio.h>

// Material: Namespace, Function, File Handling, Exception Handling
// Trading module — Simulated BTC market using in-game coin
namespace TradingModule {

    // ============================================================
    // Internal helpers (not declared in header)
    // ============================================================

    // Material: Inline Function — separator line
    static inline void printSeparator() {
        std::cout << "  ";
        for (int i = 0; i < 44; ++i) std::cout << "=";
        std::cout << "\n";
    }

    // Get today's date as YYYY-MM-DD string
    static std::string getTodayDate() {
        auto now = std::chrono::system_clock::now();
        time_t t = std::chrono::system_clock::to_time_t(now);
        tm* local = localtime(&t);

        std::stringstream ss;
        ss << std::put_time(local, "%Y-%m-%d");
        return ss.str();
    }

    // Get current timestamp as YYYY-MM-DD HH:MM:SS string
    static std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        time_t t = std::chrono::system_clock::to_time_t(now);
        tm* local = localtime(&t);

        std::stringstream ss;
        ss << std::put_time(local, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // ============================================================
    // Material: File Handling & Exception Handling — Wallet persistence
    // ============================================================

    Wallet loadWallet(const std::string& playerName) {
        Wallet wallet;
        wallet.playerName = playerName;
        wallet.coin = 0;
        wallet.btc = 0.0;
        wallet.eth = 0.0; // not used, kept for struct compatibility

        std::ifstream file(FileConfig::WALLET_FILE);
        if (!file.is_open()) {
            // File doesn't exist yet — return default wallet
            return wallet;
        }

        std::string line;
        // Skip header line
        std::getline(file, line);

        // Material: File Handling — CSV parsing
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::istringstream iss(line);
            std::string name, coinStr, btcStr;

            std::getline(iss, name, ',');
            std::getline(iss, coinStr, ',');
            std::getline(iss, btcStr, ',');

            if (name == playerName) {
                // Material: Exception Handling — safe parsing
                try {
                    wallet.coin = std::stoi(coinStr);
                    wallet.btc = std::stod(btcStr);
                } catch (const std::exception&) {
                    // Invalid data, use defaults
                    wallet.coin = 0;
                    wallet.btc = 0.0;
                }
                file.close();
                return wallet;
            }
        }

        file.close();
        // Player not found — return default wallet
        return wallet;
    }

    void saveWallet(const Wallet& wallet) {
        // Read all existing wallets
        // Material: STL Vector — store all wallet entries
        std::vector<Wallet> allWallets;
        bool playerFound = false;

        std::ifstream inFile(FileConfig::WALLET_FILE);
        if (inFile.is_open()) {
            std::string line;
            // Skip header
            std::getline(inFile, line);

            while (std::getline(inFile, line)) {
                if (line.empty()) continue;

                std::istringstream iss(line);
                std::string name, coinStr, btcStr;

                std::getline(iss, name, ',');
                std::getline(iss, coinStr, ',');
                std::getline(iss, btcStr, ',');

                Wallet w;
                w.playerName = name;
                try {
                    w.coin = std::stoi(coinStr);
                    w.btc = std::stod(btcStr);
                } catch (...) {
                    w.coin = 0;
                    w.btc = 0.0;
                }
                w.eth = 0.0;

                // Update if this is the current player
                if (name == wallet.playerName) {
                    w = wallet;
                    playerFound = true;
                }

                allWallets.push_back(w);
            }
            inFile.close();
        }

        // If player wasn't found, add them
        if (!playerFound) {
            allWallets.push_back(wallet);
        }

        // Write all wallets back
        // Material: File Handling — write CSV
        std::ofstream outFile(FileConfig::WALLET_FILE);
        if (!outFile.is_open()) {
            throw FileException("Cannot write to " + FileConfig::WALLET_FILE);
        }

        outFile << "playerName,coin,btc\n";
        for (const auto& w : allWallets) {
            outFile << w.playerName << ","
                    << w.coin << ","
                    << std::fixed << std::setprecision(4) << w.btc << "\n";
        }
        outFile.close();
    }

    // ============================================================
    // Material: File Handling — BTC Price System
    // ============================================================

    int getCurrentBTCPrice() {
        std::string today = getTodayDate();
        std::string lastDate;
        std::string lastPriceStr;

        // Read market history to find today's price
        std::ifstream inFile(FileConfig::MARKET_HISTORY_FILE);
        if (inFile.is_open()) {
            std::string line;
            // Skip header
            std::getline(inFile, line);

            while (std::getline(inFile, line)) {
                if (line.empty()) continue;

                std::istringstream iss(line);
                std::getline(iss, lastDate, ',');
                std::getline(iss, lastPriceStr, ',');
            }
            inFile.close();
        }

        // If today's price already exists, return it
        if (lastDate == today && !lastPriceStr.empty()) {
            try {
                return std::stoi(lastPriceStr);
            } catch (...) {
                // Fall through to generate new price
            }
        }

        // Generate new random BTC price: 30000 - 70000
        int newPrice = (rand() % 40001) + 30000;

        // Append to market history file
        std::ofstream outFile(FileConfig::MARKET_HISTORY_FILE, std::ios::app);
        if (outFile.is_open()) {
            outFile << today << "," << newPrice << "\n";
            outFile.close();
        }

        return newPrice;
    }

    // ============================================================
    // Secret Function — Force update today's BTC price
    // ============================================================
    void forceUpdateBTCPrice() {
        std::string today = getTodayDate();
        int newPrice = (rand() % 40001) + 30000;

        std::vector<std::string> lines;
        std::ifstream inFile(FileConfig::MARKET_HISTORY_FILE);
        if (inFile.is_open()) {
            std::string line;
            while (std::getline(inFile, line)) {
                lines.push_back(line);
            }
            inFile.close();
        }

        bool updated = false;
        if (!lines.empty()) {
            std::string lastLine = lines.back();
            std::istringstream iss(lastLine);
            std::string lastDate;
            std::getline(iss, lastDate, ',');
            
            if (lastDate == today) {
                lines.back() = today + "," + std::to_string(newPrice);
                updated = true;
            }
        }

        if (!updated) {
            lines.push_back(today + "," + std::to_string(newPrice));
        }

        std::ofstream outFile(FileConfig::MARKET_HISTORY_FILE);
        if (outFile.is_open()) {
            for (const auto& l : lines) {
                outFile << l << "\n";
            }
            outFile.close();
        }
    }

    // ============================================================
    // Material: Function — Display wallet info
    // ============================================================

    void displayWallet(const Wallet& wallet) {
        printSeparator();
        std::cout << GameColor::MENU_TITLE << "         WALLET - " << wallet.playerName << GameColor::RESET << "\n";
        printSeparator();
        std::cout << "    Coin : " << wallet.coin << "\n";
        std::cout << "    BTC  : " << std::fixed << std::setprecision(4)
                  << wallet.btc << "\n";
        printSeparator();
    }

    // ============================================================
    // Material: Function — Display current market price
    // ============================================================

    void displayMarket() {
        int price = getCurrentBTCPrice();
        std::string today = getTodayDate();

        printSeparator();
        std::cout << GameColor::MENU_TITLE << "         BTC MARKET - " << today << GameColor::RESET << "\n";
        printSeparator();
        std::cout << "    Current BTC Price : " << price << " coin\n";
        printSeparator();
    }

    // ============================================================
    // Material: Function, Exception Handling — Buy BTC
    // ============================================================

    void buyBTC(Wallet& wallet) {
        int btcPrice = getCurrentBTCPrice();

        std::cout << "\n    Current BTC Price: " << btcPrice << " coin\n";
        std::cout << "    Your Coin: " << wallet.coin << "\n\n";

        double amount = 0.0;
        std::cout << "    How much BTC to buy: ";
        std::cin >> amount;

        // Material: Exception Handling — input validation
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Invalid input." << GameColor::RESET << "\n";
            return;
        }

        if (amount <= 0) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Amount must be greater than 0." << GameColor::RESET << "\n";
            return;
        }

        int totalCost = static_cast<int>(amount * btcPrice);

        if (totalCost <= 0) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Amount too small." << GameColor::RESET << "\n";
            return;
        }

        // Validate coin balance
        if (wallet.coin < totalCost) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Not enough coin!" << GameColor::RESET
                      << " Need " << totalCost
                      << ", you have " << wallet.coin << ".\n";
            return;
        }

        // Execute transaction
        wallet.coin -= totalCost;
        wallet.btc += amount;

        // Save wallet
        saveWallet(wallet);

        // Save trade history
        // Material: File Handling — append trade record
        std::ofstream tradeFile(FileConfig::TRADE_HISTORY_FILE, std::ios::app);
        if (tradeFile.is_open()) {
            tradeFile << getTodayDate() << ","
                      << wallet.playerName << ","
                      << "BUY" << ","
                      << "BTC" << ","
                      << std::fixed << std::setprecision(4) << amount << ","
                      << btcPrice << ","
                      << totalCost << "\n";
            tradeFile.close();
        }

        std::cout << "\n    " << GameColor::TXT_SUCCESS << "[Success] Bought " << std::fixed << std::setprecision(4)
                  << amount << " BTC for " << totalCost << " coin!" << GameColor::RESET << "\n";
        displayWallet(wallet);
    }

    // ============================================================
    // Material: Function, Exception Handling — Sell BTC
    // ============================================================

    void sellBTC(Wallet& wallet) {
        int btcPrice = getCurrentBTCPrice();

        std::cout << "\n    Current BTC Price: " << btcPrice << " coin\n";
        std::cout << "    Your BTC: " << std::fixed << std::setprecision(4)
                  << wallet.btc << "\n\n";

        double amount = 0.0;
        std::cout << "    How much BTC to sell: ";
        std::cin >> amount;

        // Material: Exception Handling — input validation
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Invalid input." << GameColor::RESET << "\n";
            return;
        }

        if (amount <= 0) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Amount must be greater than 0." << GameColor::RESET << "\n";
            return;
        }

        // IMPORTANT: Validate BTC balance BEFORE modifying coin
        if (wallet.btc < amount) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Not enough BTC!" << GameColor::RESET
                      << " You have " << std::fixed << std::setprecision(4)
                      << wallet.btc << " BTC.\n";
            return;
        }

        int totalEarned = static_cast<int>(amount * btcPrice);

        // Execute transaction
        wallet.btc -= amount;
        wallet.coin += totalEarned;

        // Save wallet
        saveWallet(wallet);

        // Save trade history
        std::ofstream tradeFile(FileConfig::TRADE_HISTORY_FILE, std::ios::app);
        if (tradeFile.is_open()) {
            tradeFile << getTodayDate() << ","
                      << wallet.playerName << ","
                      << "SELL" << ","
                      << "BTC" << ","
                      << std::fixed << std::setprecision(4) << amount << ","
                      << btcPrice << ","
                      << totalEarned << "\n";
            tradeFile.close();
        }

        std::cout << "\n    " << GameColor::TXT_SUCCESS << "[Success] Sold " << std::fixed << std::setprecision(4)
                  << amount << " BTC for " << totalEarned << " coin.\n";
        displayWallet(wallet);
    }

    // ============================================================
    // Material: File Handling, Iterator — Show trade history
    // ============================================================

    void showTradeHistory() {
        std::ifstream file(FileConfig::TRADE_HISTORY_FILE);

        if (!file.is_open()) {
            std::cout << "\n    No trade history found.\n";
            return;
        }

        std::string line;
        // Skip header
        std::getline(file, line);

        // Check if there's any data after header
        bool hasData = false;

        printSeparator();
        std::cout << GameColor::MENU_TITLE << "              TRADE HISTORY\n" << GameColor::RESET;
        printSeparator();

        std::cout << "  " << std::left
                  << std::setw(12) << "Date"
                  << std::setw(12) << "Player"
                  << std::setw(6)  << "Type"
                  << std::setw(6)  << "Asset"
                  << std::setw(10) << "Amount"
                  << std::setw(8)  << "Price"
                  << std::setw(8)  << "Total" << "\n";

        std::cout << "  ";
        for (int i = 0; i < 44; ++i) std::cout << "-";
        std::cout << "\n";

        // Material: File Handling — read CSV line by line
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            hasData = true;

            std::istringstream iss(line);
            std::string day, name, type, asset, amount, price, total;

            std::getline(iss, day, ',');
            std::getline(iss, name, ',');
            std::getline(iss, type, ',');
            std::getline(iss, asset, ',');
            std::getline(iss, amount, ',');
            std::getline(iss, price, ',');
            std::getline(iss, total, ',');

            std::cout << "  " << std::left
                      << std::setw(12) << day
                      << std::setw(12) << name
                      << std::setw(6)  << type
                      << std::setw(6)  << asset
                      << std::setw(10) << amount
                      << std::setw(8)  << price
                      << std::setw(8)  << total << "\n";
        }

        if (!hasData) {
            std::cout << "    No transactions yet.\n";
        }

        printSeparator();
        file.close();
    }

    // ============================================================
    // Material: Function — Trading menu entry point
    // ============================================================

    void menuTrading(Wallet& wallet) {
        char choice;

        do {
            system("cls");
            printSeparator();
            std::cout << GameColor::MENU_TITLE << "           TRADING MARKET\n" << GameColor::RESET;
            printSeparator();
            std::cout << "\n";
            std::cout << "    [1] View Wallet\n";
            std::cout << "    [2] View BTC Price\n";
            std::cout << "    [3] Buy BTC\n";
            std::cout << "    [4] Sell BTC\n";
            std::cout << "    [5] Trade History\n";
            std::cout << "    [0] Back to Main Menu\n";
            std::cout << "\n    Your choice: ";
            
            choice = _getch();
            std::cout << choice << "\n\n";

            switch (choice) {
                case '1':
                    displayWallet(wallet);
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case '2':
                    displayMarket();
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case '3':
                    buyBTC(wallet);
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case '4':
                    sellBTC(wallet);
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case '5':
                    showTradeHistory();
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case 'm':
                case 'M':
                    forceUpdateBTCPrice();
                    std::cout << "    [Secret] Market price has been magically updated.\n";
                    std::cout << "    Press any key to continue...";
                    _getch();
                    break;
                case '0':
                    // Save wallet before exiting trading menu
                    saveWallet(wallet);
                    break;
                default:
                    std::cout << "    Invalid choice. Press any key...";
                    _getch();
                    break;
            }

        } while (choice != '0');
    }

} // namespace TradingModule
