// #include "scoremanager.h"
// #include <iostream>

// // Placeholder — full implementation by Member 2
// namespace ScoreManager {

//     void saveScore(const PlayerScore& score) {
//         // TODO: Save score to data/leaderboard.csv
//         // Will use file handling, exception handling, lambda, sort, find, count
//         (void)score; // suppress unused warning
//     }

//     std::vector<PlayerScore> loadScores() {
//         // TODO: Load scores from data/leaderboard.csv
//         return {};
//     }

//     void displayLeaderboard() {
//         // TODO: Display sorted leaderboard
//         std::cout << "[Leaderboard] Coming soon..." << std::endl;
//     }

// } // namespace ScoreManager



#include "scoremanager.h"
#include "gameconfig.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

using namespace std;

namespace ScoreManager {

    void saveScore(const PlayerScore& score) {
        // Open file in append mode
        ofstream outFile(FileConfig::LEADERBOARD_FILE, ios::app);
        
        if (!outFile.is_open()) {
            throw runtime_error("Error: Unable to open " + FileConfig::LEADERBOARD_FILE + " for writing.");
        }

        // Format: name,score,destroyedEnemy,dateTime
        outFile << score.name << ","
                << score.score << ","
                << score.destroyedEnemy << ","
                << score.dateTime << "\n";

        outFile.close();
    }

    vector<PlayerScore> loadScores() {
        vector<PlayerScore> scores;
        ifstream inFile(FileConfig::LEADERBOARD_FILE);

        if (!inFile.is_open()) {
            return scores; // Return empty vector if file doesn't exist
        }

        string line;
        bool isHeader = true;

        while (getline(inFile, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string name, scoreStr, enemiesStr, dateTime;

            getline(ss, name, ',');
            
            // Check and skip header
            if (isHeader && name == "name") {
                isHeader = false;
                continue;
            }
            isHeader = false;

            getline(ss, scoreStr, ',');
            getline(ss, enemiesStr, ',');
            getline(ss, dateTime, ',');

            PlayerScore ps;
            ps.name = name;
            ps.dateTime = dateTime;

            // Exception handling for corrupted numbers
            try {
                ps.score = stoi(scoreStr);
                ps.destroyedEnemy = stoi(enemiesStr);
                scores.push_back(ps);
            } catch (...) {
                continue; 
            }
        }

        inFile.close();
        return scores;
    }

    void displayLeaderboard() {
        vector<PlayerScore> scores = loadScores();

        // Materi: Sort & Lambda Expression
        sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
            return a.score > b.score; 
        });

        cout << "\n  ======================================================\n";
        cout << "                       LEADERBOARD                      \n";
        cout << "  ======================================================\n\n";

        if (scores.empty()) {
            cout << "    (No high scores recorded yet. Play a game!)\n\n";
            return;
        }

        cout << "    " << left << setw(15) << "Name"
             << setw(10) << "Score"
             << setw(10) << "Enemies"
             << setw(20) << "Date" << "\n";
        cout << "    ------------------------------------------------------\n";

        // Display up to top 10 scores
        int rank = 1;
        for (const auto& s : scores) {
            if (rank > 10) break; 
            
            cout << "    " << left << setw(15) << s.name
                 << setw(10) << s.score
                 << setw(10) << s.destroyedEnemy
                 << setw(20) << s.dateTime << "\n";
            rank++;
        }
        cout << "\n";
    }

} // namespace ScoreManager