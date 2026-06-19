#include "scoremanager.h"
#include <iostream>

// Placeholder — full implementation by Member 2
namespace ScoreManager {

    void saveScore(const PlayerScore& score) {
        // TODO: Save score to data/leaderboard.csv
        // Will use file handling, exception handling, lambda, sort, find, count
        (void)score; // suppress unused warning
    }

    std::vector<PlayerScore> loadScores() {
        // TODO: Load scores from data/leaderboard.csv
        return {};
    }

    void displayLeaderboard() {
        // TODO: Display sorted leaderboard
        std::cout << "[Leaderboard] Coming soon..." << std::endl;
    }

} // namespace ScoreManager
