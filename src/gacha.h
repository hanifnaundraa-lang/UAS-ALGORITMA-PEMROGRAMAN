#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

// Material: Namespace, Function, STL Vector, Iterator, Sort, Find, Count, Lambda, File Handling, Exception Handling
// Gacha module — weighted random pull system with history and statistics
namespace GachaModule {

    // --- Pool Loading ---
    // Material: File Handling, Exception Handling, STL Vector
    std::vector<GachaReward> loadPool();

    // --- Core Pull Logic ---
    // Material: Lambda Expression, STL Vector, Iterator
    GachaReward rollOnce(const std::vector<GachaReward>& pool);

    // --- History ---
    // Material: File Handling, Exception Handling
    void saveHistory(const std::string& playerName, const GachaReward& reward);
    std::vector<GachaRecord> loadHistory(const std::string& playerName);

    // --- Interactive Menu ---
    // Material: Function, STL Vector, Count, Find, Sort, Lambda
    void runGachaMenu(std::vector<Item>& inventory, const std::string& playerName);

} // namespace GachaModule