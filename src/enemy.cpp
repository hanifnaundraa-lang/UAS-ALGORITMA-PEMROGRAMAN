#include "enemy.h"
#include <cstdlib>
#include <ctime>

/*==================================================
  MODUL ENEMY
==================================================*/
namespace EnemyModule {

    void spawnEnemy(std::list<Enemy>& enemies) {
        Enemy e;
        // Posisi x acak di dalam batas arena.
        e.position.x = rand() % GameConfig::ARENA_WIDTH;
        e.position.y = 0;  // Mulai dari atas.
        e.health = 1;
        e.active = true;
        enemies.push_back(e);
    }

    void updateEnemies(std::list<Enemy>& enemies) {
        /*==================================================
          MATERI: ITERATOR UNTUK MEMPERBARUI DAN MENGHAPUS MUSUH YANG TIDAK AKTIF.
        ==================================================*/
        for (auto it = enemies.begin(); it != enemies.end(); ) {
            it->position.y += 1;  // Menggerakkan musuh ke bawah.

            // Menonaktifkan musuh jika bergerak ke bawah luar arena.
            if (it->position.y >= GameConfig::ARENA_HEIGHT) {
                it->active = false;
            }

            // Menghapus musuh yang tidak aktif.
            if (!it->active) {
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }
    }

} // namespace EnemyModule
