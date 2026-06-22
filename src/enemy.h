#pragma once
#include "gamedata.h"
#include "gameconfig.h"
#include <list>

/*==================================================
  MODUL ENEMY
==================================================*/
namespace EnemyModule {

    // Memunculkan musuh baru pada posisi x acak di bagian atas arena.
    void spawnEnemy(std::list<Enemy>& enemies);

    // Memperbarui posisi semua musuh (bergerak ke bawah), dan menghapus musuh yang tidak aktif.
    /*==================================================
      MATERI: ITERATOR
    ==================================================*/
    void updateEnemies(std::list<Enemy>& enemies);

} // namespace EnemyModule
