#include "bullet.h"

Bullet::Bullet(QObject *parent) : QObject(parent), m_position{0, 0} {}

void Bullet::setX(float x) {
    if (m_position.x != x) {
        m_position.x = x;
        emit xChanged();
    }
}

void Bullet::setY(float y) {
    if (m_position.y != y) {
        m_position.y = y;
        emit yChanged();
    }
}
