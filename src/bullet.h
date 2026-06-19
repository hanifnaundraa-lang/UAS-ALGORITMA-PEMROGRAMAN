#pragma once
#include "gamedata.h"
#include <vector>

// Material: STL Vector, Function declarations
namespace BulletModule {

    // Spawn a new bullet at the given position
    void spawnBullet(std::vector<Bullet>& bullets, int x, int y);

    // Update all bullets (move upward), remove inactive ones
    // Material: Iterator
    void updateBullets(std::vector<Bullet>& bullets);

} // namespace BulletModule
