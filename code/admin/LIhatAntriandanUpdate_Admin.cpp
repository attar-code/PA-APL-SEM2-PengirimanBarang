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

bool paketdiproses(string status) {
    return (status == "Diproses" || 
            status == "Diproses (COD)" ||
            status == "Diproses (Lunas)");
}
bool paketaktif(string status) {
    return (status == "Menunggu Validasi Admin" || 
            status == "Diproses" || 
            status == "Diproses (COD)" ||
            status == "Diproses (Lunas)" || 
            status == "Dikirim");
}
bool resiSudahAda(string resi){
    for(int i = 0; i < jumlahPaket; i++){
        if(paket[i].resi == resi) 
        return true;
    }
    return false;
}

string generateResiUnik() {
    string resiBaru = generateResi(); 
    if (!resiSudahAda(resiBaru))
    return resiBaru;
    return generateResiUnik(); 
}

int prioritasStatus(string status){
    if(status == "Menunggu Validasi Admin") return 1;
    else if(status == "Diproses" ||
            status == "Diproses (COD)" || 
            status == "Diproses (Lunas)") 
        return 2;
    else if(status == "Dikirim")
        return 3;
    else if(status == "Selesai") 
        return 4;
    return 5;
}

void bubbleSortStatus(){
    for(int i = 0; i < jumlahPaket-1; i++){
        for(int j = 0; j < jumlahPaket-i-1; j++){
            if(prioritasStatus(paket[j].status) > prioritasStatus(paket[j+1].status)){
                swap(paket[j], paket[j+1]);
            }
        }
    }
}

void cetakTabelAntrean(const vector<int>& indeksPaketAktif) {
    cout << CYAN << BOLD << setfill('=') << setw(103) << "" << setfill(' ') << RESET << endl;
    cout << BOLD << "                                      DAFTAR ANTREAN PAKET AKTIF                            " << RESET << endl;
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
    for (int idx : indeksPaketAktif) {
        cout << left << setw(4)  << nomorTabel++
            << "| " << setw(12) << paket[idx].resi
            << "| " << setw(12) << paket[idx].namaPengirim
            << "| " << setw(12) << paket[idx].namaPenerima
            << "| " << setw(12) << paket[idx].lokasi
            << "| " << setw(8)  << (to_string(paket[idx].berat) + "g")
            << "| " << BIRU << BOLD << setw(24) << paket[idx].status << RESET << endl;
    }
    cout << CYAN << BOLD << setfill('=') << setw(103) << "" << setfill(' ') << RESET << "\n\n";
}

int menuScrollUpdate(string judul, vector<string> pilihan, const vector<int>& indeksPaketAktif) {
    int posisi = 0;
    int key;
    
    while (true) {
        bersihkanLayar(); 
        cetakTabelAntrean(indeksPaketAktif); 
        cout << KUNING << BOLD << "=======================================================================================================" << RESET << endl;
        cout << BOLD << "   " << judul << RESET << endl;
        cout << KUNING << BOLD << "=======================================================================================================" << endl;
        
        for (size_t i = 0; i < pilihan.size(); i++) {
            if (i == posisi) {
                cout << BIRU << BOLD << "  > [ " << pilihan[i] << " ] <" << RESET << endl;
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
        else if (key == 80) { // Panah Bawah
            if (posisi < pilihan.size() - 1) posisi++;
            else posisi = 0;
        } 
        else if (key == 13 || key == 10) { 
            return posisi + 1; 
        }
    }
}

void AntriandanUpdateStatus() {
    loadPaket();
    bubbleSortStatus();
    bersihkanLayar(); 

    vector<int> indeksPaketAktif; 
    Paket *ptr = paket;

    for (int i = 0; i < jumlahPaket; i++) {
        if (paketaktif((ptr + i)->status)) {
            indeksPaketAktif.push_back(i); 
        }
    }

    if (indeksPaketAktif.empty()) {
        cout << KUNING << BOLD << "=========== STATUS GUDANG ===========" << RESET << "\n\n";
        cout << MERAH << BOLD << "TIDAK ADA ANTREAN PAKET YANG AKTIF SAAT INI." << RESET << "\n\n";
        tekanEnter();
        return;
    }

    vector<string> listPilihanMenu; 
    int nomorTabel = 1;
    for (int idx : indeksPaketAktif) {
        string statusBerikutnya = "";
        if (paket[idx].status == "Menunggu Validasi Admin") statusBerikutnya = "Diproses";
        else if (paketdiproses(paket[idx].status)) statusBerikutnya = "Dikirim";
        else if (paket[idx].status == "Dikirim") statusBerikutnya = "Selesai";

        string teksMenu = "No. " + to_string(nomorTabel) + " [" + paket[idx].resi + "] " 
                        + paket[idx].namaPengirim + " (" + paket[idx].status + " -> " + statusBerikutnya + ")";
        listPilihanMenu.push_back(teksMenu);
        nomorTabel++;
    }
    listPilihanMenu.push_back("Kembali ke Menu Admin");
    int pilihan = menuScrollUpdate("NAVIGASI PANAH UNTUK MEMILIH PAKET YANG AKAN DI-UPDATE:", listPilihanMenu, indeksPaketAktif);

    if (pilihan == listPilihanMenu.size()) {
        bersihkanLayar();
        cout << MERAH << BOLD << "UPDATE STATUS DIBATALKAN." << RESET << endl;
        tekanEnter();
        return;
    }

    int targetIndeks = indeksPaketAktif[pilihan - 1];
    Paket& pTarget = *(ptr + targetIndeks); 

    bersihkanLayar();
    cout << KUNING << BOLD << "============= PROSES UPDATE STATUS =============" << RESET << endl;
    cout << "Resi        : " << pTarget.resi << endl;
    cout << "Pengirim    : " << pTarget.namaPengirim << endl;
    cout << "Status Lama : " << BIRU << BOLD << pTarget.status << RESET << endl;
    cout << KUNING << "------------------------------------------------" << RESET << endl;

    if (pTarget.status == "Menunggu Validasi Admin") {
        if (pTarget.pembayaran == "Transfer") {
            pTarget.status = "Diproses";
            pTarget.resi = generateResiUnik(); 
            cout << HIJAU << BOLD << "VALIDASI BERHASIL -> Status: DIPROSES" << RESET << endl;
            cout << CYAN << "Resi Baru Otomatis Terbit: " << BOLD << pTarget.resi << RESET << endl;
        } else {
            cout << MERAH << "Paket COD tidak memerlukan validasi manual oleh admin." << RESET << endl;
            tekanEnter();
            return;
        }
    }
    else if (paketdiproses(pTarget.status)) {
        pTarget.status = "Dikirim";
        cout << HIJAU << BOLD << "STATUS BERHASIL DIUBAH -> DIKIRIM" << RESET << endl;
    }
    else if (pTarget.status == "Dikirim") {
        pTarget.status = "Selesai";
        cout << HIJAU << BOLD << "STATUS BERHASIL DIUBAH -> SELESAI (Paket Diterima)" << RESET << endl;
    }

    savePaket();
    cout << KUNING << "------------------------------------------------" << RESET << endl;
    cout << HIJAU << BOLD << "Data Berhasil Diperbarui di Database JSON!" << RESET << endl;
    tekanEnter();
}