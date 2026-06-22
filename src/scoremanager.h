#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

/*==================================================
  MODUL LEADERBOARD
==================================================*/
namespace ScoreManager {

    // Menyimpan skor pemain ke dalam file CSV leaderboard.
    void saveScore(const PlayerScore& score);

    // Memuat seluruh data skor dari file CSV leaderboard.
    std::vector<PlayerScore> loadScores();

    // Menjalankan menu interaktif untuk leaderboard.
    void runLeaderboardMenu();

} // namespace ScoreManager
