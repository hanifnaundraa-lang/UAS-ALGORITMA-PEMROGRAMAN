#pragma once
#include "gamedata.h"
#include "gameconfig.h"
#include <list>

// Material: STL List, Function declarations
namespace EnemyModule {

    // Spawn a new enemy at random x position at the top
    void spawnEnemy(std::list<Enemy>& enemies);

    // Spawn a new Armored Enemy at random x position at the top
    void spawnArmored(std::list<Enemy>& enemies);

    // Update all enemies (move downward), remove inactive ones
    // Material: Iterator
    void updateEnemies(std::list<Enemy>& enemies);

} // namespace EnemyModule
