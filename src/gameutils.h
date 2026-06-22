#pragma once
#include <string>
#include <iostream>
#include <windows.h>

/*==================================================
  MATERI: NAMESPACE, FUNCTION, DEFAULT ARGUMENT & INLINE FUNCTION, FUNCTION TEMPLATE
==================================================*/
namespace GameUtils {

    // Menginisialisasi konsol Windows untuk mendukung warna ANSI
    inline void initConsole() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;
        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
        SetConsoleOutputCP(CP_UTF8);
    }

    /*==================================================
      MATERI: FUNCTION TEMPLATE
    ==================================================*/
    template <typename T>
    T clampValue(T value, T minVal, T maxVal) {
        if (value < minVal) return minVal;
        if (value > maxVal) return maxVal;
        return value;
    }

    /*==================================================
      MATERI: INLINE FUNCTION
    ==================================================*/
    inline bool isWithinBounds(int x, int y, int width, int height) {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    /*==================================================
      MATERI: FUNCTION OVERLOADING
    ==================================================*/
    /*==================================================
      FUNGSI: Membersihkan layar terminal
    ==================================================*/
    inline void clearScreen() {
        // Menggunakan Windows API untuk rendering tanpa flicker
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD topLeft = {0, 0};
        SetConsoleCursorPosition(hConsole, topLeft);
    }

    /*==================================================
      MATERI: FUNCTION OVERLOADING
    ==================================================*/
    /*==================================================
      FUNGSI: Mengatur posisi kursor di terminal
    ==================================================*/
    inline void setCursorPosition(int x, int y) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
        SetConsoleCursorPosition(hConsole, pos);
    }

    // Menyembunyikan atau menampilkan kursor untuk rendering yang lebih bersih
    inline void setCursorVisible(bool visible) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = visible;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    /*==================================================
      MATERI: DEFAULT ARGUMENT
    ==================================================*/
    inline int calculateReward(int baseReward, int multiplier = 1) {
        return baseReward * multiplier;
    }

    /*==================================================
      MATERI: FUNCTION OVERLOADING
    ==================================================*/
    /*==================================================
      FUNGSI: Mencetak pesan ke tengah layar
    ==================================================*/
    inline void printCentered(const std::string& text, int width) {
        int padding = (width - static_cast<int>(text.length())) / 2;
        if (padding < 0) padding = 0;
        for (int i = 0; i < padding; ++i) std::cout << ' ';
        std::cout << text;
    }

    /*==================================================
      MATERI: FUNCTION OVERLOADING
    ==================================================*/
    /*==================================================
      FUNGSI: Mencetak pesan ke tengah layar dengan baris baru
    ==================================================*/
    inline void printCentered(const std::string& text, int width, bool newline) {
        printCentered(text, width);
        if (newline) std::cout << '\n';
    }

} // namespace GameUtils
