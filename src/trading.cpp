#include "trading.h"
#include "gamedata.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <string>

// Placeholder — full implementation by Member 4
namespace TradingModule {

    tm* getTimeNow() {
        auto sekarang = std::chrono::system_clock::now();
        time_t waktu = std::chrono::system_clock::to_time_t(sekarang);
        return localtime(&waktu);
    }

    void garis() {
    std::cout << "=====================================================================" << std::endl;
    }

    void displayMarket() {
       std::fstream file("data/market_history.csv", std::ios::in | std::ios::app);
    
        if (!file) {
            std::cout << "File tidak ditemukan.\n";
            return;
        }

        int harga = rand() % 80001 + 20000;

        file.seekg(0);

        std::string last, tanggal, price;
        while (getline(file, last)) {
            std::istringstream iss(last);
            getline(iss, tanggal, ',');
            getline(iss, price, ',');
        }

        file.clear();

        std::stringstream data;
        data << std::put_time(getTimeNow(), "%Y-%m-%d");
        std::string dataterbaru = data.str();

        if (dataterbaru == tanggal) {
            std::cout << "Data sudah tersedia: [" << dataterbaru << "] - IDR Rp." << price << std::endl;
            file.close();
            return;
        } else {
            file << dataterbaru << "," << harga << std::endl;
            std::cout << "\nData baru telah diterima!" << std::endl;
        }

        file.close();
    }

    void displayWallet(const Wallet& wallet) {
        std::fstream file("data/wallet.csv", std::ios::in | std::ios::app);
    
        if (!file) {
            std::cout << "File tidak ditemukan.\n";
            return;
        }

        (void)wallet;
        garis();
        std::cout << "\nPlayer Username: " << wallet.playerName << std::endl;
        std::cout << "\nCoin mu sekarang: " << wallet.coin << std::endl;
        std::cout << "Jumlah BTC mu sekarang: " << wallet.btc << std::endl;
        garis();

        file.close();
    }

    void buyBTC(Wallet& wallet) {
        (void)wallet;
        std::fstream file("data/market_history.csv", std::ios::in | std::ios::app);
        std::fstream berkas("data/trade_history.csv", std::ios::in | std::ios::app);
        std::fstream file1("data/wallet.csv", std::ios::in | std::ios::app);
        
        if (!file || !berkas || !file1) {
            std::cout << "File tidak ditemukan.\n";
            return;
        }

        file.seekg(0);

        std::string last, tanggal, rc;
        while (getline(file, last)) {
            std::istringstream iss(last);
            getline(iss, tanggal, ',');
            getline(iss, rc, ',');
        }

        file.clear();

        std::stringstream data;
        data << std::put_time(getTimeNow(), "%Y-%m-%d %H:%M:%S");
        std::string dataterbaru = data.str();

        int pilih;
        std::cout << "Mau beli berapa: ";
        std::cin >> pilih;

        int harga = stoi(rc);
        int hargaBeli = pilih * harga;

        if (wallet.coin < hargaBeli) {
            std::cout << "\nCoin mu kurang" << std::endl;
        } else {
            wallet.coin -= hargaBeli;
            wallet.btc += pilih;
            berkas << dataterbaru << wallet.playerName << ",Beli,BTC," << pilih << "," << harga << "," << hargaBeli << std::endl;
            displayWallet(wallet);
        }

        berkas.close();
        file.close();
        file1.close();
    }

    void sellBTC(Wallet& wallet) {
        (void)wallet;
        std::fstream file("data/market_history.csv", std::ios::in | std::ios::app);
        std::fstream berkas("data/trade_history.csv", std::ios::in | std::ios::app);
        std::fstream file1("data/wallet.csv", std::ios::in | std::ios::app);
        
        if (!file || !berkas) {
            std::cout << "File tidak ditemukan.\n";
            return;
        }

        file.seekg(0);

        std::string last, tanggal, pc;
        while (getline(file, last)) {
            std::istringstream iss(last);
            getline(iss, tanggal, ',');
            getline(iss, pc, ',');
        }

        file.clear();

        std::stringstream data;
        data << std::put_time(getTimeNow(), "%Y-%m-%d %H:%M:%S");
        std::string dataterbaru = data.str();

        int pilih;
        std::cout << "Mau jual berapa: ";
        std::cin >> pilih;

        int harga = stoi(pc);
        float pajak = (float)(rand() % 11 + 1.5)/5;
        int hargajual = harga * pajak / 100;

        wallet.coin += pilih * hargajual;

        if (wallet.btc < pilih) {
            std::cout << "BTC tidak cukup!\n";
            return;
        } else {
            wallet.btc -= pilih;
        }

        berkas << dataterbaru << wallet.playerName << ",Jual,BTC," << pilih << "," << harga << "," << (pilih * hargajual) << std::endl;
        displayWallet(wallet);

        berkas.close();
        file.close();
        file1.close();  
    }

    void history() {
        std::fstream file("data/trade_history.csv", std::ios::in | std::ios::app);
    
        if (!file) {
            std::cout << "File tidak ditemukan.\n";
            return;
        }

        file.seekg(0);
        garis();
        std::cout << "| " << std::setw(19) << std::left << "Tanggal"
            << "| " << std::setw(11) << std::left << "Username"
            << "| " << std::setw(11) << std::left << "Tipe"
            << "| " << std::setw(11) << std::left << "Asset"
            << "| " << std::setw(11) << std::left << "Jumlah"
            << "| " << std::setw(11) << std::left << "Harga Satuan"
            << "| " << std::setw(5)  << std::left << "Total" << " |" << std::endl;
        garis();

        std::string last;
        while (getline(file, last)) {
            if (last.empty()) continue;
            
            std::istringstream iss(last);
            std::string tgl, nm, tp, as, jml, hg, tl;
            
            getline(iss, tgl, ',');
            getline(iss, nm, ',');
            getline(iss, tp, ',');
            getline(iss, as, ',');
            getline(iss, jml, ',');
            getline(iss, hg, ',');
            getline(iss, tl, ',');

            std::cout << "| " << std::setw(12) << std::left << tgl
                << "| " << std::setw(11) << std::left << nm
                << "| " << std::setw(11) << std::left << tp
                << "| " << std::setw(11) << std::left << as
                << "| " << std::setw(11) << std::left << jml
                << "| " << std::setw(11) << std::left << hg
                << "| " << std::setw(5)  << std::left << tl << " |" << std::endl;
        }

        garis();
        file.clear();
        file.close();
    }

    void menuTrading(Wallet& wallet){
        int choice;

        do{
            garis();
            std::cout << "\nSelamat Datang di Trading BTC!" << std::endl;
            garis();
            std::cout << "1. Lihat harga BTC hari ini" << std::endl;
            std::cout << "2. Beli BTC hari ini" << std::endl;
            std::cout << "3. Jual BTC hari ini" << std::endl;
            std::cout << "4. Lihat riwayat transaksi" << std::endl;
            std::cout << "0. Keluar" << std::endl;
            std::cout << "Pilihanmu: ";
            std::cin >> choice; 
            std::cout << std::endl;

            if (choice == 1) {
                garis();
                displayMarket();
                garis();
            } else if (choice == 2) {
                garis();
                buyBTC(wallet);
                garis();
            } else if (choice == 3) {
                garis();
                sellBTC(wallet);
                garis();
            } else if (choice == 4) {
                history();
            } else {
                break;
            }

        } while (choice != 0);
    }

} // namespace TradingModule
