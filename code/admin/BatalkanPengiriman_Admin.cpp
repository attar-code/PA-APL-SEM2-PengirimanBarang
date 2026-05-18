// #include <iostream>
// #include <fstream>
// #include <limits>
// #include "../include/data.h"
// #include "../include/admin.h"
// #include "../database/json.hpp"

// using json = nlohmann::json;
// using namespace std;

// extern string userAktif;

// void tekanEnter();

// bool bisaDibatalkan(string status) {

//     return (
//         status == "Menunggu Validasi Admin" ||
//         status == "Diproses" ||
//         status == "Diproses (COD)" ||
//         status == "Diproses (Lunas)"
//     );
// }

// void BatalkanPaket_Admin() {

//     system("cls");
//     tampilRiwayatPaket();
    
//     cout << "=== BATALKAN PAKET ===\n";
//     cout << "Ketik 'batal' untuk kembali ke menu admin\n\n";

//     ifstream inputFile("database/paket.json");

//     if (!inputFile.is_open()) {
//         cout << "Gagal membuka database!\n";
//         tekanEnter();
//         return;
//     }

//     json data;
//     inputFile >> data;
//     inputFile.close();

//     string resiCari;

//     cout << "Masukkan nomor resi : ";
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     getline(cin, resiCari);

//     // 🔥 FITUR EXIT
//     if (resiCari == "batal") {
//         cout << "\nKembali ke menu admin...\n";
//         return;
//     }

//     bool ditemukan = false;

//     for (auto &paket : data) {

//         if (paket["resi"].get<string>() == resiCari) {

//             ditemukan = true;

//             string status = paket["status"].get<string>();

//             if (status == "Dikirim" || status == "Selesai") {

//                 cout << "\nPaket sudah dikirim/selesai, tidak bisa dibatalkan!\n";
//                 tekanEnter();
//                 return;
//             }

//             if (!bisaDibatalkan(status)) {

//                 cout << "\nStatus tidak valid untuk pembatalan!\n";
//                 tekanEnter();
//                 return;
//             }

//             int pilihan;

//             cout << "\nStatus saat ini: " << status << endl;
//             cout << "Yakin ingin membatalkan?\n";
//             cout << "1. Ya\n";
//             cout << "2. Tidak\n";
//             cout << "Pilih : ";
//             cin >> pilihan;
//             cin.ignore();

//             if (pilihan == 1) {

//                 paket["status"] = "Dibatalkan";

//                 ofstream outputFile("database/paket.json");
//                 outputFile << data.dump(4);
//                 outputFile.close();

//                 cout << "\nPaket berhasil dibatalkan!\n";
//             }
//             else {
//                 cout << "\nPembatalan dibatalkan.\n";
//             }

//             break;
//         }
//     }

//     if (!ditemukan) {
//         cout << "\nPaket tidak ditemukan!\n";
//     }

//     tekanEnter();
// }

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>   // 🌟 Tetap wajib untuk merapikan spasi tabel (setw, setfill)
#include "../include/data.h"
#include "../include/admin.h"
#include "../database/json.hpp"

#define RESET "\033[0m"
#define MERAH "\033[31m"
#define HIJAU "\033[32m"
#define KUNING "\033[33m"
#define BIRU "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

using json = nlohmann::json;
using namespace std;

// _getch() didefinisikan di file utama kamu (main.cpp)
int _getch(); 

// Fungsi cek status apakah paket bisa dibatalkan
bool bisaDibatalkan(string status) {
    return (
        status == "Menunggu Validasi Admin" ||
        status == "Diproses" ||
        status == "Diproses (COD)" ||
        status == "Diproses (Lunas)"
    );
}

// =========================================================================
// 🌟 PROSEDUR BANTUAN: Menggambar tabel daftar paket (Lebar 103, Tanpa Pointer)
// =========================================================================
void cetakTabelBatal(const vector<int>& indeksBisaBatal) {
    cout << CYAN << BOLD << setfill('=') << setw(103) << "" << setfill(' ') << RESET << endl;
    cout << BOLD << "                                   DAFTAR PAKET YANG BISA DIBATALKAN                          " << RESET << endl;
    cout << CYAN << BOLD << setfill('=') << setw(103) << "" << setfill(' ') << RESET << endl;
    
    cout << left << setw(4)  << "No" 
         << "| " << setw(12) << "Resi" 
         << "| " << setw(12) << "Pengirim" 
         << "| " << setw(12) << "Penerima" 
         << "| " << setw(12) << "Lokasi" 
         << "| " << setw(8)  << "Berat" 
         << "| " << setw(24) << "Status Saat Ini" << endl;
         
    cout << CYAN << setfill('-') << setw(103) << "" << setfill(' ') << RESET << endl;

    int nomorTabel = 1;
    for (int idx : indeksBisaBatal) {
        // 👍 SEKARANG PAKAI CARA BIASA: paket[idx].namaVariabel (Lebih simpel dibaca)
        cout << left << setw(4)  << nomorTabel++
             << "| " << setw(12) << paket[idx].resi
             << "| " << setw(12) << paket[idx].namaPengirim
             << "| " << setw(12) << paket[idx].namaPenerima
             << "| " << setw(12) << paket[idx].lokasi
             << "| " << setw(8)  << (to_string(paket[idx].berat) + "g")
             << "| " << MERAH << BOLD << setw(24) << paket[idx].status << RESET << endl;
    }
    cout << CYAN << BOLD << setfill('=') << setw(103) << "" << setfill(' ') << RESET << "\n\n";
}

// =========================================================================
// 🌟 FUNGSI NAVIGASI: menuScrollBatal agar tabel tidak bertelur/menduplikasi
// =========================================================================
int menuScrollBatal(string judul, vector<string> pilihan, const vector<int>& indeksBisaBatal) {
    int posisi = 0;
    int key;
    
    while (true) {
        bersihkanLayar(); // Hapus layar total agar tidak menumpuk
        
        cetakTabelBatal(indeksBisaBatal); // Gambar ulang tabel di posisi teratas
        
        cout << KUNING << BOLD << "=======================================================================================================" << RESET << endl;
        cout << BOLD << "   " << judul << RESET << endl;
        cout << KUNING << BOLD << "=======================================================================================================" << endl;
        
        for (size_t i = 0; i < pilihan.size(); i++) {
            if (i == posisi) {
                cout << MERAH << BOLD << "  > [ " << pilihan[i] << " ] <" << RESET << endl;
            } else {
                cout << "      " << pilihan[i] << endl;
            }
        }
        cout << KUNING << "-------------------------------------------------------------------------------------------------------" << RESET << endl;
        cout << "Gunakan Panah & Enter" << endl;

        key = _getch();

        if (key == 27) { 
            _getch(); 
            key = _getch();
            if (key == 65) key = 72;      
            else if (key == 66) key = 80; 
        }
        else if (key == 224) { 
            key = _getch(); 
        }

        if (key == 72) { // Panah Atas
            if (posisi > 0) posisi--;
            else posisi = pilihan.size() - 1;
        } 
        else if (key == 80) { // Panah Bawah
            if (posisi < pilihan.size() - 1) posisi++;
            else posisi = 0;
        } 
        else if (key == 13 || key == 10) { // Enter
            return posisi + 1; 
        }
    }
}

// =========================================================================
// PROSEDUR UTAMA: BatalkanPaket_Admin (VERSI SUPER SIMPEL)
// =========================================================================
void BatalkanPaket_Admin() {
    loadPaket(); // Memuat data dari JSON ke array paket
    bersihkanLayar();

    vector<int> indeksBisaBatal;

    // 1. Filter paket menggunakan perulangan array biasa (Tanpa Pointer `*` atau `->`)
    for (int i = 0; i < jumlahPaket; i++) {
        if (bisaDibatalkan(paket[i].status)) {
            indeksBisaBatal.push_back(i); 
        }
    }

    if (indeksBisaBatal.empty()) {
        cout << KUNING << BOLD << "=========== PEMBATALAN PAKET ===========" << RESET << "\n\n";
        cout << HIJAU << BOLD << "TIDAK ADA ANTREAN PAKET YANG BISA DIBATALKAN SAAT INI." << RESET << "\n\n";
        tekanEnter();
        return;
    }

    // 2. Meracik pilihan menu scroll
    vector<string> listPilihanMenu;
    int nomorTabel = 1;
    for (int idx : indeksBisaBatal) {
        string teksMenu = "No. " + to_string(nomorTabel) + " [" + paket[idx].resi + "] " 
                        + paket[idx].namaPengirim + " -> " + paket[idx].namaPenerima;
        listPilihanMenu.push_back(teksMenu);
        nomorTabel++;
    }
    listPilihanMenu.push_back("Kembali ke Menu Admin");

    // 3. Jalankan navigasi panah
    int pilihan = menuScrollBatal("PILIH PAKET YANG INGIN ANDA BATALKAN:", listPilihanMenu, indeksBisaBatal);

    if (pilihan == listPilihanMenu.size()) {
        bersihkanLayar();
        cout << KUNING << BOLD << "KEMBALI KE MENU ADMIN." << RESET << endl;
        tekanEnter();
        return;
    }

    // 4. Ambil indeks target paket yang dipilih
    int targetIndeks = indeksBisaBatal[pilihan - 1];

    bersihkanLayar();
    cout << MERAH << BOLD << "============= KONFIRMASI PEMBATALAN =============" << RESET << endl;
    cout << "Resi        : " << paket[targetIndeks].resi << endl;
    cout << "Pengirim    : " << paket[targetIndeks].namaPengirim << endl;
    cout << "Penerima    : " << paket[targetIndeks].namaPenerima << endl;
    cout << "Status Lama : " << KUNING << BOLD << paket[targetIndeks].status << RESET << endl;
    cout << MERAH << "-------------------------------------------------" << RESET << endl;
    
    // Konfirmasi akhir pakai menu scroll mini
    vector<string> konfirmasi = {"YA, Batalkan Paket Ini", "TIDAK, Batalkan Proses"};
    int sScroll = menuScrollBatal("YAKIN INGIN MEMBATALKAN PAKET INI?", konfirmasi, indeksBisaBatal);

    bersihkanLayar();
    if (sScroll == 1) {
        // Langsung ubah ke array globalnya secara instan
        paket[targetIndeks].status = "Dibatalkan"; 
        savePaket(); // Simpan otomatis ke JSON database
        
        cout << HIJAU << BOLD << "SUKSES! Paket Berhasil Dibatalkan." << RESET << endl;
    } else {
        cout << KUNING << BOLD << "Pembatalan paket dibatalkan oleh admin." << RESET << endl;
    }

    cout << MERAH << "-------------------------------------------------" << RESET << endl;
    tekanEnter();
}