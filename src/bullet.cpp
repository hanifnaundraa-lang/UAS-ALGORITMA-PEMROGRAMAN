#include "bullet.h"

// Material: STL Vector & Iterator
namespace BulletModule {

    void spawnBullet(std::vector<Bullet>& bullets, int x, int y) {
        Bullet b;
        b.position.x = x;
        b.position.y = y;
        b.active = true;
        bullets.push_back(b);
    }

    void updateBullets(std::vector<Bullet>& bullets) {
        // Material: Iterator — update and remove inactive bullets
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            it->position.y -= 1;  // Move bullet upward

            // Deactivate if bullet moves above the arena
            if (it->position.y < 0) {
                it->active = false;
            }

            // Remove inactive bullets
            if (!it->active) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
    }

} // namespace BulletModule
