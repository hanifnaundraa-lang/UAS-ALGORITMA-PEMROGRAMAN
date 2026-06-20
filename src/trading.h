#pragma once
#include "gamedata.h"
#include <vector>

// Placeholder for Trading module — will be implemented by Member 4
namespace TradingModule {

    void displayMarket();
    void displayWallet(const Wallet& wallet);
    void buyBTC(Wallet& wallet, double amount, int price);
    void sellBTC(Wallet& wallet, double amount, int price);
    void history();

} // namespace TradingModule
