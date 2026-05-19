#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>   
#include "../include/data.h"
#include "../database/json.hpp"
#include "../include/user.h"
#include <stdexcept>

#define RESET   "\033[0m"
#define MERAH   "\033[31m"
#define HIJAU   "\033[32m"
#define KUNING  "\033[33m"
#define BIRU    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

using json = nlohmann::json;
using namespace std;

// VARIABEL GLOBAL EXTERNAL
extern string userAktif;

// DEKLARASI FUNGSI BANTUAN
void tekanEnter();

// FUNGSI REKURSIF
int hitungData(int n) {
    if (n == 0) {
        return 0;
    }
    return 1 + hitungData(n - 1);
}

// POINTER SEBAGAI PARAMETER
void tampilJumlah(int *jumlah) {
    cout << CYAN << "Jumlah data paket di database : " << BOLD << *jumlah << RESET << endl;
}

// POINTER PADA STRUCT
void tampilResi(Paket *p) {
    cout << "Contoh akses pointer struct   : " << p->resi << endl;
}

// =========================================================================
// FUNGSI SCROLL PILIHAN KONFIRMASI (VERSI LIST VERTIKAL)
// =========================================================================
int menuScrollKonfirmasiHapus(string judul, vector<string> pilihan) {
    int posisi = 0;
    while (true) {
        bersihkanLayar();
        cout << KUNING << BOLD << "================ KONFIRMASI PENGHAPUSAN ================\n" << RESET;
        cout << BOLD << judul << RESET << "\n\n";

        for (size_t i = 0; i < pilihan.size(); i++) {
            if (i == (size_t)posisi) {
                cout << MERAH << BOLD << "  > " << pilihan[i] << " <" << RESET << "\n";
            } else {
                cout << "    " << pilihan[i] << "\n";
            }
        }
        cout << "--------------------------------------------------------\n";
        cout << " [Gunakan Panah Atas/Bawah & Tekan Enter untuk Memilih]\n";

        int key = _getch();
        if (key == 27) { 
            _getch(); key = _getch(); 
            if (key == 65) key = 72; 
            else if (key == 66) key = 80; 
        } else if (key == 224) { 
            key = _getch(); 
        }

        if (key == 72) { 
            if (posisi > 0) posisi--;
            else posisi = pilihan.size() - 1;
        } else if (key == 80) { 
            if (posisi < (int)pilihan.size() - 1) posisi++;
            else posisi = 0;
        } else if (key == 13 || key == 10) { 
            return posisi + 1; 
        }
    }
}

// =========================================================================
// FUNGSI TAMPILAN INTERAKTIF MENU SCROLL TABEL UTAMA (HAPUS RIWAYAT)
// =========================================================================
int menuScrollTabelHapus(string judul, const vector<int>& indeksPaketUser, const json& dataJson) {
    int posisi = 0;
    size_t totalPilihan = indeksPaketUser.size() + 1; 

    while (true) {
        bersihkanLayar();
        cout << KUNING << BOLD << "======================================= HAPUS RIWAYAT PAKET USER =======================================\n" << RESET;
        cout << judul << "\n";
        cout << "----------------------------------------------------------------------------------------------------------------\n";
        cout << left << "   " << setw(5) << "NO" << setw(15) << "RESI" << setw(22) << "PENGIRIM" << setw(22) << "PENERIMA" << setw(18) << "STATUS" << setw(15) << "ONGKIR" << "\n";
        cout << "----------------------------------------------------------------------------------------------------------------\n";

        for (size_t i = 0; i < indeksPaketUser.size(); i++) {
            int idx = indeksPaketUser[i];
            string resi = dataJson[idx].value("resi", "-");
            string pengirim = dataJson[idx].value("namaPengirim", "-");
            string penerima = dataJson[idx].value("namaPenerima", "-");
            string status = dataJson[idx].value("status", "-");
            long long ongkir = dataJson[idx].value("ongkir", 0LL);

            if (pengirim.length() > 18) pengirim = pengirim.substr(0, 15) + "...";
            if (penerima.length() > 18) penerima = penerima.substr(0, 15) + "...";

            if (i == (size_t)posisi) {
                cout << HIJAU << BOLD << " > " 
                     << left << setw(5)  << (i + 1)
                     << setw(15) << resi 
                     << setw(22) << pengirim 
                     << setw(22) << penerima 
                     << setw(18) << status
                     << "Rp " << setw(12) << ongkir << " <" << RESET << "\n";
            } else {
                cout << "   " 
                     << left << setw(5)  << (i + 1)
                     << setw(15) << resi 
                     << setw(22) << pengirim 
                     << setw(22) << penerima 
                     << BIRU << setw(18) << status << RESET
                     << HIJAU << "Rp " << setw(12) << ongkir << RESET << "\n";
            }
        }
        cout << "----------------------------------------------------------------------------------------------------------------\n";

        if ((size_t)posisi == indeksPaketUser.size()) {
            cout << MERAH << BOLD << " > [KEMBALI KE MENU UTAMA] <" << RESET << "\n";
        } else {
            cout << "   [KEMBALI KE MENU UTAMA]\n";
        }
        cout << "----------------------------------------------------------------------------------------------------------------\n";
        cout << " " << BOLD << "Petunjuk:" << RESET << " Gunakan tombol " << CYAN << "Panah Atas / Bawah" << RESET << " lalu tekan " << CYAN << "Enter" << RESET << " untuk memilih.\n";

        int key = _getch();
        if (key == 27) { 
            _getch(); key = _getch(); 
            if (key == 65) key = 72;      
            else if (key == 66) key = 80; 
        } else if (key == 224) { 
            key = _getch(); 
        }

        if (key == 72) { 
            if (posisi > 0) posisi--;
            else posisi = totalPilihan - 1;
        } else if (key == 80) { 
            if (posisi < (int)totalPilihan - 1) posisi++;
            else posisi = 0;
        } else if (key == 13 || key == 10) { 
            return posisi + 1; 
        }
    }
}

// =========================================================================
// PROSEDUR UTAMA
// =========================================================================
void HapusRiwayatPaket_User() {
    bersihkanLayar();
    json data;

    // EXCEPTION HANDLING
    try {
        // FILE HANDLING
        ifstream inputFile("database/paket.json");

        if (!inputFile.is_open()) {
            throw runtime_error("File paket.json tidak ditemukan!");
        }

        if (inputFile.peek() == ifstream::traits_type::eof()) {
            throw runtime_error("Data paket kosong!");
        }

        inputFile >> data;
        inputFile.close();
    }
    catch (exception &e) {
        cout << MERAH << BOLD << "\nError : " << e.what() << RESET << endl;
        tekanEnter();
        return;
    }

    // FILTER STRATEGIS: Hanya tampilkan paket milik user aktif yang sudah "Selesai" atau "Dibatalkan"
    vector<int> indeksPaketUser;
    for (size_t i = 0; i < data.size(); i++) {
        string status = data[i].value("status", "-");
        if (data[i]["pemilik"] == userAktif && (status == "Selesai" || status == "Dibatalkan")) {
            indeksPaketUser.push_back(i);
        }
    }

    if (indeksPaketUser.empty()) {
        cout << KUNING << BOLD << "================ HAPUS RIWAYAT PAKET ================\n" << RESET;
        cout << MERAH << BOLD << "\nTidak ditemukan riwayat paket berstatus 'Selesai' atau 'Dibatalkan' yang dapat dihapus.\n" << RESET;
        tekanEnter();
        return;
    }

    // Panggil menu tabel scroll interaktif langsung
    int pilihanTabel = menuScrollTabelHapus("PILIH DATA RIWAYAT PAKET YANG INGIN DIHAPUS PERMANEN:", indeksPaketUser, data);

    // Opsi Kembali ke Menu Utama
    if (pilihanTabel == (int)indeksPaketUser.size() + 1) {
        bersihkanLayar();
        cout << MERAH << BOLD << "PROSES PENGHAPUSAN RIWAYAT PAKET DIBATALKAN." << RESET << endl;
        tekanEnter();
        return;
    }

    int targetIndeks = indeksPaketUser[pilihanTabel - 1];
    auto &paketTarget = data[targetIndeks];

    // DETAIL DATA TARGET SEBELUM DIHAPUS (PUTIH & LURUS PRESISI PAKAI LEFT)
    bersihkanLayar();
    cout << KUNING << BOLD << "================ DETAIL RIWAYAT PAKET YANG AKAN DIHAPUS ================" << RESET << endl;
    cout << left << setw(18) << " Nomor Resi"     << " : " << BOLD << paketTarget["resi"] << RESET << "\n";
    cout << left << setw(18) << " Nama Pengirim"   << " : " << paketTarget["namaPengirim"] << "\n";
    cout << left << setw(18) << " Nama Penerima"   << " : " << paketTarget["namaPenerima"] << "\n";
    cout << left << setw(18) << " Alamat Tujuan"   << " : " << paketTarget["alamat"] << "\n";
    cout << left << setw(18) << " Berat Barang"    << " : " << paketTarget["berat"] << " gram\n";
    cout << left << setw(18) << " Tipe Barang"     << " : " << paketTarget["tipe"] << "\n";
    cout << left << setw(18) << " Status Paket"    << " : " << MERAH << BOLD << paketTarget["status"] << RESET << "\n";
    cout << KUNING << "========================================================================" << RESET << endl;
    tekanEnter();

    // PANGGIL MENU SCROLL PILIHAN YA / TIDAK
    vector<string> opsiHapus = {"Ya, Hapus Permanen Riwayat Ini", "Tidak, Kembali"};
    int konfirmasi = menuScrollKonfirmasiHapus("Apakah Anda yakin ingin menghapus data ini dari riwayat secara permanen?", opsiHapus);

    if (konfirmasi == 1) {
        // Hapus data dari struktur json secara berurutan sesuai index asli di data
        data.erase(data.begin() + targetIndeks);

        // FILE HANDLING - SIMPAN PERUBAHAN KE FILE JSON
        ofstream outputFile("database/paket.json");
        outputFile << data.dump(4);
        outputFile.close();

        bersihkanLayar();
        cout << KUNING << BOLD << "=====================================================" << RESET << endl;
        cout << HIJAU << BOLD  << "      SUKSES! RIWAYAT PAKET BERHASIL DIHAPUS!        " << RESET << endl;
        cout << KUNING << BOLD << "=====================================================" << RESET << endl;
        cout << left << setw(18) << " Info Status"    << " : " << MERAH << BOLD << "DATA TERHAPUS DARI DATABASE" << RESET << "\n";
        cout << KUNING << BOLD << "=====================================================" << RESET << endl;
    } 
    else {
        cout << KUNING << "\nPenghapusan riwayat paket dibatalkan oleh pengguna." << RESET << endl;
    }

    // BLOCK MATERI KULIAH POINTER & REKURSIF
    cout << "\n--------------------------------------------------------\n";
    int jumlahData = data.size();
    tampilJumlah(&jumlahData);

    cout << CYAN << "Total perhitungan data (Rekursif): " << BOLD << hitungData(jumlahData) << RESET << endl;
    
    tampilPesan("Fitur hapus riwayat paket selesai dieksekusi.");
    tekanEnter();
}