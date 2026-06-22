#include "bullet.h"

/*==================================================
  MODUL BULLET
==================================================*/
namespace BulletModule {

    void spawnBullet(std::vector<Bullet>& bullets, int x, int y) {
        Bullet b;
        b.position.x = x;
        b.position.y = y;
        b.active = true;
        bullets.push_back(b);
    }

    void updateBullets(std::vector<Bullet>& bullets) {
        /*==================================================
          MATERI: ITERATOR UNTUK MEMPERBARUI DAN MENGHAPUS PELURU YANG TIDAK AKTIF.
        ==================================================*/
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            it->position.y -= 1;  // Menggerakkan peluru ke atas.

            // Menonaktifkan peluru jika bergerak ke atas luar arena.
            if (it->position.y < 0) {
                it->active = false;
            }

            // Menghapus peluru yang tidak aktif.
            if (!it->active) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
    }

} // namespace BulletModule
