#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include "gamedata.h"

class Bullet : public QObject {
    Q_OBJECT
    Q_PROPERTY(float x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(float y READ y WRITE setY NOTIFY yChanged)

public:
    explicit Bullet(QObject *parent = nullptr);

    float x() const { return m_position.x; }
    void setX(float x);

    float y() const { return m_position.y; }
    void setY(float y);

signals:
    void xChanged();
    void yChanged();

private:
    Vector2D m_position;
};

#endif // BULLET_H
