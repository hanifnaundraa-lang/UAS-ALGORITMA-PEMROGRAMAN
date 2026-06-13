#include "enemy.h"

Enemy::Enemy(QObject *parent) : QObject(parent), m_position{0, 0} {}

void Enemy::setX(float x) {
    if (m_position.x != x) {
        m_position.x = x;
        emit xChanged();
    }
}

void Enemy::setY(float y) {
    if (m_position.y != y) {
        m_position.y = y;
        emit yChanged();
    }
}
