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
        char choice;
        while (appRunning) {
            showMainMenu();

            choice = _getch();
            std::cout << choice << "\n";

            switch (choice) {
                case '1':
                    if (player.name.empty()) {
                        inputPlayerName();
                    } else {
                        PlayerModule::initPlayer(player, player.name); // Reset stats for new run
                    }
                    showLoadoutAndStart();
                    break;
                case '2':
                    showInventory();
                    break;
                case '3':
                    showShop();
                    break;
                case '4':
                    showTrading();
                    break;
                case '5':
                    showLeaderboard();
                    break;
                case '6':
                    showGacha();
                    break;
                case '7':
                    showControls();
                    break;
                case '0':
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
    std::cout << "    [5] Leaderboard\n";
    std::cout << "    [6] Gacha\n";
    std::cout << "    [7] Guide\n";
    std::cout << "    [0] Exit\n";
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
// Material: Function — Pre-game Loadout Menu (Buff Limit System)
// ============================================================
void Game::showLoadoutAndStart() {
    try {
        inventory = InventoryModule::loadInventory(player.name);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
    }

    std::vector<Item> originalInventory = inventory; // Keep backup for unequip feature
    int buffSlotsUsed = 0;
    bool readyToStart = false;

    while (!readyToStart) {
        system("cls");
        std::cout << "  ========================================\n";
        std::cout << "                  LOADOUT                 \n";
        std::cout << "  ========================================\n\n";
        
        std::cout << "  Inventory:\n";
        int hpQty = InventoryModule::countItem(inventory, 1);
        int shQty = InventoryModule::countItem(inventory, 2);
        int dbQty = InventoryModule::countItem(inventory, 3);
        int sbQty = InventoryModule::countItem(inventory, 4);
        int cbQty = InventoryModule::countItem(inventory, 6);
        int empQty = InventoryModule::countItem(inventory, 7);

        std::cout << "  1. Health Potion x" << hpQty << "\n";
        std::cout << "  2. Shield        x" << shQty << "\n";
        std::cout << "  3. Double Bullet x" << dbQty << "\n";
        std::cout << "  4. Score Booster x" << sbQty << "\n";
        std::cout << "  5. Coin Booster  x" << cbQty << "\n";
        std::cout << "  6. EMP Device    x" << empQty << "\n\n";

        std::cout << "  Current Loadout:\n";
        std::cout << "  Health Bonus : +" << player.loadout.healthPotionUsed << "\n";
        std::cout << "  Shield       : " << (player.loadout.shieldActive ? "ON" : "OFF") << "\n";
        std::cout << "  DoubleBullet : " << (player.loadout.doubleBulletActive ? "ON" : "OFF") << "\n";
        std::cout << "  Score Boost  : " << (player.loadout.scoreBoostActive ? "ON" : "OFF") << "\n";
        std::cout << "  Coin Boost   : " << (player.loadout.coinBoostActive ? "ON" : "OFF") << "\n";
        std::cout << "  EMP Device   : " << (player.loadout.empEquipped ? "ON" : "OFF") << "\n";
        std::cout << "  Buff Slots Used: " << buffSlotsUsed << " / 2\n\n";

        std::cout << "  Options:\n";
        std::cout << "  [1] Use Health Potion\n";
        std::cout << "  [2] Equip Shield\n";
        std::cout << "  [3] Equip Double Bullet\n";
        std::cout << "  [4] Equip Score Booster\n";
        std::cout << "  [5] Equip Coin Booster\n";
        std::cout << "  [6] Equip EMP Device\n";
        std::cout << "  [7] Start Mission\n";
        std::cout << "  [0] Cancel & Back to Menu\n\n";

        std::cout << "  Your choice: ";
        char choice = _getch();
        std::cout << choice << "\n\n";

        if (choice == '0') {
            inventory = InventoryModule::loadInventory(player.name); // Reload to discard unsaved consumption
            return;
        } else if (choice == '7') {
            readyToStart = true;
        } else if (choice == '1') {
            if (hpQty > 0) {
                if (player.loadout.healthPotionUsed < 2) { // Cap at 5 HP (Base 3 + 2 Potions)
                    InventoryModule::removeItem(inventory, 1);
                    player.loadout.healthPotionUsed++;
                } else {
                    std::cout << "  Max Health reached! (Cap at 5 HP). Press any key..."; _getch();
                }
            } else {
                std::cout << "  Not enough Health Potions! Press any key..."; _getch();
            }
        } else if (choice >= '2' && choice <= '6') {
            int itemId = 0;
            bool* activeFlag = nullptr;
            std::string itemName = "";
            
            if (choice == '2') { itemId = 2; activeFlag = &player.loadout.shieldActive; itemName = "Shield"; }
            else if (choice == '3') { itemId = 3; activeFlag = &player.loadout.doubleBulletActive; itemName = "Double Bullet"; }
            else if (choice == '4') { itemId = 4; activeFlag = &player.loadout.scoreBoostActive; itemName = "Score Booster"; }
            else if (choice == '5') { itemId = 6; activeFlag = &player.loadout.coinBoostActive; itemName = "Coin Booster"; }
            else if (choice == '6') { itemId = 7; activeFlag = &player.loadout.empEquipped; itemName = "EMP Device"; }

            if (*activeFlag) {
                std::cout << "  " << itemName << " is already equipped. Unequip? (y/n): ";
                char unequipChoice = _getch();
                std::cout << unequipChoice << "\n";
                if (unequipChoice == 'y' || unequipChoice == 'Y') {
                    *activeFlag = false;
                    buffSlotsUsed--;
                    
                    Item* original = InventoryModule::findItem(originalInventory, itemId);
                    if (original) {
                        Item toReturn = *original;
                        toReturn.quantity = 1;
                        InventoryModule::addItem(inventory, toReturn);
                        std::cout << "  " << itemName << " unequipped.\n";
                    }
                    std::cout << "  Press any key..."; _getch();
                }
            } else {
                int qty = InventoryModule::countItem(inventory, itemId);
                if (qty > 0) {
                    if (buffSlotsUsed < 2) {
                        InventoryModule::removeItem(inventory, itemId);
                        *activeFlag = true;
                        buffSlotsUsed++;
                    } else {
                        std::cout << "  Buff Limit Reached! Maximum 2 buffs allowed. Press any key..."; _getch();
                    }
                } else {
                    std::cout << "  Not enough " << itemName << "! Press any key..."; _getch();
                }
            }
        }
    }

    // Apply Health
    player.health += player.loadout.healthPotionUsed;

    // Apply EMP Charges
    if (player.loadout.empEquipped) {
        player.loadout.empCharges = 2; // Grant 2 charges for the mission
        player.loadout.empTimer = 0;
    }

    // Save inventory permanently
    try {
        InventoryModule::saveInventory(player.name, inventory);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
        std::cout << "  Mission Aborted due to save failure. Press any key..."; _getch();
        return; // Abort mission
    }

    // Start the real-time loop
    startGame();
}

// ============================================================
// Show controls screen
// ============================================================
void Game::showControls() {
    system("cls");
    std::cout << "\n";
    std::cout << "  ========================================\n";
    std::cout << "               PLAYING GUIDE               \n";
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

    std::cout << "\n";
    std::cout << "  =============================================================\n";
    std::cout << "                         ITEM SHOP GUIDE                       \n";
    std::cout << "  =============================================================\n\n";
    std::cout << "    Loadout Rules:\n";
    std::cout << "      - You can bring MAX 2 active buffs per mission.\n";
    std::cout << "      - Health Potions do not consume buff slots.\n\n";
    std::cout << "    Available Items:\n";
    std::cout << "      [1] Health Potion : Max +2 HP per run (Cap at 5 HP)\n";
    std::cout << "      [2] Shield        : Blocks 1 hit from an enemy\n";
    std::cout << "      [3] Double Bullet : Fires 2 parallel side-projectiles\n";
    std::cout << "      [4] Score Booster : Doubles points earned from kills\n";
    std::cout << "      [6] Coin Booster  : Doubles coins dropped by enemies\n";
    std::cout << "      [7] EMP Device    : Grants 2 active charges [Key: E]\n";
    std::cout << "                          Triggers 3 sec of Slow Motion\n\n";
    
    std::cout << "\n";
    std::cout << "  =============================================================\n";
    std::cout << "                      TRADING MARKET GUIDE                     \n";
    std::cout << "  =============================================================\n\n";
    std::cout << "    Market Mechanics:\n";
    std::cout << "      - Use your hard-earned gameplay coins to trade BTC.\n";
    std::cout << "      - BTC prices fluctuate dynamically every calendar day.\n";
    std::cout << "      - Price Range: 30,000 to 70,000 coins per BTC.\n\n";
    std::cout << "    Trading Strategies:\n";
    std::cout << "      - Buy Low  : Accumulate BTC when price trends near 30k.\n";
    std::cout << "      - Sell High: Liquidate BTC when price approaches 70k.\n";
    std::cout << "      - History  : Check Option [5] to audit your trade logs.\n\n";
    std::cout << "    * Note: Fractional BTC buys (e.g., 0.1) are fully supported.\n\n";
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

    // Feature 6 & 7: Interactive inventory menu with search and filter
    InventoryModule::runInventory(inventory, player.name);

    // Save inventory setelah keluar dari menu
    try {
        InventoryModule::saveInventory(player.name, inventory);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
    }
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

    // Feature 1: Load wallet — Wallet is single source of truth for coin
    try {
        wallet = TradingModule::loadWallet(player.name);
    } catch (const std::exception& e) {
        std::cout << "\n  " << e.what() << "\n";
    }

    // Sync coin from gameplay to wallet
    wallet.coin += player.coin;
    player.coin = 0;

    // Materi: Exception Handling, load inventory sebelum berbelanja
    try {
        inventory = InventoryModule::loadInventory(player.name);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
    }

    // Feature 1: Shop now uses Wallet& instead of Player&
    try {
        ShopModule::runShop(wallet, inventory);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
        std::cout << "    Press any key to continue...";
        _getch();
    }

    // Save wallet setelah berbelanja
    try {
        TradingModule::saveWallet(wallet);
    } catch (const std::exception& e) {
        std::cout << "\n  " << e.what() << "\n";
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
// Materi: Function, Exception Handling (Leaderboard screen)
// ============================================================
void Game::showLeaderboard() {
    system("cls");

    // Materi: Exception Handling, loading leaderboard data safely
    try {
        ScoreManager::runLeaderboardMenu();
    } catch (const std::exception& e) {
        std::cout << "\n  An error occurred while loading the leaderboard:\n";
        std::cout << "  " << e.what() << "\n";
    }
}

// ============================================================
// Material: Function, Exception Handling — Gacha screen
// ============================================================
void Game::showGacha() {
    system("cls");

    // Player must have a name before using gacha
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

    // Material: Exception Handling — load inventory before gacha
    try {
        inventory = InventoryModule::loadInventory(player.name);
    } catch (const FileException& e) {
        std::cout << "\n  " << e.what() << "\n";
    }

    // Open gacha menu
    GachaModule::runGachaMenu(inventory, player.name);

    // Save inventory after gacha (already saved inside, but ensure consistency)
    try {
        InventoryModule::saveInventory(player.name, inventory);
    } catch (const FileException& e) {
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
            // Update EMP Timer
            if (player.loadout.empTimer > 0) {
                player.loadout.empTimer--;
            }

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
                    if (player.loadout.doubleBulletActive) {
                        int leftX = player.position.x - 1;
                        int rightX = player.position.x + 1;
                        // spawn left bullet
                        if (leftX >= 0) BulletModule::spawnBullet(bullets, leftX, player.position.y - 1);
                        else BulletModule::spawnBullet(bullets, rightX + 1, player.position.y - 1);
                        
                        // spawn right bullet
                        if (rightX < GameConfig::ARENA_WIDTH) BulletModule::spawnBullet(bullets, rightX, player.position.y - 1);
                        else BulletModule::spawnBullet(bullets, leftX - 1, player.position.y - 1);
                    } else {
                        BulletModule::spawnBullet(bullets, player.position.x, player.position.y - 1);
                    }
                }
                break;
            case 'e': case 'E':
                if (!isPaused && player.loadout.empEquipped && player.loadout.empCharges > 0 && player.loadout.empTimer == 0) {
                    player.loadout.empCharges--;
                    player.loadout.empTimer = 100; // ~3 seconds of slow motion
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
    // Only move enemies every N frames (slower movement during EMP)
    int enemySpeed = (player.loadout.empTimer > 0) ? 9 : GameConfig::ENEMY_MOVE_INTERVAL;
    if (frameCounter % enemySpeed == 0) {
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
                (bulletIt->position.y == enemyIt->position.y || 
                 bulletIt->position.y == enemyIt->position.y - 1)) {

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
              << "        \n";

    if (player.loadout.empEquipped) {
        std::cout << "  EMP Charges: " << player.loadout.empCharges << " / 2";
        if (player.loadout.empTimer > 0) std::cout << "   [SLOW MOTION ACTIVE]";
        std::cout << "                \n";
    }
    std::cout << "\n";

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
    std::cout << "  [A/D] Move  [W/Space] Shoot  [E] EMP  [P] Pause  [Q] Quit\n";
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

    std::cout << "    Player            : " << player.name << "\n";
    std::cout << "    Final Score       : " << player.score << "\n";
    std::cout << "    Coin Earned       : " << player.coin << "\n";
    std::cout << "    Enemies Destroyed : " << player.destroyedEnemy << "\n\n";

    std::cout << "  ";
    for (int i = 0; i < totalWidth; ++i) std::cout << "-";
    std::cout << "\n\n";

    if (!player.name.empty() && player.score > 0) {
        try {
            PlayerScore currentRun; 
            currentRun.name = player.name;
            currentRun.score = player.score;
            currentRun.destroyedEnemy = player.destroyedEnemy;

            // Get current system time
            std::time_t now = std::time(nullptr);
            std::tm* ltm = std::localtime(&now);

            // Format the time into dd/mm/yyyy using a stringstream
            std::stringstream ss;
            ss << std::setfill('0') 
               << std::setw(2) << ltm->tm_mday << "/"
               << std::setw(2) << (ltm->tm_mon + 1) << "/"
               << (ltm->tm_year + 1900);

            currentRun.dateTime = ss.str();

            ScoreManager::saveScore(currentRun);
        } catch (const std::exception& e) {
            std::cerr << "Failed to save score: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "An unknown error occurred while saving the score." << std::endl;
        }
    }

    std::cout << "    Press any key to return to Main Menu...";
    _getch();
}
