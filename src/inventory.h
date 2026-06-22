#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

/*==================================================
  MATERI: NAMESPACE, FUNCTION, STL VECTOR, FIND, COUNT, SORT, LAMBDA, ITERATOR
==================================================*/
/*==================================================
  MODUL INVENTORY MENANGANI MANAJEMEN ITEM PEMAIN SEPERTI PENCARIAN, FILTER, DAN PENYIMPANAN MENGGUNAKAN CSV.
==================================================*/
namespace InventoryModule {

    // Operasi CRUD inti untuk inventory.
    void addItem(std::vector<Item>& inventory, const Item& item);
    bool removeItem(std::vector<Item>& inventory, int itemId);

    // Pencarian dan perhitungan.
    /*==================================================
      MATERI: FIND, LAMBDA - PENCARIAN ITEM BERDASARKAN ID.
    ==================================================*/
    Item* findItem(std::vector<Item>& inventory, int itemId);

    /*==================================================
      MATERI: COUNT, LAMBDA - MENGHITUNG JUMLAH ITEM BERDASARKAN ID.
    ==================================================*/
    int countItem(const std::vector<Item>& inventory, int itemId);

    /*==================================================
      MATERI: FIND, LAMBDA - PENCARIAN ITEM BERDASARKAN NAMA.
    ==================================================*/
    std::vector<Item> searchByName(const std::vector<Item>& inventory, const std::string& keyword);

    /*==================================================
      MATERI: ITERATOR, LAMBDA - MEMFILTER ITEM BERDASARKAN TIPE.
    ==================================================*/
    std::vector<Item> filterByType(const std::vector<Item>& inventory, const std::string& type);

    // Menampilkan isi inventory ke layar.
    void displayInventory(const std::vector<Item>& inventory);

    // Menjalankan menu interaktif untuk inventory.
    void runInventory(std::vector<Item>& inventory, const std::string& playerName);

    // Menyimpan dan memuat inventory dari file CSV dengan format kolom yang telah ditentukan.
    void saveInventory(const std::string& playerName, const std::vector<Item>& inventory);
    std::vector<Item> loadInventory(const std::string& playerName);

} // namespace InventoryModule
