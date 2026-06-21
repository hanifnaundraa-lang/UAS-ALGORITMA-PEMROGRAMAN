#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

// Placeholder for Leaderboard module — will be implemented by Member 2
namespace ScoreManager {

    // Save a player's score to leaderboard CSV
    void saveScore(const PlayerScore& score);

    // Load all scores from leaderboard CSV
    std::vector<PlayerScore> loadScores();

    // Run the interactive leaderboard menu
    void runLeaderboardMenu();

} // namespace ScoreManager
