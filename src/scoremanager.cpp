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
        // Membuka file dalam mode append.
        ofstream outFile(FileConfig::LEADERBOARD_FILE, ios::app);
        
        if (!outFile.is_open()) {
            throw runtime_error("Error: Unable to open " + FileConfig::LEADERBOARD_FILE + " for writing.");
        }

        // Format data: name,score,destroyedEnemy,dateTime
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
            return scores; // Mengembalikan vektor kosong jika file tidak ditemukan.
        }

        string line;
        bool isHeader = true;

        while (getline(inFile, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string name, scoreStr, enemiesStr, dateTime;

            getline(ss, name, ',');
            
            // Mengabaikan baris header pada file CSV.
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

            /*==================================================
              MATERI: EXCEPTION HANDLING UNTUK DATA ANGKA YANG TIDAK VALID.
            ==================================================*/
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

    // Menampilkan daftar peringkat skor tertinggi.
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

    // Mencari data pemain berdasarkan nama.
    void searchPlayer(const vector<PlayerScore>& scores) {
        cout << "\n  Enter Player Name: ";
        string target;
        getline(cin, target);

        string lowerTarget = target;
        transform(lowerTarget.begin(), lowerTarget.end(), lowerTarget.begin(), ::tolower);
        
        /*==================================================
          MATERI: STL FIND & LAMBDA EXPRESSION
        ==================================================*/
        auto it = find_if(scores.begin(), scores.end(), [&](const PlayerScore& s) {
            string lowerName = s.name;
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            return lowerName == lowerTarget;
        });

        if (it != scores.end()) {
            int rank = distance(scores.begin(), it) + 1;
            cout << "\n  Player Found\n\n";
            cout << "  Name  : " << it->name << "\n";
            cout << "  Rank  : " << rank << "\n";
            cout << "  Score : " << it->score << "\n";
        } else {
            cout << "\n  Player not found.\n";
        }
    }

    // Menampilkan statistik leaderboard (total pemain, skor tertinggi, terendah, rata-rata).
    void showStatistics(const vector<PlayerScore>& scores) {
        cout << "\n  ===== LEADERBOARD STATS =====\n\n";
        if (scores.empty()) {
            cout << "  No data available.\n";
            return;
        }

        vector<string> uniquePlayers;
        int maxScore = 0;
        int minScore = scores[0].score;
        long long sumScore = 0;

        /*==================================================
          MATERI: ITERATOR
        ==================================================*/
        for (auto it = scores.begin(); it != scores.end(); ++it) {
            string lowerName = it->name;
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            
            if (find(uniquePlayers.begin(), uniquePlayers.end(), lowerName) == uniquePlayers.end()) {
                uniquePlayers.push_back(lowerName);
            }

            if (it->score > maxScore) maxScore = it->score;
            if (it->score < minScore) minScore = it->score;
            sumScore += it->score;
        }

        int avgScore = (scores.size() > 0) ? (sumScore / scores.size()) : 0;

        cout << "  Total Players : " << uniquePlayers.size() << "\n";
        cout << "  Highest Score : " << maxScore << "\n";
        cout << "  Lowest Score  : " << minScore << "\n";
        cout << "  Average Score : " << avgScore << "\n";
    }

    // Menampilkan skor terbaik dari pemain tertentu.
    void showPersonalBest(const vector<PlayerScore>& scores) {
        cout << "\n  Enter Player Name: ";
        string target;
        getline(cin, target);
        string lowerTarget = target;
        transform(lowerTarget.begin(), lowerTarget.end(), lowerTarget.begin(), ::tolower);

        int bestScore = -1;
        string actualName = "";

        /*==================================================
          MATERI: STL COUNT & LAMBDA EXPRESSION
        ==================================================*/
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
            cout << "\n  ===== PERSONAL BEST =====\n\n";
            cout << "  Player       : " << actualName << "\n";
            cout << "  Best Score   : " << bestScore << "\n";
            cout << "  Games Played : " << gamesPlayed << "\n";
        } else {
            cout << "\n  Player not found.\n";
        }
    }

    // Menampilkan pemain dengan skor tertinggi secara keseluruhan.
    void showHallOfFame(const vector<PlayerScore>& scores) {
        cout << "\n  ===== HALL OF FAME =====\n\n";
        if (scores.empty()) {
            cout << "  No records available.\n";
            return;
        }

        auto best = scores.begin(); // Sudah diurutkan secara menurun.

        cout << "  ***  The Greatest Player of All Time  ***\n\n";
        cout << "  Player : " << best->name << "\n";
        cout << "  Score  : " << best->score << "\n";
        cout << "  Date   : " << best->dateTime << "\n";
    }

    // Menjalankan loop menu utama untuk leaderboard.
    void runLeaderboardMenu() {
        vector<PlayerScore> scores = loadScores();

        // Mengurutkan secara global satu kali untuk indeks peringkat yang akurat.
        /*==================================================
          MATERI: STL SORT & LAMBDA EXPRESSION
        ==================================================*/
        sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
            return a.score > b.score; 
        });

        bool inMenu = true;
        while (inMenu) {
            system("cls");
            cout << "\n  ======================================================\n";
            cout << "                       LEADERBOARD                      \n";
            cout << "  ======================================================\n\n";
            cout << "    1. View Leaderboard\n";
            cout << "    2. Search Player\n";
            cout << "    3. Statistics\n";
            cout << "    4. Personal Best\n";
            cout << "    5. Hall Of Fame\n";
            cout << "    6. Back\n\n";
            cout << "    Enter your choice: ";
            
            char choice = _getch();
            cout << choice << "\n";
            
            system("cls");
            
            if (choice == '1') {
                cout << "\n  ======================================================\n";
                cout << "                       LEADERBOARD                      \n";
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
            } else if (choice == '6') {
                inMenu = false;
                continue;
            } else {
                cout << "\n  Invalid choice.\n";
            }
            
            if (inMenu) {
                cout << "\n  Press any key to return to Leaderboard Menu...\n";
                _getch();
            }
        }
    }

} // namespace ScoreManager
