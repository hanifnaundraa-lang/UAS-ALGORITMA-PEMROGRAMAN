#include "enemy.h"
#include <cstdlib>
#include <ctime>

// Material: STL List & Iterator
namespace EnemyModule {

    void spawnEnemy(std::list<Enemy>& enemies) {
        Enemy e;
        // Random x position within arena bounds
        e.position.x = rand() % GameConfig::ARENA_WIDTH;
        e.position.y = 0;  // Start at top
        e.health = 1;
        e.active = true;
        enemies.push_back(e);
    }

    void updateEnemies(std::list<Enemy>& enemies) {
        // Material: Iterator — update and remove inactive enemies
        for (auto it = enemies.begin(); it != enemies.end(); ) {
            it->position.y += 1;  // Move enemy downward

            // Deactivate if enemy moves below the arena
            if (it->position.y >= GameConfig::ARENA_HEIGHT) {
                it->active = false;
            }

            // Remove inactive enemies
            if (!it->active) {
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }
    }

} // namespace EnemyModule
