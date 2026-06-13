#include "scoremanager.h"
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include "gameexception.h"

ScoreManager::ScoreManager(QObject *parent) : QObject(parent) {}

void ScoreManager::addScore(const PlayerScore& score) {
    m_scores.push_back(score);
}

void ScoreManager::addScore(QString name, int score, int enemies) {
    m_scores.push_back({name, score, enemies, QDateTime::currentDateTime()});
}

// Material: File Handling & CSV Processing
void ScoreManager::loadLeaderboard(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw FileLoadException(filename.toStdString());
    }

    QTextStream in(&file);
    QString header = in.readLine(); // Skip header
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (fields.size() >= 4) {
            PlayerScore ps;
            ps.name = fields[0];
            ps.score = fields[1].toInt();
            ps.destroyedEnemy = fields[2].toInt();
            ps.dateTime = QDateTime::fromString(fields[3], Qt::ISODate);
            m_scores.push_back(ps);
        }
    }
    file.close();
}

void ScoreManager::saveLeaderboard(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    out << "name,score,destroyedEnemy,dateTime\n";
    for (const auto& s : m_scores) {
        out << s.name << "," << s.score << "," << s.destroyedEnemy << "," 
            << s.dateTime.toString(Qt::ISODate) << "\n";
    }
    file.close();
}

// Material: Sort & Lambda Expression
void ScoreManager::sortScores() {
    std::sort(m_scores.begin(), m_scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return a.score > b.score;
    });
}

// Material: Count & Lambda Expression
int ScoreManager::countHighScores(int threshold) {
    return std::count_if(m_scores.begin(), m_scores.end(), [threshold](const PlayerScore& s) {
        return s.score >= threshold;
    });
}
