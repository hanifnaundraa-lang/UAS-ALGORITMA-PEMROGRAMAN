#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <QObject>
#include <vector>
#include <list>
#include "gamedata.h"

// Material: STL Vector & List, File Handling & CSV Processing
class ScoreManager : public QObject {
    Q_OBJECT
public:
    explicit ScoreManager(QObject *parent = nullptr);

    // Material: Function Overloading
    void addScore(const PlayerScore& score);
    void addScore(QString name, int score, int enemies);

    void loadLeaderboard(const QString& filename);
    void saveLeaderboard(const QString& filename);

    // Material: Sort, Find, Count, Lambda Expression
    void sortScores();
    int countHighScores(int threshold);

private:
    // Material: STL Vector & List
    std::vector<PlayerScore> m_scores;
};

#endif // SCOREMANAGER_H
