#pragma once
#include <string>
#include <iostream>
#include <windows.h>

// Material: Namespace, Function, Default Argument & Inline Function, Function Template
namespace GameUtils {

    // Material: Function Template
    template <typename T>
    T clampValue(T value, T minVal, T maxVal) {
        if (value < minVal) return minVal;
        if (value > maxVal) return maxVal;
        return value;
    }

    // Material: Inline Function
    inline bool isWithinBounds(int x, int y, int width, int height) {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    // Material: Function Overloading — clear screen
    inline void clearScreen() {
        // Use Windows API for flicker-free rendering
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD topLeft = {0, 0};
        SetConsoleCursorPosition(hConsole, topLeft);
    }

    // Material: Function Overloading — set cursor position
    inline void setCursorPosition(int x, int y) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
        SetConsoleCursorPosition(hConsole, pos);
    }

    // Hide/show cursor for cleaner rendering
    inline void setCursorVisible(bool visible) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = visible;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    // Material: Default Argument
    inline int calculateReward(int baseReward, int multiplier = 1) {
        return baseReward * multiplier;
    }

    // Material: Function Overloading — print message
    inline void printCentered(const std::string& text, int width) {
        int padding = (width - static_cast<int>(text.length())) / 2;
        if (padding < 0) padding = 0;
        for (int i = 0; i < padding; ++i) std::cout << ' ';
        std::cout << text;
    }

    // Material: Function Overloading — print with newline
    inline void printCentered(const std::string& text, int width, bool newline) {
        printCentered(text, width);
        if (newline) std::cout << '\n';
    }

} // namespace GameUtils
