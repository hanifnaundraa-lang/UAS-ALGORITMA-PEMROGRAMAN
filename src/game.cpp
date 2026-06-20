#include "game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

// Material: Function, STL Vector & List, Iterator, Struct, Pointer & Reference

Game::Game()
    : isRunning(false), isPaused(false), frameCounter(0) {
    // Seed random number generator once
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

// ============================================================
// Material: Function — Main application entry
// ============================================================
void Game::run() {
    // Material: Exception Handling
    try {
        bool appRunning = true;
        while (appRunning) {
            showMainMenu();

            // Wait for menu choice
            int choice = 0;
            std::cin >> choice;
            std::cin.ignore(); // clear newline from buffer

            switch (choice) {
                case 1:
                    inputPlayerName();
                    startGame();
                    break;
                case 2:
                    showInventory();
                    break;
                case 3:
                    showShop();
                    break;
                case 4:
                    showTrading();
                    break;
                case 5:
                    showControls();
                    break;
                case 6:
                    appRunning = false;
                    break;
                default:
                    std::cout << "\n  Invalid choice. Press any key to continue...";
                    _getch();
                    break;
            }
        }

        // Restore cursor before exiting
        GameUtils::setCursorVisible(true);
        system("cls");
        std::cout << "\n  Thank you for playing Astro Strike!\n\n";

    } catch (const GameException& e) {
        std::cerr << "\n  " << e.what() << std::endl;
        GameUtils::setCursorVisible(true);
    } catch (const std::exception& e) {
        std::cerr << "\n  Unexpected error: " << e.what() << std::endl;
        GameUtils::setCursorVisible(true);
    }
}

// ============================================================
// Material: Function — Show main menu
// ============================================================
void Game::showMainMenu() {
    system("cls");
    GameUtils::setCursorVisible(true);

    int totalWidth = GameConfig::ARENA_WIDTH + 2; // +2 for borders

    std::cout << "\n";
    std::cout << "  ";
    for (int i = 0; i < totalWidth; ++i) std::cout << "=";
    std::cout << "\n";

    GameUtils::printCentered("ASTRO STRIKE", totalWidth + 2, true);
    GameUtils::printCentered("Terminal Space Shooter", totalWidth + 2, true);

    std::cout << "  ";
    for (int i = 0; i < totalWidth; ++i) std::cout << "=";
    std::cout << "\n\n";

    std::cout << "    [1] Start Game\n";
    std::cout << "    [2] Inventory\n";
    std::cout << "    [3] Shop\n";
    std::cout << "    [4] Trading Market\n";
    std::cout << "    [5] Controls\n";
    std::cout << "    [6] Exit\n";
    std::cout << "\n    Enter your choice: ";
}

// ============================================================
// Material: Function, Exception Handling — Input player name
// ============================================================
void Game::inputPlayerName() {
    system("cls");
    GameUtils::setCursorVisible(true);
    std::cout << "\n";
    std::cout << "  ========================================\n";
    std::cout << "             ENTER PLAYER NAME            \n";
    std::cout << "  ========================================\n\n";
    std::cout << "    Name: ";

    std::string name;
    std::getline(std::cin, name);

    // Material: Exception Handling — validate input
    if (name.empty()) {
        throw InputException("Player name cannot be empty.");
    }

    // Material: Struct & Reference — initialize player
    PlayerModule::initPlayer(player, name);
}

// ============================================================
// Show controls screen
// ============================================================
void Game::showControls() {
    system("cls");
    std::cout << "\n";
    std::cout << "  ========================================\n";
    std::cout << "               GAME CONTROLS              \n";
    std::cout << "  ========================================\n\n";
    std::cout << "    A / Left Arrow   = Move Left\n";
    std::cout << "    D / Right Arrow  = Move Right\n";
    std::cout << "    W / Space        = Shoot\n";
    std::cout << "    P                = Pause / Resume\n";
    std::cout << "    Q                = Quit Game\n\n";
    std::cout << "    Symbols:\n";
    std::cout << "      " << GameConfig::PLAYER_SYMBOL << " = Your spaceship\n";
    std::cout << "      " << GameConfig::BULLET_SYMBOL << " = Bullet\n";
    std::cout << "      " << GameConfig::ENEMY_SYMBOL  << " = Enemy\n";
    std::cout << "      " << GameConfig::BORDER_SYMBOL  << " = Border\n\n";
    std::cout << "    Press any key to return to menu...";
    _getch();
}

// ============================================================
// Materi: Function, Exception Handling (Inventory screen)
// ============================================================
void Game::showInventory() {
    system("cls");

    // Player harus memiliki name sebelum inventory dapat ditampilkan
    if (player.name.empty()) {
        std::cout << "\n  You need to enter a player name first.\n";
        std::cout << "  Name: ";
        std::string name;
        std::getline(std::cin, name);

        if (name.empty()) {
            std::cout << "\n  No name entered. Returning to menu...";
            _getch();
            return;
        }
        PlayerModule::initPlayer(player, name);
    }

    // Material: Exception Handling, loading inventory tidak boleh gagal
    try {
        inventory = InventoryModule::loadInventory(player.name);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
    }

    InventoryModule::displayInventory(inventory);
    std::cout << "    Press any key to return to Main Menu...";
    _getch();
}

// ============================================================
// Materi: Function, Exception Handling (Shop screen)
// ============================================================
void Game::showShop() {
    system("cls");

    // Player harus memiliki name sebelum pembelian dapat ditampilkan
    if (player.name.empty()) {
        std::cout << "\n  You need to enter a player name first.\n";
        std::cout << "  Name: ";
        std::string name;
        std::getline(std::cin, name);

        if (name.empty()) {
            std::cout << "\n  No name entered. Returning to menu...";
            _getch();
            return;
        }
        PlayerModule::initPlayer(player, name);
    }

    // Materi: Exception Handling, load inventory sebelum berbelanja
    try {
        inventory = InventoryModule::loadInventory(player.name);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
    }

    try {
        ShopModule::runShop(player, inventory);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
        std::cout << "    Press any key to continue...";
        _getch();
    }

    // Materi: Exception Handling, simpan inventory setelah berbelanja
    try {
        InventoryModule::saveInventory(player.name, inventory);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
        std::cout << "    Press any key to continue...";
        _getch();
    }
}

// ============================================================
// Material: Function, File Handling — Trading screen
// ============================================================
void Game::showTrading() {
    system("cls");

    // Player must have a name before trading
    if (player.name.empty()) {
        std::cout << "\n  You need to enter a player name first.\n";
        std::cout << "  Name: ";
        std::string name;
        std::getline(std::cin, name);

        if (name.empty()) {
            std::cout << "\n  No name entered. Returning to menu...";
            _getch();
            return;
        }
        PlayerModule::initPlayer(player, name);
    }

    // Material: File Handling — Load wallet from CSV
    try {
        wallet = TradingModule::loadWallet(player.name);
    } catch (const std::exception& e) {
        std::cout << "\n  " << e.what() << "\n";
    }

    // Sync coin from game to wallet (player earns coin from gameplay)
    wallet.coin += player.coin;
    player.coin = 0;

    // Open trading menu
    TradingModule::menuTrading(wallet);

    // After trading, save wallet
    try {
        TradingModule::saveWallet(wallet);
    } catch (const std::exception& e) {
        std::cout << "\n  " << e.what() << "\n";
        std::cout << "    Press any key to continue...";
        _getch();
    }
}

// ============================================================
// Material: Function — Main real-time game loop
// ============================================================
void Game::startGame() {
    isRunning = true;
    isPaused = false;
    frameCounter = 0;
    bullets.clear();
    enemies.clear();

    // Hide cursor for clean rendering
    GameUtils::setCursorVisible(false);

    // Clear screen once before starting
    system("cls");

    // Material: Game Loop Pattern
    while (isRunning) {
        if (!isPaused) {
            handleInput();
            updateBullets();
            updateEnemies();
            spawnEnemy();
            checkCollisions();
            render();
            frameCounter++;
        } else {
            // Still handle input when paused (to unpause or quit)
            handleInput();
        }

        Sleep(GameConfig::GAME_LOOP_DELAY_MS);
    }

    // Show game over screen
    GameUtils::setCursorVisible(true);
    showGameOver();
}

// ============================================================
// Material: Function — Handle real-time keyboard input using conio.h
// ============================================================
void Game::handleInput() {
    // Material: conio.h — _kbhit() and _getch() for real-time input
    if (_kbhit()) {
        char key = _getch();

        // Handle special keys (arrow keys send 0 or 0xE0 prefix)
        if (key == 0 || key == -32) {
            char arrow = _getch();
            switch (arrow) {
                case 75: // Left arrow
                    if (!isPaused) PlayerModule::moveLeft(player);
                    break;
                case 77: // Right arrow
                    if (!isPaused) PlayerModule::moveRight(player);
                    break;
            }
            return;
        }

        switch (key) {
            case 'a': case 'A':
                if (!isPaused) PlayerModule::moveLeft(player);
                break;
            case 'd': case 'D':
                if (!isPaused) PlayerModule::moveRight(player);
                break;
            case 'w': case 'W': case ' ':
                if (!isPaused) {
                    // Spawn bullet above the player
                    BulletModule::spawnBullet(bullets, player.position.x, player.position.y - 1);
                }
                break;
            case 'p': case 'P':
                isPaused = !isPaused;
                if (isPaused) {
                    // Show pause message
                    GameUtils::setCursorPosition(0, GameConfig::ARENA_HEIGHT + 4);
                    std::cout << "  >>> PAUSED - Press P to resume, Q to quit <<<   ";
                }
                break;
            case 'q': case 'Q':
                isRunning = false;
                break;
        }
    }
}

// ============================================================
// Material: STL Vector & Iterator — Update all bullets
// ============================================================
void Game::updateBullets() {
    BulletModule::updateBullets(bullets);
}

// ============================================================
// Material: STL List & Iterator — Update all enemies
// ============================================================
void Game::updateEnemies() {
    // Only move enemies every N frames (slower movement)
    if (frameCounter % GameConfig::ENEMY_MOVE_INTERVAL == 0) {
        EnemyModule::updateEnemies(enemies);
    }
}

// ============================================================
// Material: Function — Spawn enemies periodically
// ============================================================
void Game::spawnEnemy() {
    if (frameCounter % GameConfig::ENEMY_SPAWN_INTERVAL == 0) {
        EnemyModule::spawnEnemy(enemies);
    }
}

// ============================================================
// Material: STL Vector & List, Iterator — Check collisions
// ============================================================
void Game::checkCollisions() {
    // --- Bullet vs Enemy collision ---
    // Material: Iterator — nested iteration with safe erasure
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        bool bulletHit = false;

        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
            if (bulletIt->active && enemyIt->active &&
                bulletIt->position.x == enemyIt->position.x &&
                bulletIt->position.y == enemyIt->position.y) {

                // Bullet hits enemy
                bulletIt->active = false;
                enemyIt->active = false;

                // Material: Default Argument — reward functions
                PlayerModule::addScore(player);  // uses default +10
                PlayerModule::addCoin(player);    // uses default +5
                player.destroyedEnemy++;

                enemyIt = enemies.erase(enemyIt);
                bulletHit = true;
                break; // This bullet can only hit one enemy
            } else {
                ++enemyIt;
            }
        }

        if (bulletHit || !bulletIt->active) {
            bulletIt = bullets.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }

    // --- Enemy vs Player collision ---
    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
        if (enemyIt->active &&
            enemyIt->position.x == player.position.x &&
            enemyIt->position.y == player.position.y) {

            enemyIt->active = false;
            // Material: Default Argument — takeDamage with default 1
            PlayerModule::takeDamage(player);

            enemyIt = enemies.erase(enemyIt);

            // Check game over
            if (!PlayerModule::isAlive(player)) {
                isRunning = false;
                return;
            }
        } else {
            ++enemyIt;
        }
    }

    // --- Enemy reaches bottom (passes player row) ---
    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
        if (enemyIt->active && enemyIt->position.y >= GameConfig::ARENA_HEIGHT) {
            enemyIt = enemies.erase(enemyIt);
        } else {
            ++enemyIt;
        }
    }
}

// ============================================================
// Material: Function — Render the game arena to terminal
// ============================================================
void Game::render() {
    // Move cursor to top-left instead of clearing (reduces flicker)
    GameUtils::clearScreen();

    int arenaW = GameConfig::ARENA_WIDTH;
    int arenaH = GameConfig::ARENA_HEIGHT;

    // --- HUD line ---
    std::cout << "  Score: " << std::setw(5) << player.score
              << " | HP: " << player.health
              << " | Coin: " << std::setw(5) << player.coin
              << " | Destroyed: " << player.destroyedEnemy
              << "        \n\n";

    // --- Top border ---
    std::cout << "  ";
    for (int i = 0; i < arenaW + 2; ++i) std::cout << GameConfig::BORDER_SYMBOL;
    std::cout << "\n";

    // --- Arena rows ---
    for (int y = 0; y < arenaH; ++y) {
        std::cout << "  " << GameConfig::BORDER_SYMBOL;

        for (int x = 0; x < arenaW; ++x) {
            char cell = GameConfig::EMPTY_SYMBOL;

            // Check if player is at this position
            if (x == player.position.x && y == player.position.y) {
                cell = GameConfig::PLAYER_SYMBOL;
            }

            // Check if any bullet is at this position
            // Material: STL Vector iteration
            if (cell == GameConfig::EMPTY_SYMBOL) {
                for (const auto& b : bullets) {
                    if (b.active && b.position.x == x && b.position.y == y) {
                        cell = GameConfig::BULLET_SYMBOL;
                        break;
                    }
                }
            }

            // Check if any enemy is at this position
            // Material: STL List iteration
            if (cell == GameConfig::EMPTY_SYMBOL) {
                for (const auto& e : enemies) {
                    if (e.active && e.position.x == x && e.position.y == y) {
                        cell = GameConfig::ENEMY_SYMBOL;
                        break;
                    }
                }
            }

            std::cout << cell;
        }

        std::cout << GameConfig::BORDER_SYMBOL << "\n";
    }

    // --- Bottom border ---
    std::cout << "  ";
    for (int i = 0; i < arenaW + 2; ++i) std::cout << GameConfig::BORDER_SYMBOL;
    std::cout << "\n";

    // --- Controls hint ---
    std::cout << "  [A/D] Move  [W/Space] Shoot  [P] Pause  [Q] Quit\n";
}

// ============================================================
// Material: Function — Game over screen
// ============================================================
void Game::showGameOver() {
    system("cls");
    int totalWidth = GameConfig::ARENA_WIDTH + 2;

    std::cout << "\n\n";
    std::cout << "  ";
    for (int i = 0; i < totalWidth; ++i) std::cout << "=";
    std::cout << "\n";

    GameUtils::printCentered("GAME OVER", totalWidth + 2, true);

    std::cout << "  ";
    for (int i = 0; i < totalWidth; ++i) std::cout << "=";
    std::cout << "\n\n";

    std::cout << "    Player:          " << player.name << "\n";
    std::cout << "    Final Score:     " << player.score << "\n";
    std::cout << "    Coin Earned:     " << player.coin << "\n";
    std::cout << "    Enemies Destroyed: " << player.destroyedEnemy << "\n\n";

    std::cout << "  ";
    for (int i = 0; i < totalWidth; ++i) std::cout << "-";
    std::cout << "\n\n";

    std::cout << "    Press any key to return to Main Menu...";
    _getch();
}
