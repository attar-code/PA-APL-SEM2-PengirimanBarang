// #include <iostream>
// #include <fstream>
// #include "../include/data.h"
// #include "../database/json.hpp"

// using json = nlohmann::json;
// using namespace std;

// //VARIABEL GLOBAL
// extern string userAktif;

// //DEKLARASI FUNGSI
// void tekanEnter();

// //PROSEDUR
// //Materi: Prosedur
// void LihatRiwayatPaket_User() {
//     system("cls");
//     cout << "=== RIWAYAT PAKET ===\n";

//     //VARIABEL LOKAL
//     bool ditemukan = false;

//     //FILE HANDLING
//     ifstream inputFile("database/paket.json");

//     json data;

//     //ERROR HANDLING
//     if (inputFile.peek() == ifstream::traits_type::eof()) {
//         cout << "\nBelum ada riwayat paket.\n";
//         inputFile.close();
//         tekanEnter();
//         return;
//     }

//     inputFile >> data;

//     inputFile.close();

//     //SEARCHING + LINEAR SEARCH
//     for (auto paket : data) {

//         if (paket["pemilik"] == userAktif) {
//             ditemukan = true;
//             cout << "\n============================\n";
//             cout << "Nomor Resi     : "
//                  << paket["resi"] << endl;

//             cout << "Nama Pengirim  : "
//                  << paket["namaPengirim"] << endl;

//             cout << "Nama Penerima  : "
//                  << paket["namaPenerima"] << endl;

//             cout << "Alamat Tujuan  : "
//                  << paket["alamat"] << endl;

//             cout << "Berat Barang   : "
//                  << paket["berat"] << " gram" << endl;

//             cout << "Tipe Barang    : "
//                  << paket["tipe"] << endl;

//             cout << "Pembayaran     : "
//                  << paket["pembayaran"] << endl;

//             cout << "Total Ongkir   : Rp "
//                  << paket["ongkir"] << endl;

//             cout << "Status Paket   : "
//                  << paket["status"] << endl;
//         }
//     }

//     //ERROR HANDLING
//     if (!ditemukan) {
//         cout << "\nBelum ada riwayat paket.\n";
//     }

//     tekanEnter();
// }

#include <iostream>
#include <fstream>
#include <iomanip>   
#include "../include/data.h"
#include "../database/json.hpp"

// 🌟 TAMBAHKAN INI BIAR WARNA ANSI DIKENAL OLEH COMPILER

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

// PROSEDUR LIHAT RIWAYAT PAKET USER (VERSI KOLOM TERPISAH)
// =========================================================================
void LihatRiwayatPaket_User() {
    bersihkanLayar(); 

    // FILE HANDLING
    ifstream inputFile("database/paket.json");
    json data;

    // VALIDASI: Jika file tidak ada atau kosong
    if (!inputFile.is_open() || inputFile.peek() == ifstream::traits_type::eof()) {
        cout << KUNING << BOLD << "===============================================================================================================" << RESET << endl;
        cout << BOLD << "                                           RIWAYAT PENGIRIMAN PAKET                                            " << RESET << endl;
        cout << KUNING << BOLD << "===============================================================================================================" << RESET << "\n\n";
        cout << MERAH << BOLD << " [INFO] Belum ada riwayat paket terdaftar di sistem." << RESET << "\n\n";
        if (inputFile.is_open()) inputFile.close();
        tekanEnter();
        return;
    }

    inputFile >> data;
    inputFile.close();

    // 1. MENGGANDENG PROSEDUR CETAK HEADER TABEL (Lebar disesuaikan jadi 116 agar kolom terpisah muat longgar)
    cout << CYAN << BOLD << setfill('=') << setw(116) << "" << setfill(' ') << RESET << endl;
    cout << BOLD << "                                        RIWAYAT PENGIRIMAN PAKET ANDA                                        " << RESET << endl;
    cout << CYAN << BOLD << setfill('=') << setw(116) << "" << setfill(' ') << RESET << endl;
    
    cout << left << setw(4)  << "No" 
         << "| " << setw(12) << "Resi" 
         << "| " << setw(12) << "Penerima" 
         << "| " << setw(12) << "Lokasi"        // 🌟 Kolom Lokasi Sendiri
         << "| " << setw(18) << "Alamat Tujuan"  // 🌟 Kolom Alamat Sendiri
         << "| " << setw(7)  << "Berat" 
         << "| " << setw(10) << "Tipe"
         << "| " << setw(11) << "Ongkir" 
         << "| " << "Status Pengiriman" << endl;
         
    cout << CYAN << setfill('-') << setw(116) << "" << setfill(' ') << RESET << endl;

    int nomorTabel = 1;
    bool ditemukan = false;

    // 2. SEARCHING + LINEAR SEARCH (Hanya mengambil data milik userAktif)
    for (auto& item : data) {
        string pemilik = item.value("pemilik", "-");

        if (pemilik == userAktif) {
            ditemukan = true;

            string resi         = item.value("resi", "-");
            string namaPenerima = item.value("namaPenerima", "-");
            string lokasi       = item.value("lokasi", "-"); 
            string alamat       = item.value("alamat", "-"); 
            long long berat     = item.value("berat", 0LL);
            string tipe         = item.value("tipe", "-");
            long long ongkir    = item.value("ongkir", 0LL);
            string status       = item.value("status", "-");

            // Proteksi teks: Potong data jika terlalu panjang agar layout tabel tidak patah/melar
            if (namaPenerima.length() > 11) namaPenerima = namaPenerima.substr(0, 9) + "..";
            if (alamat.length() > 17) alamat = alamat.substr(0, 15) + "...";

            // Pewarnaan dinamis untuk status paket agar interaktif
            string warnaStatus = RESET;
            if (status == "Selesai") warnaStatus = HIJAU BOLD;
            else if (status == "Dibatalkan" || status == "Menunggu Validasi Admin") warnaStatus = MERAH BOLD;
            else warnaStatus = KUNING BOLD; 

            // Cetak Baris Tabel Data
            cout << left << setw(4)  << nomorTabel++
                 << "| " << setw(12) << resi
                 << "| " << setw(12) << namaPenerima
                 << "| " << setw(12) << lokasi    // 🌟 Cetak Lokasi
                 << "| " << setw(18) << alamat    // 🌟 Cetak Alamat
                 << "| " << setw(7)  << (to_string(berat) + "g")
                 << "| " << setw(10) << tipe
                 << "| Rp " << left << setw(8) << ongkir
                 << "| " << warnaStatus << status << RESET << endl;
        }
    }

    if (!ditemukan) {
        cout << left << setw(116) << "| [INFO] Anda belum pernah melakukan pengiriman paket apapun melalui akun ini." << endl;
    }

    cout << CYAN << BOLD << setfill('=') << setw(116) << "" << setfill(' ') << RESET << "\n\n";
    tekanEnter();
}