# PRD — Astro Strike Revised Terminal

## 1. Product Name

**Astro Strike**

## 2. Product Type

Astro Strike is a Final Project for an Algorithm and Programming course.

The project is a **real-time terminal-based space shooter game** built with C++. It combines a playable terminal game with data-management features such as inventory, shop, trading, gacha, and leaderboard.

---

## 3. Revised Project Concept

Astro Strike is a real-time terminal space shooter where the player controls a spaceship, moves left and right, shoots enemies, earns score and coin, and survives until health reaches zero.

The game uses terminal rendering with ASCII characters. Keyboard input is read in real time using `conio.h`, so the player does not need to press Enter for movement or shooting.

In addition to the main shooting gameplay, Astro Strike includes:

- Inventory system
- Shop system
- Trading market simulation
- Gacha system
- Leaderboard system
- CSV-based data persistence

Qt/QML is not part of the main implementation anymore. It may be mentioned only as optional future development.

---

## 4. Background

The original idea was a Qt/QML space shooter. After project scope revision, the main implementation changed into a real-time terminal game to make the project more realistic for a two-week deadline and easier to align with the Algorithm and Programming course materials.

The terminal approach makes the C++ logic more visible and easier to explain in the report, especially for materials such as struct, pointer, reference, STL vector/list, iterator, sort, find, count, file handling, exception handling, and lambda expression.

---

## 5. Goals

The goals of Astro Strike are:

1. Build a playable real-time terminal space shooter game.
2. Implement player movement, shooting, enemy spawning, collision, score, health, and game over.
3. Add coin rewards from the game core.
4. Use coin in shop, trading, and gacha systems.
5. Store leaderboard, inventory, wallet, market, and transaction data using CSV files.
6. Apply more than 70% of the Algorithm and Programming course materials.
7. Create a project that can be presented through source code, report, and video demo.

---

## 6. Technology Stack

| Area | Technology |
|---|---|
| Language | C++ |
| Standard | C++17 |
| Main Platform | Windows Terminal |
| Build System | CMake |
| Input | `conio.h`, `_kbhit()`, `_getch()` |
| Timing | `windows.h`, `Sleep()` |
| Data Storage | CSV files |
| UI | Terminal / ASCII |
| Optional Future UI | Qt/QML |

---

## 7. Main Features

### 7.1 Main Menu

The main menu provides access to the main systems.

Required menu options:

1. Start Game
2. Inventory
3. Shop
4. Trading Market
5. Gacha
6. Leaderboard
7. Exit

---

### 7.2 Input Player Name

Before playing, the user enters a player name.

The player name is used for:

- Leaderboard
- Inventory
- Wallet
- Trading history
- Gacha or shop data if needed

---

### 7.3 Real-Time Terminal Gameplay

The main gameplay is a real-time terminal space shooter.

Required mechanics:

- Player appears at the bottom of the arena.
- Player can move left and right.
- Player can shoot bullets upward.
- Enemies spawn from the top.
- Enemies move downward.
- Bullets move upward.
- Collision is checked every frame.
- Player earns score and coin by destroying enemies.
- Player loses health when hit by enemies.
- Game over occurs when health reaches zero.

---

### 7.4 Player Movement

Controls:

- `A` = move left
- `D` = move right

Rules:

- Player cannot move beyond the left boundary.
- Player cannot move beyond the right boundary.

---

### 7.5 Shooting System

Controls:

- `W` or Space = shoot

Rules:

- A bullet appears above the player.
- Bullet moves upward automatically every game loop.
- Bullet disappears when it exits the arena or hits an enemy.

---

### 7.6 Enemy System

Rules:

- Enemies spawn periodically at random x positions.
- Enemies start from the top of the arena.
- Enemies move downward.
- Enemies disappear when hit by bullet.
- Enemies reduce player health when they collide with the player.

---

### 7.7 Collision Detection

Required collision checks:

1. Bullet vs Enemy
2. Enemy vs Player

Bullet vs Enemy result:

- Bullet removed
- Enemy removed
- Score increases
- Coin increases
- Destroyed enemy count increases

Enemy vs Player result:

- Enemy removed
- Player health decreases
- Game over if health becomes zero

---

### 7.8 Score System

Score increases when enemies are destroyed.

Suggested scoring:

- Normal enemy destroyed: +10 score
- Optional stronger enemy: +20 or +50 score

The score is saved to leaderboard after game over.

---

### 7.9 Coin Reward System

Coin is earned from the main gameplay.

Suggested coin reward:

- Normal enemy destroyed: +5 coin
- Optional stronger enemy: +10 or more coin

Coin is used for:

- Shop
- Trading
- Gacha

---

### 7.10 Health System

The player starts with initial health.

Suggested default:

- Initial health: 3

Health decreases when enemy hits the player.

Game over occurs when health reaches zero.

---

### 7.11 Game Over System

When game over occurs, the game displays:

- Player name
- Final score
- Coin earned
- Destroyed enemy count
- Option to return to main menu

After game over, score data must be saved to leaderboard CSV.

---

## 8. Inventory System

The inventory stores items owned by the player.

The inventory is used by:

- Shop
- Trading
- Gacha
- Optional gameplay effects

Required features:

1. Add item
2. Remove item
3. Check item quantity
4. Display inventory
5. Save inventory to CSV
6. Load inventory from CSV

Required file:

```text
data/inventory.csv
```

Required CSV header:

```csv
playerName,itemId,itemName,type,quantity
```

---

## 9. Shop System

The shop allows the player to buy items using coin.

Required shop items:

1. Health Potion
2. Shield
3. Double Bullet
4. Damage Boost
5. Gacha Ticket

Required features:

1. Display shop items
2. Buy item
3. Validate player coin
4. Deduct coin after purchase
5. Add purchased item to inventory

Required file:

```text
data/shop_items.csv
```

Required CSV header:

```csv
id,name,type,price,effect
```

---

## 10. Trading Market System

The trading feature is a game simulation market. It is not connected to real cryptocurrency prices and does not involve real money.

Required assets:

- BTC as required asset
- ETH as optional asset

Required player balance:

- Coin
- BTC
- ETH optional

Required features:

1. Display current market day
2. Display player wallet
3. Display BTC price
4. Display ETH price if implemented
5. Buy BTC using coin
6. Sell BTC to receive coin
7. Generate next day price randomly
8. Save wallet
9. Save market history
10. Save trade history

Recommended rule:

- BTC price changes randomly each in-game day.
- ETH may be implemented if time allows.
- Price should not be completely unrealistic; use controlled random change.

Required files:

```text
data/wallet.csv
data/market_history.csv
data/trade_history.csv
```

Required CSV headers:

```csv
wallet.csv:
playerName,coin,btc,eth
```

```csv
market_history.csv:
day,btcPrice,ethPrice
```

```csv
trade_history.csv:
day,playerName,type,asset,amount,price,total
```

---

## 11. Gacha System

The gacha system lets the player roll for random rewards.

The player can roll using:

- Coin
- Gacha Ticket

Required features:

1. Display gacha pool
2. Roll reward
3. Determine reward based on rarity
4. Add reward to inventory
5. Deduct coin or ticket after roll
6. Handle insufficient coin or ticket

Recommended rarity:

- Common
- Rare
- Epic
- Legendary

Required file:

```text
data/gacha_pool.csv
```

Required CSV header:

```csv
id,name,type,rarity,dropRate
```

---

## 12. Leaderboard System

The leaderboard stores player game results.

Required features:

1. Save score after game over
2. Load leaderboard from CSV
3. Display leaderboard
4. Sort leaderboard by highest score
5. Search player score by name
6. Count how many times a player has played

Required file:

```text
data/leaderboard.csv
```

Required CSV header:

```csv
name,score,destroyedEnemy,dateTime
```

---

## 13. Gameplay Flow

Main flow:

```text
Open Program
↓
Main Menu
↓
Input Player Name
↓
Start Game
↓
Player moves and shoots in terminal
↓
Enemy spawns and moves downward
↓
Collision is checked
↓
Score and coin increase when enemy is destroyed
↓
Health decreases when enemy hits player
↓
Game Over when health reaches 0
↓
Save score to leaderboard.csv
↓
Return to Main Menu
```

Additional system flow:

```text
Main Menu
↓
Inventory / Shop / Trading / Gacha / Leaderboard
↓
Read or write CSV data
↓
Return to Main Menu
```

---

## 14. Controls

| Key | Action |
|---|---|
| A | Move left |
| D | Move right |
| W | Shoot |
| Space | Shoot |
| P | Pause / Resume |
| Q | Quit current game |

---

## 15. ASCII Rendering Design

Suggested symbols:

| Symbol | Meaning |
|---|---|
| `A` | Player spaceship |
| `|` | Bullet |
| `V` | Enemy |
| `.` | Empty space |
| `#` | Border |

Example:

```text
Score: 120 | HP: 3 | Coin: 50

##############################
#............V...............#
#............................#
#..............|.............#
#............................#
#..............A.............#
##############################
```

---

## 16. System Modules

### 16.1 Game Core

Files:

- `game.h`
- `game.cpp`
- `player.h`
- `player.cpp`
- `bullet.h`
- `bullet.cpp`
- `enemy.h`
- `enemy.cpp`

Responsibilities:

- Real-time game loop
- Input handling
- Rendering
- Movement
- Shooting
- Enemy spawning
- Collision
- Score
- Health
- Coin reward
- Game over

### 16.2 Shared Data and Utilities

Files:

- `gamedata.h`
- `gameconfig.h`
- `gameutils.h`
- `gameexception.h`

Responsibilities:

- Shared structs
- Constants
- Helper functions
- Custom exception classes

### 16.3 Inventory

Files:

- `inventory.h`
- `inventory.cpp`

Responsibilities:

- Add, remove, search, count, display inventory item
- Save/load inventory CSV

### 16.4 Shop

Files:

- `shop.h`
- `shop.cpp`

Responsibilities:

- Display shop items
- Buy item
- Validate coin
- Add item to inventory

### 16.5 Trading

Files:

- `trading.h`
- `trading.cpp`

Responsibilities:

- Wallet
- BTC price
- ETH optional
- Buy/sell asset
- Next day price
- Save/load wallet and history

### 16.6 Gacha

Files:

- `gacha.h`
- `gacha.cpp`

Responsibilities:

- Gacha pool
- Roll reward
- Rarity logic
- Add reward to inventory

### 16.7 Leaderboard

Files:

- `scoremanager.h`
- `scoremanager.cpp`

Responsibilities:

- Save score
- Load leaderboard
- Sort score
- Search player
- Count player history

---

## 17. Recommended Folder Structure

```text
AstroStrike/
├── AGENTS.md
├── README.md
├── CMakeLists.txt
├── .gitignore
│
├── docs/
│   ├── PRD.md
│   ├── laporan.docx
│   └── script-demo.md
│
├── data/
│   ├── leaderboard.csv
│   ├── inventory.csv
│   ├── wallet.csv
│   ├── shop_items.csv
│   ├── gacha_pool.csv
│   ├── market_history.csv
│   └── trade_history.csv
│
├── src/
│   ├── main.cpp
│   ├── game.h
│   ├── game.cpp
│   ├── player.h
│   ├── player.cpp
│   ├── bullet.h
│   ├── bullet.cpp
│   ├── enemy.h
│   ├── enemy.cpp
│   ├── inventory.h
│   ├── inventory.cpp
│   ├── shop.h
│   ├── shop.cpp
│   ├── trading.h
│   ├── trading.cpp
│   ├── gacha.h
│   ├── gacha.cpp
│   ├── scoremanager.h
│   ├── scoremanager.cpp
│   ├── gamedata.h
│   ├── gameconfig.h
│   ├── gameutils.h
│   └── gameexception.h
│
└── tests/
```

---

## 18. Course Material Mapping

| Course Material | Implementation in Astro Strike |
|---|---|
| Struct, Pointer & Reference | `Player`, `Bullet`, `Enemy`, `Item`, `Wallet`, `PlayerScore`; references used to update wallet, inventory, and game stats |
| Namespace | `GameConfig`, `FileConfig`, `GameUtils` |
| Function | Game functions, shop functions, trading functions, gacha functions, leaderboard functions |
| Default Argument & Inline Function | Default score/coin values, helper calculations, position boundary check |
| Function Overloading & Function Template | Overloaded buy/add functions, template helper such as clamp or random range |
| Exception Handling | File errors, invalid input, insufficient coin/item/BTC |
| STL Vector & List | Vector for bullets, inventory, scores, shop items, gacha pool; list for enemies |
| Iterator | Updating bullets and enemies; traversing inventory and scores |
| Sort, Find, Count | Leaderboard sorting, player search, item search, transaction count |
| File Handling & CSV Processing | Leaderboard, inventory, wallet, shop items, gacha pool, market history, trade history |
| Lambda Expression | Sorting, filtering, searching, counting |

---

## 19. Team Division

| Member | Main Responsibility |
|---|---|
| Member 1 | Game Core |
| Member 2 | Leaderboard + Inventory |
| Member 3 | Shop |
| Member 4 | Trading |
| Member 5 | Gacha |

---

## 20. Timeline

### Week 1

| Day | Target |
|---|---|
| Day 1 | Project setup, CMake, folder structure, shared data structs |
| Day 2 | Game loop, terminal render, real-time input |
| Day 3 | Player movement and shooting |
| Day 4 | Bullet movement, enemy spawn, enemy movement |
| Day 5 | Collision, score, health, coin reward |
| Day 6 | Game over, return to menu, basic testing |
| Day 7 | Stabilize game core |

### Week 2

| Day | Target |
|---|---|
| Day 8 | Inventory system |
| Day 9 | Leaderboard CSV |
| Day 10 | Shop system |
| Day 11 | Trading system |
| Day 12 | Gacha system |
| Day 13 | Integration, testing, bug fixing |
| Day 14 | Report, video demo, final check |

---

## 21. Acceptance Criteria

The project is considered complete if:

1. The program builds using CMake.
2. The program runs in Windows terminal.
3. Main menu works.
4. Player can move left and right without pressing Enter.
5. Player can shoot.
6. Bullets move upward automatically.
7. Enemies spawn and move downward automatically.
8. Collision bullet vs enemy works.
9. Collision enemy vs player works.
10. Score increases when enemy is destroyed.
11. Coin increases when enemy is destroyed.
12. Health decreases when enemy hits player.
13. Game over works.
14. Score is saved to leaderboard CSV.
15. Leaderboard can be displayed.
16. Leaderboard can be sorted.
17. Player score can be searched.
18. Player game history can be counted.
19. Inventory can save and load data.
20. Shop can buy items using coin.
21. Trading can buy/sell BTC using coin.
22. Gacha can roll rewards using coin or ticket.
23. Required course materials are visible in the code.
24. The project can be demonstrated in a video.

---

## 22. Optional Features

Optional features if time allows:

1. ETH trading
2. Boss enemy
3. Stronger enemy types
4. More shop items
5. Gacha history
6. Top 5 leaderboard display
7. Reset leaderboard
8. Difficulty level
9. Achievement system
10. Qt/QML visual version as future development

---

## 23. Out of Scope

The following are not required:

1. Real crypto API
2. Real money transaction
3. Online trading
4. Multiplayer
5. Database integration
6. Mobile app
7. Full Qt/QML implementation
8. Complex graphics
9. Sound effects

---

## 24. Main Risk

| Risk | Mitigation |
|---|---|
| `conio.h` only works well on Windows | State Windows terminal as the target platform |
| Game core is too heavy for one member | Keep game core simple and playable first |
| Shop/trading/gacha depends on inventory | Build inventory early |
| CSV files missing or empty | Create files automatically with headers |
| Scope becomes too large | Prioritize game core and required systems only |

---

## 25. Final Scope Summary

The final project scope is:

**A real-time terminal C++ space shooter with player movement, shooting, enemies, collision, score, health, coin, inventory, shop, trading, gacha, leaderboard, and CSV persistence.**

The main priority is to make the terminal game playable first, then integrate data systems step by step.
