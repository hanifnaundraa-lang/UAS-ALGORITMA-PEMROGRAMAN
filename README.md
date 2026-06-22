# 🚀 Astro Strike

Astro Strike adalah game bertema **Space Shooter** berbasis terminal yang dikembangkan menggunakan bahasa pemrograman **C++17** sebagai proyek akhir mata kuliah **Algoritma Pemrograman**.

Dalam permainan ini, pemain mengendalikan sebuah pesawat luar angkasa untuk menghancurkan musuh, mengumpulkan koin, meningkatkan kemampuan melalui item, melakukan trading aset virtual, menggunakan sistem gacha, serta bersaing dengan pemain lain melalui leaderboard.

Project ini tidak hanya berfokus pada gameplay, tetapi juga mengimplementasikan berbagai konsep penting dalam Algoritma Pemrograman seperti STL, File Handling, Lambda Expression, Sorting, Searching, dan Exception Handling.

---

# 🎮 Fitur Utama

## 1. Game Core

* Pergerakan pesawat secara real-time
* Sistem tembak (shooting)
* Enemy spawning
* Collision detection
* Health system
* Score system
* Coin reward system
* Game Over system

## 2. Inventory System

* Penyimpanan item pemain
* Quantity item
* Search item
* Filter item
* Save & Load inventory

## 3. Shop System

* Pembelian item
* Stock management
* Quantity purchase
* Purchase history
* Item catalog

## 4. Trading System

* Wallet management
* Coin currency
* BTC trading simulation
* Market price fluctuation
* Trade history

## 5. Leaderboard System

* Rank display
* Search player
* Personal best
* Statistics menu
* Hall of Fame

## 6. Gacha System

* Single pull
* Multi pull
* Reward rarity system
* Gacha history
* Gacha statistics
* Inventory integration

## 7. Loadout System

* Persiapan item sebelum permainan dimulai
* Penggunaan item dari inventory
* Integrasi dengan Game Core

---

# 🛠 Teknologi yang Digunakan

* C++17
* STL (Standard Template Library)
* File Handling (CSV)
* Object-Oriented Programming (OOP)
* CMake
* Visual Studio Code
* Git & GitHub

---

# 📚 Materi Algoritma Pemrograman yang Diimplementasikan

Project ini mengimplementasikan materi-materi berikut:

* Pointer & Reference
* Namespace
* Function
* Inline Function
* Function Overloading
* Exception Handling
* STL Vector
* STL List
* Iterator
* Sort
* Find
* Count
* File Handling
* Lambda Expression

---

# 📁 Struktur Project

```text
AstroStrike/
│
├── src/
│   ├── game.cpp
│   ├── inventory.cpp
│   ├── shop.cpp
│   ├── trading.cpp
│   ├── leaderboard.cpp
│   ├── gacha.cpp
│   └── main.cpp
│
├── include/
│   ├── game.h
│   ├── inventory.h
│   ├── shop.h
│   ├── trading.h
│   ├── leaderboard.h
│   └── gacha.h
│
├── data/
│   ├── inventory.csv
│   ├── wallet.csv
│   ├── leaderboard.csv
│   ├── gacha_history.csv
│   ├── trade_history.csv
│   ├── market_history.csv
│   └── shop_history.csv
│
├── docs/
│   ├── PRD.md
│   └── AGENTS.md
│
├── README.md
└── CMakeLists.txt
```

---

# ▶ Cara Menjalankan Program

## Menggunakan CMake

```bash
mkdir build
cd build

cmake ..
cmake --build .

./AstroStrike
```

## Menggunakan Compiler C++

```bash
g++ -std=c++17 src/*.cpp -Iinclude -o AstroStrike

./AstroStrike
```

---

# 💾 Penyimpanan Data

Astro Strike menggunakan file CSV untuk menyimpan data permainan secara permanen.

Data yang disimpan meliputi:

* Inventory pemain
* Wallet pemain
* Riwayat trading
* Riwayat gacha
* Leaderboard
* Statistik permainan

Dengan sistem ini, data tetap tersedia meskipun program ditutup dan dijalankan kembali.

---

# 🎯 Tujuan Project

Tujuan utama dari pengembangan Astro Strike adalah:

1. Menerapkan konsep Algoritma Pemrograman dalam proyek nyata.
2. Mengembangkan game berbasis terminal menggunakan C++.
3. Mengimplementasikan berbagai struktur data dan algoritma yang telah dipelajari.
4. Melatih kemampuan perancangan, implementasi, dan pengujian perangkat lunak.
5. Menghasilkan aplikasi yang fungsional, terstruktur, dan mudah dikembangkan.

---

# 👥 Tim Pengembang

Project Astro Strike dikembangkan sebagai tugas akhir kelompok pada mata kuliah Algoritma Pemrograman.

---

# 📄 Lisensi

Project ini dibuat untuk tujuan akademik dan pembelajaran.
