#include "trading.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <conio.h>

/*==================================================
  MATERI: NAMESPACE, FUNCTION, FILE HANDLING, EXCEPTION HANDLING
==================================================*/
/*==================================================
  MODUL TRADING MENYIMULASIKAN TRANSAKSI PEMBELIAN DAN PENJUALAN ASET BTC DI MANA HARGANYA BERUBAH MENGIKUTI WAKTU RILL HARIAN DENGAN DOMPET KOIN.
==================================================*/
namespace TradingModule {

    // ============================================================
    // Fungsi-fungsi bantuan (helper) untuk memperingkas repetisi dari operasi string, format tanggal/waktu, atau tata letak karakter.
    // ============================================================

    /*==================================================
      MATERI: INLINE FUNCTION - GARIS PEMBATAS.
    ==================================================*/
    static inline void printSeparator() {
        std::cout << "  ";
        for (int i = 0; i < 44; ++i) std::cout << "=";
        std::cout << "\n";
    }

    // Tanggal hari ini bertipe YYYY-MM-DD.
    static std::string getTodayDate() {
        auto now = std::chrono::system_clock::now();
        time_t t = std::chrono::system_clock::to_time_t(now);
        tm* local = localtime(&t);

        std::stringstream ss;
        ss << std::put_time(local, "%Y-%m-%d");
        return ss.str();
    }

    // Merekam stempel waktu persis (timestamp) format standar.
    static std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        time_t t = std::chrono::system_clock::to_time_t(now);
        tm* local = localtime(&t);

        std::stringstream ss;
        ss << std::put_time(local, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    // ============================================================
    /*==================================================
      MATERI: FILE HANDLING, EXCEPTION HANDLING - MANAGEMEN MUAT DAN SIMPAN MEMORI DARI PROFIL ASET SETIAP AKUN PADA WALLET.
    ==================================================*/
    // ============================================================

    Wallet loadWallet(const std::string& playerName) {
        Wallet wallet;
        wallet.playerName = playerName;
        wallet.coin = 0;
        wallet.btc = 0.0;
        wallet.eth = 0.0; // not used, kept for struct compatibility

        std::ifstream file(FileConfig::WALLET_FILE);
        if (!file.is_open()) {
            // Jika file baru, hasilkan dompet yang nominalnya dimulai dari saldo permulaan sistem.
            return wallet;
        }

        std::string line;
        // Lebati bacaan header karena berupa teks penamaan kolom.
        std::getline(file, line);

        /*==================================================
          MATERI: FILE HANDLING - LAKUKAN PARSE TEKS KOLOM FORMAT SEPARATOR KOMA DI MEMORI MENJADI DATA TERSTRUKTUR (STRUCT).
        ==================================================*/
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::istringstream iss(line);
            std::string name, coinStr, btcStr;

            std::getline(iss, name, ',');
            std::getline(iss, coinStr, ',');
            std::getline(iss, btcStr, ',');

            if (name == playerName) {
                /*==================================================
                  MATERI: EXCEPTION HANDLING - MENCEGAH SISTEM PUTUS PAKSA APAKALA TIPE DATA YANG DIBERIKAN TERKOMPROMI/RUSAK.
                ==================================================*/
                try {
                    wallet.coin = std::stoi(coinStr);
                    wallet.btc = std::stod(btcStr);
                } catch (const std::exception&) {
                    // Gunakan nominal awal karena parameter gagal dilanjutkan/invalid data.
                    wallet.coin = 0;
                    wallet.btc = 0.0;
                }
                file.close();
                return wallet;
            }
        }

        file.close();
        // Beri respon objek kosong kalau baris tersebut merupakan hak akun punya pemain lain.
        return wallet;
    }

    void saveWallet(const Wallet& wallet) {
        /*==================================================
          MATERI: STL VECTOR - BUFFER BACAAN UNTUK SEMUA DAFTAR KEPEMILIKAN WALLET AGAR TAK ADA AKUN YANG TAK SENGAJA DISAPU SAAT DITIMPA (OVER-WRITE).
        ==================================================*/
        std::vector<Wallet> allWallets;
        bool playerFound = false;

        std::ifstream inFile(FileConfig::WALLET_FILE);
        if (inFile.is_open()) {
            std::string line;
            // Lewati pembacaan informasi header struktur CSV karena kita sudah menetapkan ini di variabel sistem.
            std::getline(inFile, line);

            while (std::getline(inFile, line)) {
                if (line.empty()) continue;

                std::istringstream iss(line);
                std::string name, coinStr, btcStr;

                std::getline(iss, name, ',');
                std::getline(iss, coinStr, ',');
                std::getline(iss, btcStr, ',');

                Wallet w;
                w.playerName = name;
                try {
                    w.coin = std::stoi(coinStr);
                    w.btc = std::stod(btcStr);
                } catch (...) {
                    w.coin = 0;
                    w.btc = 0.0;
                }
                w.eth = 0.0;

                // Kalau sesuai maka kita ubah objek struct-nya langsung.
                if (name == wallet.playerName) {
                    w = wallet;
                    playerFound = true;
                }

                allWallets.push_back(w);
            }
            inFile.close();
        }

        // Bila status baris nama ini nihil, berarti sistem diwajibkan menulis pendatang baru pada ujung akhir urutan tersebut (append).
        if (!playerFound) {
            allWallets.push_back(wallet);
        }

        /*==================================================
          MATERI: FILE HANDLING - ROMBAK ISI FILE DENGAN PEMBARUAN BARIS BERFORMATKAN CSV YANG DIRAKIT ULANG DARI OBJEK STRUCT BARU.
        ==================================================*/
        std::ofstream outFile(FileConfig::WALLET_FILE);
        if (!outFile.is_open()) {
            throw FileException("Cannot write to " + FileConfig::WALLET_FILE);
        }

        outFile << "playerName,coin,btc\n";
        for (const auto& w : allWallets) {
            outFile << w.playerName << ","
                    << w.coin << ","
                    << std::fixed << std::setprecision(4) << w.btc << "\n";
        }
        outFile.close();
    }

    // ============================================================
    /*==================================================
      MATERI: FILE HANDLING - KONFIGURASI SIMULASI UNTUK MENGOLAH PASANG SURUT HARGA PASAR KOMODITI YANG SEDANG DIPERDAGANGKAN DI MARKET.
    ==================================================*/
    // ============================================================

    int getCurrentBTCPrice() {
        std::string today = getTodayDate();
        std::string lastDate;
        std::string lastPriceStr;

        // Melacak ke pangkalan rekaman pergerakan harga komoditas (mis: data harian market).
        std::ifstream inFile(FileConfig::MARKET_HISTORY_FILE);
        if (inFile.is_open()) {
            std::string line;
            // Mengabaikan struktur baris penamaan file.
            std::getline(inFile, line);

            while (std::getline(inFile, line)) {
                if (line.empty()) continue;

                std::istringstream iss(line);
                std::getline(iss, lastDate, ',');
                std::getline(iss, lastPriceStr, ',');
            }
            inFile.close();
        }

        // Mengirim balik parameter jika sudah tersedia nilainya agar tidak berganti ganti nominal setiap detiknya pada sesi/hari operasi ini dijalankan (mengembalikan status persisten).
        if (lastDate == today && !lastPriceStr.empty()) {
            try {
                return std::stoi(lastPriceStr);
            } catch (...) {
                // Beralih kembali agar di-generate-ulang jika terjadi error dalam pemilahan (stod).
            }
        }

        // Menghasilkan bilangan komoditas fluktuasi pasaran secara otomatis menggunakan RNG sistem dalam interval kewajaran (bound rand).
        int newPrice = (rand() % 40001) + 30000;

        // Merekam pergerakan harian yang dinamis tadi jadi daftar tetap terhubung persisten.
        std::ofstream outFile(FileConfig::MARKET_HISTORY_FILE, std::ios::app);
        if (outFile.is_open()) {
            outFile << today << "," << newPrice << "\n";
            outFile.close();
        }

        return newPrice;
    }

    // ============================================================
    // Metode pembantu pengujian/testing tersembunyi yang berguna menyimulasikan/memaksa pergantian acak harian pada hari yang identik saat debugging program.
    // ============================================================
    void forceUpdateBTCPrice() {
        std::string today = getTodayDate();
        int newPrice = (rand() % 40001) + 30000;

        std::vector<std::string> lines;
        std::ifstream inFile(FileConfig::MARKET_HISTORY_FILE);
        if (inFile.is_open()) {
            std::string line;
            while (std::getline(inFile, line)) {
                lines.push_back(line);
            }
            inFile.close();
        }

        bool updated = false;
        if (!lines.empty()) {
            std::string lastLine = lines.back();
            std::istringstream iss(lastLine);
            std::string lastDate;
            std::getline(iss, lastDate, ',');
            
            if (lastDate == today) {
                lines.back() = today + "," + std::to_string(newPrice);
                updated = true;
            }
        }

        if (!updated) {
            lines.push_back(today + "," + std::to_string(newPrice));
        }

        std::ofstream outFile(FileConfig::MARKET_HISTORY_FILE);
        if (outFile.is_open()) {
            for (const auto& l : lines) {
                outFile << l << "\n";
            }
            outFile.close();
        }
    }

    // ============================================================
    /*==================================================
      MATERI: FUNCTION - MENYAJIKAN ANTARMUKA PORTOFOLIO WALLET PADA LAYAR PENGGUNA (CLI).
    ==================================================*/
    // ============================================================

    void displayWallet(const Wallet& wallet) {
        printSeparator();
        std::cout << GameColor::MENU_TITLE << "         WALLET - " << wallet.playerName << GameColor::RESET << "\n";
        printSeparator();
        std::cout << "    Coin : " << wallet.coin << "\n";
        std::cout << "    BTC  : " << std::fixed << std::setprecision(4)
                  << wallet.btc << "\n";
        printSeparator();
    }

    // ============================================================
    /*==================================================
      MATERI: FUNCTION - MENYAJIKAN ANTARMUKA NILAI TICKER INSTRUMEN FINANSIAL DI BURSA.
    ==================================================*/
    // ============================================================

    void displayMarket() {
        int price = getCurrentBTCPrice();
        std::string today = getTodayDate();

        printSeparator();
        std::cout << GameColor::MENU_TITLE << "         BTC MARKET - " << today << GameColor::RESET << "\n";
        printSeparator();
        std::cout << "    Current BTC Price : " << price << " coin\n";
        printSeparator();
    }

    // ============================================================
    /*==================================================
      MATERI: FUNCTION, EXCEPTION HANDLING - OPERASI PEMESANAN BUY ORDER INSTRUMEN KOMODITI BTC DENGAN PEMROSESAN PEMBATASAN PENGECEKAN ASET PEMAIN DI DOMPET SUPAYA SISTEM TIDAK OVERSPEND (SALDONYA NGAWUR).
    ==================================================*/
    // ============================================================

    void buyBTC(Wallet& wallet) {
        int btcPrice = getCurrentBTCPrice();

        std::cout << "\n    Current BTC Price: " << btcPrice << " coin\n";
        std::cout << "    Your Coin: " << wallet.coin << "\n\n";

        double amount = 0.0;
        std::cout << "    How much BTC to buy: ";
        std::cin >> amount;

        /*==================================================
          MATERI: EXCEPTION HANDLING - MENANGKIS CRASH MEMORI JIKA PENGGUNA TANPA SENGAJA / ISENG MEMASUKKAN HURUF / SIMBOL LAIN PADA FORM ANGKA INI.
        ==================================================*/
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Invalid input." << GameColor::RESET << "\n";
            return;
        }

        if (amount <= 0) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Amount must be greater than 0." << GameColor::RESET << "\n";
            return;
        }

        int totalCost = static_cast<int>(amount * btcPrice);

        if (totalCost <= 0) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Amount too small." << GameColor::RESET << "\n";
            return;
        }

        // Memastikan stok nominal mencukupi batas minim dari nilai aset pembelian.
        if (wallet.coin < totalCost) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Not enough coin!" << GameColor::RESET
                      << " Need " << totalCost
                      << ", you have " << wallet.coin << ".\n";
            return;
        }

        // Menjalankan eksekusi perhitungan transfer koin antar dompet pertukaran dan pemotongan jumlah (swap) portofolio akhir.
        wallet.coin -= totalCost;
        wallet.btc += amount;

        // Melegitimasi pengesahan akhir (commit/save) pembaruan database dompet akun saat selesai order agar transaksi yang berlangsung tidak menjadi bayang bayang ulangan (bocor).
        saveWallet(wallet);

        /*==================================================
          MATERI: FILE HANDLING - MEREKAM REKAM SEJARAH PEMBELIAN TERVALIDASI INI AGAR DAPAT DIPAMPANG DAN DIBACA KAPAN KAPAN SAJA DARI LOG ARSIP KHUSUS PEMAIN (AUDIT HISTORIS).
        ==================================================*/
        std::ofstream tradeFile(FileConfig::TRADE_HISTORY_FILE, std::ios::app);
        if (tradeFile.is_open()) {
            tradeFile << getTodayDate() << ","
                      << wallet.playerName << ","
                      << "BUY" << ","
                      << "BTC" << ","
                      << std::fixed << std::setprecision(4) << amount << ","
                      << btcPrice << ","
                      << totalCost << "\n";
            tradeFile.close();
        }

        std::cout << "\n    " << GameColor::TXT_SUCCESS << "[Success] Bought " << std::fixed << std::setprecision(4)
                  << amount << " BTC for " << totalCost << " coin!" << GameColor::RESET << "\n";
        displayWallet(wallet);
    }

    // ============================================================
    /*==================================================
      MATERI: FUNCTION, EXCEPTION HANDLING - OPERASI PEMESANAN SELL ORDER, MEKANISME PELEPASAN (LIKUIDASI) POSISI KEPEMILIKAN ASET PASAR UNTUK DIAMBIL UNTUNG PADA KOIN IN-GAME AGAR PORTOFOLIONYA TERTRANSLASI KEMBALI DALAM SALDO KOIN BIASA.
    ==================================================*/
    // ============================================================

    void sellBTC(Wallet& wallet) {
        int btcPrice = getCurrentBTCPrice();

        std::cout << "\n    Current BTC Price: " << btcPrice << " coin\n";
        std::cout << "    Your BTC: " << std::fixed << std::setprecision(4)
                  << wallet.btc << "\n\n";

        double amount = 0.0;
        std::cout << "    How much BTC to sell: ";
        std::cin >> amount;

        /*==================================================
          MATERI: EXCEPTION HANDLING - FILTER INPUT HURUF MAUPUN SPASI (CLEAR INPUT STREAM DAN IGNORE STREAM) AKIBAT KESALAHAN JARI PADA COMMAND BAR / CLI SUPAYA PROMPT TETAP VALID / TAK MASUK KE LOOP TIDAK HENTI HENTI.
        ==================================================*/
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Invalid input." << GameColor::RESET << "\n";
            return;
        }

        if (amount <= 0) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Amount must be greater than 0." << GameColor::RESET << "\n";
            return;
        }

        // Memastikan akun saldo BTC nya sendiri bukan merupakan fiktif lalu divalidasi kebenaran ukurannya di portofolio agar akun tak berakhir melakukan pelepasan posisi bodong yang mustahil (mengeluarkan exception logis pada proses pengecekan margin aset di Wallet).
        if (wallet.btc < amount) {
            std::cout << "\n    " << GameColor::TXT_ERROR << "[Error] Not enough BTC!" << GameColor::RESET
                      << " You have " << std::fixed << std::setprecision(4)
                      << wallet.btc << " BTC.\n";
            return;
        }

        int totalEarned = static_cast<int>(amount * btcPrice);

        // Pengurangan pada rasio komoditi bursa, lalu mengalirkan akumulasi total hasil ke mata uang biasa milik pengguna dan membalikkannya jadi pemasukan akun tersebut.
        wallet.btc -= amount;
        wallet.coin += totalEarned;

        // Mendaftarkan transaksi penyelesaian ini (selesai swap likuidasi / profit pengambilan margin) menuju penyimpanan profil database dari Player di dalam CSV Wallet untuk mencegah duplikasi atau penghangusan portofolio ketika pengguna mendadak meninggalkan atau keluar / log-out aplikasi pasca pencairan selesai.
        saveWallet(wallet);

        // Menulis bukti transfer ke dalam file buku rekap penjualan pribadi historis milik pemain ini sendiri.
        std::ofstream tradeFile(FileConfig::TRADE_HISTORY_FILE, std::ios::app);
        if (tradeFile.is_open()) {
            tradeFile << getTodayDate() << ","
                      << wallet.playerName << ","
                      << "SELL" << ","
                      << "BTC" << ","
                      << std::fixed << std::setprecision(4) << amount << ","
                      << btcPrice << ","
                      << totalEarned << "\n";
            tradeFile.close();
        }

        std::cout << "\n    " << GameColor::TXT_SUCCESS << "[Success] Sold " << std::fixed << std::setprecision(4)
                  << amount << " BTC for " << totalEarned << " coin.\n";
        displayWallet(wallet);
    }

    // ============================================================
    /*==================================================
      MATERI: FILE HANDLING, ITERATOR - ANTARMUKA MENCETAK BUKTI ARSIP DARI SEGALA JENIS PEMINDAHAN MARGIN PORTOFOLIO ATAU BUKTI RIWAYAT PEMROSESAN / LAPORAN ORDER HISTORIKAL PEMAIN YANG TEREKAM PADA PEMBUKUAN / CSV LEDGER PERMAINAN.
    ==================================================*/
    // ============================================================

    void showTradeHistory() {
        std::ifstream file(FileConfig::TRADE_HISTORY_FILE);

        if (!file.is_open()) {
            std::cout << "\n    No trade history found.\n";
            return;
        }

        std::string line;
        // Lewati pengindeksan label kolom CSV.
        std::getline(file, line);

        // Menjadi sakelar atau pengingat parameter logikal pembaca (bool flag), untuk memeriksa setidaknya keberadaan transaksi satu pun dalam tabel yang sedang dibongkar.
        bool hasData = false;

        printSeparator();
        std::cout << GameColor::MENU_TITLE << "              TRADE HISTORY\n" << GameColor::RESET;
        printSeparator();

        std::cout << "  " << std::left
                  << std::setw(12) << "Date"
                  << std::setw(12) << "Player"
                  << std::setw(6)  << "Type"
                  << std::setw(6)  << "Asset"
                  << std::setw(10) << "Amount"
                  << std::setw(8)  << "Price"
                  << std::setw(8)  << "Total" << "\n";

        std::cout << "  ";
        for (int i = 0; i < 44; ++i) std::cout << "-";
        std::cout << "\n";

        /*==================================================
          MATERI: FILE HANDLING - MENSINKRONISASIKAN SATU PER-SATU ALUR FORMAT STRING BERKOMA MENJADI ELEMEN DATA VISUAL ANTAR-KOLOM DI LAYER PEMANTAUAN PEMAIN SECARA LINIER.
        ==================================================*/
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            hasData = true;

            std::istringstream iss(line);
            std::string day, name, type, asset, amount, price, total;

            std::getline(iss, day, ',');
            std::getline(iss, name, ',');
            std::getline(iss, type, ',');
            std::getline(iss, asset, ',');
            std::getline(iss, amount, ',');
            std::getline(iss, price, ',');
            std::getline(iss, total, ',');

            std::cout << "  " << std::left
                      << std::setw(12) << day
                      << std::setw(12) << name
                      << std::setw(6)  << type
                      << std::setw(6)  << asset
                      << std::setw(10) << amount
                      << std::setw(8)  << price
                      << std::setw(8)  << total << "\n";
        }

        if (!hasData) {
            std::cout << "    No transactions yet.\n";
        }

        printSeparator();
        file.close();
    }

    // ============================================================
    /*==================================================
      MATERI: FUNCTION - TITIK PELUNCURAN AWAL DAN MENU INDUK TEMPAT BERSARANGNYA TOMBOL INTERAKSI ANTARA PORTOFOLIO MARKET CRYPTO / BURSA, PEMANTAUAN WALLET DOMPET KOIN, BESERTA MENU ARSIP REKAMAN ORDER TRANSAKSI PENJUALAN ASET YANG TERMODULASI DENGAN BAIK DAN AMAN DI DALAM SUB SISTEM TRADING.
    ==================================================*/
    // ============================================================

    void menuTrading(Wallet& wallet) {
        char choice;

        do {
            system("cls");
            printSeparator();
            std::cout << GameColor::MENU_TITLE << "           TRADING MARKET\n" << GameColor::RESET;
            printSeparator();
            std::cout << "\n";
            std::cout << "    [1] View Wallet\n";
            std::cout << "    [2] View BTC Price\n";
            std::cout << "    [3] Buy BTC\n";
            std::cout << "    [4] Sell BTC\n";
            std::cout << "    [5] Trade History\n";
            std::cout << "    [0] Back to Main Menu\n";
            std::cout << "\n    Your choice: ";
            
            choice = _getch();
            std::cout << choice << "\n\n";

            switch (choice) {
                case '1':
                    displayWallet(wallet);
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case '2':
                    displayMarket();
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case '3':
                    buyBTC(wallet);
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case '4':
                    sellBTC(wallet);
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case '5':
                    showTradeHistory();
                    std::cout << "\n    Press any key to continue...";
                    _getch();
                    break;
                case 'm':
                case 'M':
                    forceUpdateBTCPrice();
                    std::cout << "    [Secret] Market price has been magically updated.\n";
                    std::cout << "    Press any key to continue...";
                    _getch();
                    break;
                case '0':
                    // Memanggil trigger save secara otomatis sebagai garansi / asuransi apabila program dihentikan sesaat setelah keluar bursa tanpa menyimpan lebih dari satu posisi order ganda, hal ini untuk menahan portofolio pemain.
                    saveWallet(wallet);
                    break;
                default:
                    std::cout << "    Invalid choice. Press any key...";
                    _getch();
                    break;
            }

        } while (choice != '0');
    }

} // namespace TradingModule
