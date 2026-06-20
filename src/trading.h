#pragma once
#include "gamedata.h"
#include "gameconfig.h"
#include "gameexception.h"
#include <string>

// Material: Namespace, Function — Trading module by Member 4
// Simulated BTC trading market using in-game coin
namespace TradingModule {

    // --- Wallet Persistence ---
    // Material: File Handling & Exception Handling
    Wallet loadWallet(const std::string& playerName);
    void saveWallet(const Wallet& wallet);

    // --- BTC Price System ---
    // Returns today's BTC price. Generates a new one if none exists for today.
    int getCurrentBTCPrice();

    // --- Trading Actions ---
    void displayWallet(const Wallet& wallet);
    void displayMarket();
    void buyBTC(Wallet& wallet);
    void sellBTC(Wallet& wallet);
    void showTradeHistory();

    // --- Menu Entry Point ---
    // Called from Game::showTrading()
    void menuTrading(Wallet& wallet);

} // namespace TradingModule
