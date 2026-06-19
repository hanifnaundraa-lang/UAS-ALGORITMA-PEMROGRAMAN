#include "trading.h"
#include <iostream>

// Placeholder — full implementation by Member 4
namespace TradingModule {

    void displayMarket() {
        std::cout << "[Trading Market] Coming soon..." << std::endl;
    }

    void displayWallet(const Wallet& wallet) {
        (void)wallet;
        std::cout << "[Wallet] Coming soon..." << std::endl;
    }

    bool buyBTC(Wallet& wallet, double amount, int price) {
        (void)wallet; (void)amount; (void)price;
        return false;
    }

    bool sellBTC(Wallet& wallet, double amount, int price) {
        (void)wallet; (void)amount; (void)price;
        return false;
    }

    void nextDay() {
        // TODO: Generate next day market prices
    }

} // namespace TradingModule
