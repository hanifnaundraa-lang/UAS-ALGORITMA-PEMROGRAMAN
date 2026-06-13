#include "player.h"

Player::Player(QObject *parent) : QObject(parent), m_position{400, 500} {}

void Player::setX(float x) {
    if (m_position.x != x) {
        m_position.x = x;
        emit xChanged();
    }
}

void Player::setY(float y) {
    if (m_position.y != y) {
        m_position.y = y;
        emit yChanged();
    }
}
