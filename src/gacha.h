#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

/*==================================================
  MATERI: NAMESPACE, FUNCTION, STL VECTOR, ITERATOR, SORT, FIND, COUNT, LAMBDA, FILE HANDLING, EXCEPTION HANDLING
==================================================*/
/*==================================================
  MODUL GACHA MENANGANI SISTEM ACAK BERBOBOT UNTUK UNDIAN ITEM BESERTA PENCATATAN RIWAYAT DAN STATISTIK.
==================================================*/
namespace GachaModule {

    /*==================================================
      MATERI: FILE HANDLING, EXCEPTION HANDLING, STL VECTOR - PEMUATAN KOLEKSI HADIAH UNDIAN.
    ==================================================*/
    std::vector<GachaReward> loadPool();

    /*==================================================
      MATERI: LAMBDA EXPRESSION, STL VECTOR, ITERATOR - LOGIKA EKSEKUSI PENARIKAN GACHA.
    ==================================================*/
    GachaReward rollOnce(const std::vector<GachaReward>& pool);

    /*==================================================
      MATERI: FILE HANDLING, EXCEPTION HANDLING - PENCATATAN DAN PEMBACAAN REKAMAN RIWAYAT GACHA.
    ==================================================*/
    void saveHistory(const std::string& playerName, const GachaReward& reward);
    std::vector<GachaRecord> loadHistory(const std::string& playerName);

    /*==================================================
      MATERI: FUNCTION, STL VECTOR, COUNT, FIND, SORT, LAMBDA - MENU INTERAKTIF MODUL GACHA.
    ==================================================*/
    void runGachaMenu(std::vector<Item>& inventory, const std::string& playerName);

} // namespace GachaModule