/*==================================================
  MODUL : main.cpp
  FUNGSI:
  - Entry point aplikasi
==================================================*/
#include "game.h"
#include "gameutils.h"
#include <iostream>

/*==================================================
  MATERI: Function
  Titik awal eksekusi program.
==================================================*/
int main() {
    // Mengaktifkan Pemrosesan Warna ANSI untuk Terminal Windows
    GameUtils::initConsole();

    /*==================================================
      MATERI: Exception Handling
      Menangkap error fatal saat program berjalan.
    ==================================================*/
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "\n  Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
