// #include <iostream>
// #include <cctype>
// #include "../include/data.h"
// #include "../include/admin.h"
// #include "../database/json.hpp"

// // Tambahkan makro warna agar konsisten dengan menu tambah paket
// #define RESET "\033[0m"
// #define MERAH "\033[31m"
// #define HIJAU "\033[32m"
// #define KUNING "\033[33m"
// #define BIRU "\033[34m"
// #define CYAN "\033[36m"
// #define BOLD "\033[1m"

// using json = nlohmann::json;
// using namespace std;

// bool paketdiproses(string status) {
//     return (
//         status == "Diproses" ||
//         status == "Diproses (COD)" ||
//         status == "Diproses (Lunas)"
//     );
// }

// bool paketaktif(string status) {
//     return (
//         status == "Menunggu Validasi Admin" ||
//         status == "Diproses" ||
//         status == "Diproses (COD)" ||
//         status == "Diproses (Lunas)" ||
//         status == "Dikirim"
//     );
// }

// bool resiSudahAda(string resi){

//     for(int i=0;i<jumlahPaket;i++){
//         if(paket[i].resi==resi){
//             return true;
//         }
//     }
//     return false;
// }

// // =========================================================================
// // FUNGSI REKURSIF: Mengacak resi baru menggunakan pengecekan array global
// // =========================================================================
// string generateResiUnik() {
//     // 1. Langkah Awal: Acak nomor resi baru sepanjang 8 karakter
//     string resiBaru = generateResi(); 
//     // 2. BASE CASE (Kondisi Berhenti): 
//     // Jika resi belum ada di array global, langsung kembalikan (Fungsi Selesai)
//     if (!resiSudahAda(resiBaru)) {
//         return resiBaru;
//     }
//     // 3. RECURSIVE STEP (Pemanggilan Diri Sendiri): 
//     // Jika resi sudah ada (kembar/bentrok), panggil fungsi ini lagi untuk mengacak ulang dari awal
//     cout << KUNING << "[Sistem] Resi kembar terdeteksi di memori RAM! Melakukan rekursi..." << RESET << endl;
//     return generateResiUnik(); 
// }
// int prioritasStatus(string status){

//     if(status == "Menunggu Validasi Admin")
//         return 1;

//     else if(
//         status == "Diproses" ||
//         status == "Diproses (COD)" ||
//         status == "Diproses (Lunas)")
//         return 2;
//     else if(status == "Dikirim")
//         return 3;
//     else if(status == "Selesai")
//         return 4;
//     else if(status == "Dibatalkan")
//         return 5;
//     return 999;
// }


// void bubbleSortStatus(){

//     for(int i=0;i<jumlahPaket-1;i++){

//         for(int j=0;
//             j<jumlahPaket-i-1;
//             j++)
//         {

//             if(
//             prioritasStatus(
//                 paket[j].status
//             )
//             >
//             prioritasStatus(
//                 paket[j+1].status
//             )
//             ){
//                 swap(
//                     paket[j],
//                     paket[j+1]
//                 );
//             }
//         }
//     }
// }

// void AntriandanUpdateStatus() {
//     loadPaket();
//     bubbleSortStatus();
//     bersihkanLayar(); // Diganti ke fungsi bawaan data.h agar aman di Mac
//     string resiCari;
//     int menunggu = 0;
//     int diproses = 0;
//     int dikirim = 0;
//     int selesai = 0;

//     Paket *ptr = paket;
//     // menghitung status yg ada
//     for (int i = 0; i < jumlahPaket; i++) {
//         if ((ptr + i)->status == "Menunggu Validasi Admin") {
//             menunggu++;
//         }
//         else if (paketdiproses((ptr + i)->status)) {
//             diproses++;
//         }
//         else if ((ptr + i)->status == "Dikirim") {
//             dikirim++;
//         }
//         else if ((ptr + i)->status == "Selesai") {
//             selesai++;
//         }
//     }

//     cout << KUNING << BOLD << "=========== STATUS GUDANG ===========" << RESET << "\n\n";
//     cout << "Menunggu Validasi Admin : " << BOLD << menunggu << RESET << " paket\n";
//     cout << "Diproses                : " << BOLD << diproses << RESET << " paket\n";
//     cout << "Dikirim                 : " << BOLD << dikirim << RESET << " paket\n";
//     cout << "Selesai                 : " << BOLD << selesai << RESET << " paket\n";
//     cout << KUNING << BOLD << "=====================================" << RESET << endl;
//     cout << BOLD << "            SEMUA PAKET              " << RESET << endl;
//     cout << KUNING << BOLD << "=====================================" << RESET << "";
//     for (int i = 0; i < jumlahPaket; i++) {
//         cout << "\n" << CYAN << "Resi       : " << RESET << BOLD << (ptr + i)->resi << RESET << endl;
//         cout << "Pengirim   : " << (ptr + i)->namaPengirim << endl;
//         cout << "Penerima   : " << (ptr + i)->namaPenerima << endl;
//         cout << "Pembayaran : " << (ptr + i)->pembayaran << endl;
//         cout << "Status     : " << BIRU << BOLD << (ptr + i)->status << RESET << endl;
//         cout << KUNING << "-------------------------------------" << RESET << endl;
//     }
//     // DAFTAR AKTIF
//     cout << KUNING << BOLD << "=====================================" << RESET << endl;
//     cout << BOLD << "            PAKET AKTIF              " << RESET << endl;
//     cout << KUNING << BOLD << "=====================================" << RESET << endl;
//     bool ada = false;
//     for (int i = 0; i < jumlahPaket; i++) {
//         if (paketaktif((ptr + i)->status)) {
//             ada = true;
//             cout << "\n" << CYAN << "Resi     : " << RESET << BOLD << (ptr + i)->resi << RESET << endl;
//             cout << "Pengirim : " << (ptr + i)->namaPengirim << endl;
//             cout << "Status   : " << HIJAU << BOLD << (ptr + i)->status << RESET << endl;
//         }
//     }
//     if (!ada) {
//         cout << "\n" << KUNING << "Tidak ada paket aktif." << RESET << "\n";
//         tekanEnter();
//         return;
//     }

//     // INPUT RESI

//     cout << endl << MERAH << BOLD << "KETIK 'BATAL' UNTUK KELUAR" << RESET << endl;
//     cout << BOLD << "Masukkan resi paket: " << RESET;
//     cin >> resiCari;
//     for(char &huruf : resiCari){
//         huruf = toupper(huruf);
//     }
//     if (resiCari == "BATAL"){
//         cout << KUNING << BOLD << "=====================================" << RESET << endl;
//         cout << MERAH << BOLD << "         UPDATE STATUS BATAL         " << RESET << endl;
//         cout << KUNING << BOLD << "=====================================" << RESET << endl;
//         tekanEnter();
//         return;
//     }
//     for (int i = 0; i < jumlahPaket; i++) {
//         if ((ptr + i)->resi == resiCari) {
//             string statusLama = (ptr + i)->status;
//             // validasi admin -> diproses
//             if ((ptr + i)->status == "Menunggu Validasi Admin") {
//                 if ((ptr + i)->pembayaran == "Transfer") {
//                     (ptr + i)->status = "Diproses";
//                     /* untuk paket menunggu validasi admin belum mndpt resi
//                         di sini baru dapat resi dengan syarat admin menginput "BELUM_RILIS" 
//                         nanti dia akan menyesuaikan antrian (yang input paket dluan yang akan di
//                         validasi admin dluan). setelah di vallidasi admin baru akan mendapat resi baru*/
//                     (ptr + i)->resi = generateResiUnik();
//                     cout << "\n" << HIJAU << BOLD << "VALIDASI BERHASIL -> DIPROSES" << RESET << "\n";
//                     cout << CYAN << "Resi baru: " << RESET << BOLD << (ptr + i)->resi << RESET << endl;
//                 }
//                 else {
//                     cout << "\n" << MERAH << "COD tidak perlu validasi admin." << RESET << "\n";
//                 }
//             }

//             // ===============================
//             // 2. DIPROSES → DIKIRIM
//             // ===============================
//             else if (paketdiproses((ptr + i)->status)) {
//                 (ptr + i)->status = "Dikirim";
//                 cout << "\n" << HIJAU << BOLD << "Status berubah: DIPROSES -> DIKIRIM" << RESET << "\n";
//             }
//             // ===============================
//             // 3. DIKIRIM → SELESAI
//             // ===============================
//             else if ((ptr + i)->status == "Dikirim") {
//                 (ptr + i)->status = "Selesai";
//                 cout << "\n" << HIJAU << BOLD << "Status berubah: DIKIRIM -> SELESAI" << RESET << "\n";
//             }

//             else {
//                 cout << "\n" << MERAH << BOLD << "Status tidak bisa diubah lagi!" << RESET << "\n";
//                 tekanEnter();
//                 return;
//             }
//             savePaket();
//             cout << "\n" << HIJAU << BOLD << "Update berhasil untuk resi: " << RESET << BOLD << resiCari << RESET << endl;
//             tekanEnter();
//             return;
//         }
//     }

//     cout << "\n" << MERAH << BOLD << "Resi tidak ditemukan!" << RESET << "\n";
//     tekanEnter();
// }
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

int _getch(); 
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
    else if(status == "Diproses" || status == "Diproses (COD)" || status == "Diproses (Lunas)") 
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