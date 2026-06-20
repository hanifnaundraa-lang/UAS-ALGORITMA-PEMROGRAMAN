#include "game.h"
#include <iostream>

// Material: Function — Program entry point
int main() {
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
