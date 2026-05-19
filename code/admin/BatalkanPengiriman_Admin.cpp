#include <iostream>
#include <string>
#include <vector>
#include <iomanip>  
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


bool bisaDibatalkan(string status) {
    return (
        status == "Menunggu Validasi Admin" ||
        status == "Diproses" ||
        status == "Diproses (COD)" ||
        status == "Diproses (Lunas)"
    );
}
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

int menuScrollBatal(string judul, vector<string> pilihan, const vector<int>& indeksBisaBatal) {
    int posisi = 0;
    int key;
    
    while (true) {
        bersihkanLayar();
        cetakTabelBatal(indeksBisaBatal); 
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
        if (key == 72) { 
            if (posisi > 0) posisi--;
            else posisi = pilihan.size() - 1;
        } 
        else if (key == 80) { 
            if (posisi < pilihan.size() - 1) posisi++;
            else posisi = 0;
        } 
        else if (key == 13 || key == 10) { // Enter
            return posisi + 1; 
        }
    }
}

void BatalkanPaket_Admin() {
    loadPaket(); 
    bersihkanLayar();
    vector<int> indeksBisaBatal;
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
    vector<string> listPilihanMenu;
    int nomorTabel = 1;
    for (int idx : indeksBisaBatal) {
        string teksMenu = "No. " + to_string(nomorTabel) + " [" + paket[idx].resi + "] " 
                        + paket[idx].namaPengirim + " -> " + paket[idx].namaPenerima;
        listPilihanMenu.push_back(teksMenu);
        nomorTabel++;
    }
    listPilihanMenu.push_back("Kembali ke Menu Admin");
    int pilihan = menuScrollBatal("PILIH PAKET YANG INGIN ANDA BATALKAN:", listPilihanMenu, indeksBisaBatal);
    if (pilihan == listPilihanMenu.size()) {
        bersihkanLayar();
        cout << KUNING << BOLD << "KEMBALI KE MENU ADMIN." << RESET << endl;
        tekanEnter();
        return;
    }
    int targetIndeks = indeksBisaBatal[pilihan - 1];
    bersihkanLayar();
    cout << MERAH << BOLD << "============= KONFIRMASI PEMBATALAN =============" << RESET << endl;
    cout << "Resi        : " << paket[targetIndeks].resi << endl;
    cout << "Pengirim    : " << paket[targetIndeks].namaPengirim << endl;
    cout << "Penerima    : " << paket[targetIndeks].namaPenerima << endl;
    cout << "Status Lama : " << KUNING << BOLD << paket[targetIndeks].status << RESET << endl;
    cout << MERAH << "-------------------------------------------------" << RESET << endl;
    vector<string> konfirmasi = {"YA, Batalkan Paket Ini", "TIDAK, Batalkan Proses"};
    int sScroll = menuScrollBatal("YAKIN INGIN MEMBATALKAN PAKET INI?", konfirmasi, indeksBisaBatal);
    bersihkanLayar();
    if (sScroll == 1) {
        paket[targetIndeks].status = "Dibatalkan"; 
        savePaket(); 
        cout << HIJAU << BOLD << "SUKSES! Paket Berhasil Dibatalkan." << RESET << endl;
    } else {
        cout << KUNING << BOLD << "Pembatalan paket dibatalkan oleh admin." << RESET << endl;
    }
    cout << MERAH << "-------------------------------------------------" << RESET << endl;
    tekanEnter();
}