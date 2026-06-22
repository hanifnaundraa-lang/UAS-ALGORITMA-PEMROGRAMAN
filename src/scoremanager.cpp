#include "scoremanager.h"
#include "gameconfig.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <conio.h>
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

    // ============================================================
    // Feature: Display Leaderboard View (Rank Display)
    // ============================================================
    void displayLeaderboardView(vector<PlayerScore>& scores) {
        if (scores.empty()) {
            cout << "    (No high scores recorded yet. Play a game!)\n\n";
            return;
        }

        cout << "    " << left << setw(6) << "Rank"
             << setw(15) << "Name"
             << setw(10) << "Score"
             << setw(10) << "Enemies"
             << setw(20) << "Date" << "\n";
        cout << "    ------------------------------------------------------------\n";

        int rank = 1;
        for (const auto& s : scores) {
            if (rank > 10) break; 
            
            cout << "    #" << left << setw(5) << rank
                 << setw(15) << s.name
                 << setw(10) << s.score
                 << setw(10) << s.destroyedEnemy
                 << setw(20) << s.dateTime << "\n";
            rank++;
        }
        cout << "\n";
    }

    // ============================================================
    // Feature 1: Search Player
    // ============================================================
    void searchPlayer(const vector<PlayerScore>& scores) {
        cout << "\n  Enter Player Name: ";
        string target;
        getline(cin, target);

        string lowerTarget = target;
        transform(lowerTarget.begin(), lowerTarget.end(), lowerTarget.begin(), ::tolower);
        
        // Material: Find & Lambda Expression
        auto it = find_if(scores.begin(), scores.end(), [&](const PlayerScore& s) {
            string lowerName = s.name;
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            return lowerName == lowerTarget;
        });

        if (it != scores.end()) {
            int rank = distance(scores.begin(), it) + 1;
            cout << "\n  " << GameColor::TXT_SUCCESS << "Player Found" << GameColor::RESET << "\n\n";
            cout << "  Name  : " << it->name << "\n";
            cout << "  Rank  : " << rank << "\n";
            cout << "  Score : " << it->score << "\n";
        } else {
            cout << "\n  " << GameColor::TXT_WARNING << "Player not found." << GameColor::RESET << "\n";
        }
    }

    // ============================================================
    // Feature 2: Statistics Menu
    // ============================================================
    void showStatistics(const vector<PlayerScore>& scores) {
        cout << "\n  " << GameColor::MENU_TITLE << "===== LEADERBOARD STATS =====" << GameColor::RESET << "\n\n";
        if (scores.empty()) {
            cout << "  No data available.\n";
            return;
        }

        vector<string> uniquePlayers;
        int maxScore = scores[0].score;
        int minScore = scores[0].score;
        string namaMax = scores[0].name;
        string namaMin = scores[0].name;
        long long sumScore = 0;

        // Material: Iterator
        for (auto it = scores.begin(); it != scores.end(); ++it) {
            string lowerName = it->name;
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            
            if (find(uniquePlayers.begin(), uniquePlayers.end(), lowerName) == uniquePlayers.end()) {
                uniquePlayers.push_back(lowerName);
            }

            if (it->score > maxScore) {
                maxScore = it->score;
                namaMax = it->name;
            }
            if (it->score < minScore) {
                minScore = it->score;
                namaMin = it->name;
            }
            sumScore += it->score;
        }

        int avgScore = (scores.size() > 0) ? (sumScore / scores.size()) : 0;

        cout << "  Total Players : " << uniquePlayers.size() << "\n";
        cout << "  Highest Score : " << maxScore << " - [" << namaMax << "]" << "\n";
        cout << "  Lowest Score  : " << minScore << " - [" << namaMin << "]" << "\n";
        cout << "  Average Score : " << avgScore << "\n";
    }

    // ============================================================
    // Feature 3: Personal Best
    // ============================================================
    void showPersonalBest(const vector<PlayerScore>& scores) {
        cout << "\n  Enter Player Name: ";
        string target;
        getline(cin, target);
        string lowerTarget = target;
        transform(lowerTarget.begin(), lowerTarget.end(), lowerTarget.begin(), ::tolower);

        int bestScore = -1;
        string actualName = "";

        // Material: Count & Lambda Expression
        int gamesPlayed = count_if(scores.begin(), scores.end(), [&](const PlayerScore& s) {
            string lowerName = s.name;
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            if (lowerName == lowerTarget) {
                if (s.score > bestScore) {
                    bestScore = s.score;
                    actualName = s.name;
                }
                return true;
            }
            return false;
        });

        if (gamesPlayed > 0) {
            cout << "\n  " << GameColor::MENU_TITLE << "===== PERSONAL BEST =====" << GameColor::RESET << "\n\n";
            cout << "  Player       : " << actualName << "\n";
            cout << "  Best Score   : " << bestScore << "\n";
            cout << "  Games Played : " << gamesPlayed << "\n";
        } else {
            cout << "\n  " << GameColor::TXT_WARNING << "Player not found." << GameColor::RESET << "\n";
        }
    }

    // ============================================================
    // Feature 5: Hall of Fame
    // ============================================================
    void showHallOfFame(const vector<PlayerScore>& scores) {
        cout << "\n  " << GameColor::MENU_TITLE << "===== HALL OF FAME =====" << GameColor::RESET << "\n\n";
        if (scores.empty()) {
            cout << "  No records available.\n";
            return;
        }

        auto best = scores.begin(); // already sorted descending

        cout << "  " << GameColor::TXT_SUCCESS << "***  The Greatest Player of All Time  ***" << GameColor::RESET << "\n\n";
        cout << "  Player : " << best->name << "\n";
        cout << "  Score  : " << best->score << "\n";
        cout << "  Date   : " << best->dateTime << "\n";
    }

    // ============================================================
    // Main Leaderboard Menu Runner
    // ============================================================
    void runLeaderboardMenu() {
        vector<PlayerScore> scores = loadScores();

        // Sort globally once for accurate rank indexing
        // Material: Sort & Lambda Expression
        sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
            return a.score > b.score; 
        });

        bool inMenu = true;
        while (inMenu) {
            system("cls");
            cout << "\n  ======================================================\n";
            cout << GameColor::MENU_TITLE << "                       LEADERBOARD                      \n" << GameColor::RESET;
            cout << "  ======================================================\n\n";
            cout << "    [1] View Leaderboard\n";
            cout << "    [2] Search Player\n";
            cout << "    [3] Statistics\n";
            cout << "    [4] Personal Best\n";
            cout << "    [5] Hall Of Fame\n";
            cout << "    [0] Back\n\n";
            cout << "    Enter your choice: ";
            
            char choice = _getch();
            cout << choice << "\n";
            
            system("cls");
            
            if (choice == '1') {
                cout << "\n  ======================================================\n";
                cout << GameColor::MENU_TITLE << "                       LEADERBOARD                      \n" << GameColor::RESET;
                cout << "  ======================================================\n\n";
                displayLeaderboardView(scores);
            } else if (choice == '2') {
                searchPlayer(scores);
            } else if (choice == '3') {
                showStatistics(scores);
            } else if (choice == '4') {
                showPersonalBest(scores);
            } else if (choice == '5') {
                showHallOfFame(scores);
            } else if (choice == '0') {
                inMenu = false;
                break;
            } else {
                cout << "\n  " << GameColor::TXT_WARNING << "Invalid choice." << GameColor::RESET << "\n";
            }
            
            if (inMenu) {
                cout << "\n  Press any key to return to Leaderboard Menu...\n";
                _getch();
            }
        }
    }

} // namespace ScoreManager
