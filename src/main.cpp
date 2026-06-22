#include "game.h"
#include "gameutils.h"
#include <iostream>

// Material: Function — Program entry point
int main() {
    // Enable ANSI Color Processing for Windows Terminal
    GameUtils::initConsole();

    // Material: Exception Handling
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "\n  Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
