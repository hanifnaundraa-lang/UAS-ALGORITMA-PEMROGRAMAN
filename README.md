# Astro Strike

**Real-Time Terminal Space Shooter with Shop, Trading, Gacha, Inventory, and Leaderboard**

Astro Strike is a final project for Algorithm and Programming course. It is a real-time terminal-based space shooter game built with C++17.

## How to Build

Requires: CMake 3.16+, C++17 compiler (MSVC/MinGW on Windows)

```bash
cmake -S . -B build
cmake --build build
```

## How to Run

After building, run the executable from the project root directory:

```bash
# If using MSVC (Visual Studio)
.\build\Debug\AstroStrike.exe

# If using MinGW
.\build\AstroStrike.exe
```

**Important:** Run from the project root directory so the `data/` folder with CSV files is accessible.

## Game Controls

| Key | Action |
|-----|--------|
| A | Move left |
| D | Move right |
| W / Space | Shoot |
| P | Pause / Resume |
| Q | Quit current game |

## Project Structure

```text
AstroStrike/
├── AGENTS.md           # AI assistant instructions
├── README.md           # This file
├── CMakeLists.txt      # Build configuration
├── .gitignore
├── docs/               # Documentation
│   ├── PRD.md
│   ├── laporan.docx
│   └── script-demo.md
├── data/               # CSV data files
│   ├── leaderboard.csv
│   ├── inventory.csv
│   ├── wallet.csv
│   ├── shop_items.csv
│   ├── gacha_pool.csv
│   ├── market_history.csv
│   └── trade_history.csv
├── src/                # Source code
│   ├── main.cpp
│   ├── game.h / game.cpp
│   ├── player.h / player.cpp
│   ├── bullet.h / bullet.cpp
│   ├── enemy.h / enemy.cpp
│   ├── gamedata.h
│   ├── gameconfig.h
│   ├── gameutils.h
│   ├── gameexception.h
│   ├── scoremanager.h / scoremanager.cpp
│   ├── inventory.h / inventory.cpp
│   ├── shop.h / shop.cpp
│   ├── trading.h / trading.cpp
│   └── gacha.h / gacha.cpp
└── tests/
```

## Team Division

| Member | Module |
|--------|--------|
| Member 1 | Game Core (implemented) |
| Member 2 | Leaderboard + Inventory (placeholder) |
| Member 3 | Shop (placeholder) |
| Member 4 | Trading (placeholder) |
| Member 5 | Gacha (placeholder) |

## Technology Stack

- C++17
- CMake
- Windows Terminal
- `conio.h` for real-time input
- `windows.h` for timing and console control
- CSV files for data persistence
