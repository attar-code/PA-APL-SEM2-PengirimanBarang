#include <iostream>
#include <fstream>
#include <iomanip>   
#include "../include/data.h"
#include "../database/json.hpp"

#define RESET   "\033[0m"
#define MERAH   "\033[31m"
#define HIJAU   "\033[32m"
#define KUNING  "\033[33m"
#define BIRU    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

using json = nlohmann::json;
using namespace std;

// VARIABEL GLOBAL
extern string userAktif;

// DEKLARASI FUNGSI
void tekanEnter();

// PROSEDUR LIHAT RIWAYAT PAKET USER (REVISI FIX GARIS & WARNA KONSISTEN)
// =========================================================================
void LihatRiwayatPaket_User() {
    bersihkanLayar(); 

    // FILE HANDLING
    ifstream inputFile("database/paket.json");
    json data;

    // Lebar tabel disesuaikan secara presisi agar menutup teks status yang panjang
    int lebarTabel = 150;

    // 1. HEADER ATAS (Semua garis diseragamkan menjadi warna CYAN BOLD)
    cout << CYAN << BOLD << setfill('=') << setw(lebarTabel) << "" << "\n" << setfill(' ');
    cout << "                                                 RIWAYAT PENGIRIMAN PAKET                                                 \n";
    cout << "                                          (Urutan: Diproses -> Dikirim -> Selesai)                                        \n";
    cout << setfill('=') << setw(lebarTabel) << "" << "\n" << RESET << setfill(' ');

    // VALIDASI: Jika file tidak ada atau kosong
    if (!inputFile.is_open() || inputFile.peek() == ifstream::traits_type::eof()) {
        cout << "                                      " << KUNING << "Belum ada riwayat paket terdaftar." << RESET << "\n";
        cout << CYAN << BOLD << setfill('=') << setw(lebarTabel) << "" << "\n" << RESET << setfill(' ');
        if (inputFile.is_open()) inputFile.close();
        tekanEnter();
        return;
    }

    inputFile >> data;
    inputFile.close();

    // 2. HEADER KOLOM TABEL (Sesuai dengan ketukan lebar di layar kodemu)
    cout << BOLD << left 
         << setw(12) << "Resi" 
         << setw(16) << "Pengirim" 
         << setw(16) << "Penerima" 
         << setw(32) << "Alamat" 
         << setw(14) << "Lokasi"
         << setw(10) << "Berat" 
         << setw(14) << "Tipe"
         << setw(12) << "Ongkir" 
         << "Status\n" << RESET;
         
    // Garis tengah diubah menjadi CYAN BOLD agar senada dengan atas bawah
    cout << CYAN << BOLD << setfill('-') << setw(lebarTabel) << "" << "\n" << setfill(' ') << RESET;

    bool ditemukan = false;

    // 3. LOOPING AMBIL DATA
    for (auto& item : data) {
        string pemilik = item.value("pemilik", "-");

        if (pemilik == userAktif) {
            ditemukan = true;

            string resi         = item.value("resi", "-");
            string namaPengirim = item.value("namaPengirim", "-"); 
            string namaPenerima = item.value("namaPenerima", "-");
            string lokasi       = item.value("lokasi", "-"); 
            string alamat       = item.value("alamat", "-"); 
            int berat           = item.value("berat", 0);      
            string tipe         = item.value("tipe", "-");
            long long ongkir    = item.value("ongkir", 0LL);   
            string status       = item.value("status", "-");

            // Pewarnaan dinamis untuk teks status paket
            string warnaStatus = RESET;
            if (status == "Diproses") warnaStatus = KUNING;
            else if (status == "Dikirim") warnaStatus = BIRU;
            else if (status == "Selesai") warnaStatus = HIJAU;
            else if (status == "Dibatalkan" || status == "Menunggu Validasi Admin") warnaStatus = MERAH;

            // Cetak Baris Data
            cout << left 
                 << setw(12) << resi
                 << setw(16) << namaPengirim
                 << setw(16) << namaPenerima
                 << setw(32) << alamat
                 << setw(14) << lokasi
                 << setw(10) << (to_string(berat) + "g")
                 << setw(14) << tipe
                 << "Rp " << setw(9) << ongkir
                 << warnaStatus << BOLD << status << RESET << "\n";
        }
    }

    if (!ditemukan) {
        cout << "                               " << KUNING << "Anda belum pernah melakukan pengiriman paket apapun." << RESET << "\n";
    }

    // 4. GARIS PENUTUP BAWAH (Diubah menjadi CYAN BOLD dengan panjang 150)
    cout << CYAN << BOLD << setfill('=') << setw(lebarTabel) << "" << "\n" << RESET << setfill(' ');
    tekanEnter();
}