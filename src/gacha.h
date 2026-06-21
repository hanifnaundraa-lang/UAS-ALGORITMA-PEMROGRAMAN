#pragma once
#include "gamedata.h"
#include <string>

namespace GachaModule {
    void displayGachaPool();
    Item rollGacha(const std::string& playerName, int cost);
    Item rollWithTicket(const std::string& playerName);
}