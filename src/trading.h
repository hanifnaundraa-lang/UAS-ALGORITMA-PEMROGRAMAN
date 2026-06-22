#pragma once
#include "gamedata.h"
#include "gameconfig.h"
#include "gameexception.h"
#include <string>

/*==================================================
  MATERI: NAMESPACE, FUNCTION
==================================================*/
/*==================================================
  MODUL TRADING BERFUNGSI SEBAGAI SIMULASI PASAR ASET DIGITAL (BTC) MEMAKAI MATA UANG DALAM GAME.
==================================================*/
namespace TradingModule {

    /*==================================================
      MATERI: FILE HANDLING, EXCEPTION HANDLING - MEMBACA DAN MENYIMPAN DATA SALDO DOMPET.
    ==================================================*/
    Wallet loadWallet(const std::string& playerName);
    void saveWallet(const Wallet& wallet);

    // Mekanisme fluktuasi harga otomatis, hari baru akan memunculkan nilai acak harga terbaru dari BTC.
    int getCurrentBTCPrice();

    // Aksi yang bisa dimainkan dalam menu Trading.
    void displayWallet(const Wallet& wallet);
    void displayMarket();
    void buyBTC(Wallet& wallet);
    void sellBTC(Wallet& wallet);
    void showTradeHistory();

    // Titik akses modular yang akan dipanggil dari navigasi root utama game.
    void menuTrading(Wallet& wallet);

} // namespace TradingModule
