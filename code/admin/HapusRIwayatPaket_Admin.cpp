#include <iostream>
#include <string>
#include <vector>
#include <iomanip>   
#include "../include/data.h"
#include "../include/admin.h"
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

bool bisaDihapus(string status) {
    return (
        status == "Selesai" ||
        status == "Dibatalkan"
    );
}

void cetakTabelHapusRiwayat(const vector<int>& indeksBisaHapus) {
    cout << CYAN << BOLD << setfill('=') << setw(103) << "" << setfill(' ') << RESET << endl;
    cout << BOLD << "                                   DAFTAR RIWAYAT PAKET (FINAL)                               " << RESET << endl;
    cout << CYAN << BOLD << setfill('=') << setw(103) << "" << setfill(' ') << RESET << endl;
    
    cout << left << setw(4)  << "No" 
        << "| " << setw(12) << "Resi" 
        << "| " << setw(12) << "Pengirim" 
        << "| " << setw(12) << "Penerima" 
        << "| " << setw(12) << "Lokasi" 
        << "| " << setw(8)  << "Berat" 
        << "| " << setw(24) << "Status Akhir" << endl;
    cout << CYAN << setfill('-') << setw(103) << "" << setfill(' ') << RESET << endl;

    int nomorTabel = 1;
    for (int idx : indeksBisaHapus) {
        string warnaStatus = (paket[idx].status == "Selesai") ? HIJAU : MERAH;
        string namaPengirim = paket[idx].namaPengirim;
        if (namaPengirim.length() > 11) namaPengirim = namaPengirim.substr(0, 9) + "..";
        string namaPenerima = paket[idx].namaPenerima;
        if (namaPenerima.length() > 11) namaPenerima = namaPenerima.substr(0, 9) + "..";
        cout << left << setw(4)  << nomorTabel++
            << "| " << setw(12) << paket[idx].resi
            << "| " << setw(12) << namaPengirim
            << "| " << setw(12) << namaPenerima
            << "| " << setw(12) << paket[idx].lokasi
            << "| " << setw(8)  << (to_string(paket[idx].berat) + "g")
            << "| " << warnaStatus << BOLD << setw(24) << paket[idx].status << RESET << endl;
    }
    cout << CYAN << BOLD << setfill('=') << setw(103) << "" << setfill(' ') << RESET << "\n\n";
}

int menuScrollHapus(string judul, vector<string> pilihan, const vector<int>& indeksBisaHapus) {
    int posisi = 0;
    int key;
    
    while (true) {
        bersihkanLayar(); 
        cetakTabelHapusRiwayat(indeksBisaHapus); 
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
            return posisi + 1; // Mengembalikan angka navigasi 1, 2, dst.
        }
    }
}

void HapusRiwayatPaket_Admin() {
    loadPaket(); 
    bersihkanLayar();
    vector<int> indeksBisaHapus;
    for (int i = 0; i < jumlahPaket; i++) {
        if (bisaDihapus(paket[i].status)) {
            indeksBisaHapus.push_back(i); 
        }
    }

    if (indeksBisaHapus.empty()) {
        cout << KUNING << BOLD << "=========== HAPUS RIWAYAT PAKET ===========" << RESET << "\n\n";
        cout << MERAH << BOLD << "TIDAK ADA RIWAYAT PAKET YANG BISA DIHAPUS SAAT INI." << RESET << "\n\n";
        tekanEnter();
        return;
    }

    vector<string> listPilihanMenu;
    int nomorTabel = 1;
    for (int idx : indeksBisaHapus) {
        string teksMenu = "No. " + to_string(nomorTabel) + " [" + paket[idx].resi + "] " 
                        + paket[idx].namaPengirim + " -> " + paket[idx].namaPenerima;
        listPilihanMenu.push_back(teksMenu);
        nomorTabel++;
    }
    listPilihanMenu.push_back("Kembali ke Menu Admin");
    int pilihan = menuScrollHapus("PILIH RIWAYAT PAKET YANG INGIN DIHAPUS PERMANEN:", listPilihanMenu, indeksBisaHapus);
    if (pilihan == listPilihanMenu.size()) {
        bersihkanLayar();
        cout << KUNING << BOLD << "KEMBALI KE MENU ADMIN." << RESET << endl;
        tekanEnter();
        return;
    }
    int targetIndeks = indeksBisaHapus[pilihan - 1];
    bersihkanLayar();
    cout << MERAH << BOLD << "============= KONFIRMASI HAPUS PERMANEN =============" << RESET << endl;
    cout << "Resi        : " << paket[targetIndeks].resi << endl;
    cout << "Pengirim    : " << paket[targetIndeks].namaPengirim << endl;
    cout << "Penerima    : " << paket[targetIndeks].namaPenerima << endl;
    cout << "Status Paket: " << BOLD << paket[targetIndeks].status << RESET << endl;
    cout << MERAH << "-----------------------------------------------------" << RESET << endl;
    vector<string> konfirmasi = {"YA, Hapus Permanen Dari Database", "TIDAK, Batalkan Penghapusan"};
    int sScroll = menuScrollHapus("YAKIN DATA INI INGIN DIHAPUS? (AKSI TIDAK DAPAT DIURUNGKAN)", konfirmasi, indeksBisaHapus);

    bersihkanLayar();
    if (sScroll == 1) {
        for (int i = targetIndeks; i < jumlahPaket - 1; i++) {
            paket[i] = paket[i + 1];
        }
        jumlahPaket--;
        
        savePaket();
        cout << HIJAU << BOLD << "SUKSES! Riwayat paket berhasil dihapus permanen dari database." << RESET << endl;
    } else {
        cout << KUNING << BOLD << "Penghapusan dibatalkan. Data riwayat tetap aman." << RESET << endl;
    }
    cout << MERAH << "-----------------------------------------------------" << endl;
    tekanEnter();
}