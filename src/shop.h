#pragma once
#include "gamedata.h"
#include <vector>
#include <string>

/*==================================================
  MATERI: NAMESPACE, FUNCTION, STL VECTOR, SORT, LAMBDA, FILE HANDLING, EXCEPTION HANDLING
==================================================*/
/*==================================================
  MODUL SHOP MENANGANI INTEGRASI WALLET, STOK BARANG, PENGURUTAN, DAN RIWAYAT TRANSAKSI.
==================================================*/
namespace ShopModule {

    // Manajemen katalog toko dengan memuat dari file CSV ke dalam memori.
    std::vector<Item> loadShopCatalog();

    // Menyimpan pembaruan stok kembali ke dalam file CSV.
    void saveShopCatalog(const std::vector<Item>& catalog);

    // Menampilkan katalog toko beserta ketersediaan stok.
    void displayShop(const std::vector<Item>& catalog, int walletCoin);

    // Membeli barang menggunakan wallet dengan validasi kuantitas dan stok.
    bool buyItem(Wallet& wallet, std::vector<Item>& catalog,
                 std::vector<Item>& inventory, int itemId, int quantity = 1);

    /*==================================================
      MATERI: SORT, LAMBDA - OPSI PENGURUTAN UNTUK KATALOG TOKO.
    ==================================================*/
    void sortByPriceAsc(std::vector<Item>& catalog);   // LEVEL 3: Harga terendah.
    void sortByPriceDesc(std::vector<Item>& catalog);   // LEVEL 3: Harga tertinggi.
    void sortByNameAsc(std::vector<Item>& catalog);     // LEVEL 3: Nama A-Z.
    void sortByNameDesc(std::vector<Item>& catalog);     // LEVEL 3: Nama Z-A.
    void sortByIdAsc(std::vector<Item>& catalog);       // LEVEL 3: Berdasarkan ID numerik.

    // Fungsi untuk menyimpan riwayat pembelian.
    void saveShopHistory(const std::string& playerName, const std::string& itemName,
                         int quantity, int totalCost);

    // Titik masuk menu utama toko menggunakan instance wallet.
    void runShop(Wallet& wallet, std::vector<Item>& inventory);

} // namespace ShopModule
