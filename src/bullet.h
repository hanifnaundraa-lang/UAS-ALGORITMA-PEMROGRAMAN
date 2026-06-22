#pragma once
#include "gamedata.h"
#include <vector>

/*==================================================
  MODUL BULLET
==================================================*/
namespace BulletModule {

    // Memunculkan peluru baru pada posisi tertentu.
    void spawnBullet(std::vector<Bullet>& bullets, int x, int y);

    // Memperbarui posisi semua peluru (bergerak ke atas), dan menghapus peluru yang tidak aktif.
    /*==================================================
      MATERI: ITERATOR
    ==================================================*/
    void updateBullets(std::vector<Bullet>& bullets);

} // namespace BulletModule
