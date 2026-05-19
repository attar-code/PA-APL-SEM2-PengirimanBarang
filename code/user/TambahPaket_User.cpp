// 

#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include <vector>    // 🌟 Wajib untuk menampung daftar pilihan menu scroll
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
string generateResi();
bool cekResiDiJson(const string& Cari);
long long hitungOngkir(long beratGram, int opsiLokasi, int opsiTipe);
void tekanEnter();
void savePaketToJson(Paket p);

// FUNGSI VALIDASI
bool ValidasiInputUser(const string& input) {
    if (input.empty()) {
        return false;
    }
    for (char c : input) {
        if (!isalnum(c) && !isspace(c) && c != '.' && c != ',') {
            return false;
        }
    }
    return true;
}

// FUNGSI VALIDASI FIELD
void cekValiditasInputUser(const string& input, const string& namaField) {
    if (input.empty()) {
        throw (namaField + " tidak boleh kosong!").c_str();
    }
    if (!ValidasiInputUser(input)) {
        throw (namaField + " mengandung karakter tidak valid!").c_str();
    }
}

// =========================================================================
// 🌟 FUNGSI NAVIGASI: menuScrollInput (Sederhana, Bersih, Tanpa Pointer)
// =========================================================================
int menuScrollInput(string judul, vector<string> pilihan) {
    int posisi = 0;
    int key;
    
    while (true) {
        bersihkanLayar(); // Bersihkan layar agar tidak bertelur teksnya
        
        cout << CYAN << BOLD << "==================================================" << RESET << endl;
        cout << BOLD << "   " << judul << RESET << endl;
        cout << CYAN << BOLD << "==================================================" << RESET << endl;
        
        for (size_t i = 0; i < pilihan.size(); i++) {
            if (i == posisi) {
                cout << KUNING << BOLD << "  > [ " << pilihan[i] << " ] <" << RESET << endl;
            } else {
                cout << "      " << pilihan[i] << endl;
            }
        }
        cout << CYAN << "--------------------------------------------------" << RESET << endl;
        cout << "Gunakan Panah Atas/Bawah & Enter" << endl;

        key = _getch();

        // Antisipasi tombol panah di Mac / Linux / Windows
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
            return posisi + 1; // Mengembalikan 1, 2, dst.
        }
    }
}

// FUNGSI HITUNG ONGKIR
long long hitungOngkir(long beratGram, int opsiLokasi, int opsiTipe) {
    long long hargaPerKg = (opsiLokasi == 1) ? 10000 : 20000;
    long long tambahanTipe = 0;
    
    switch (opsiTipe) {
        case 1: tambahanTipe = 0; break;
        case 2: tambahanTipe = 20000; break;
        case 3: tambahanTipe = 15000; break;
        case 4: tambahanTipe = 10000; break;
    }
    return ((beratGram / 1000.0) * hargaPerKg) + tambahanTipe;
}

// PROSEDUR UTAMA: TambahPaket_User (VERSI UPGRADE SCROLL MENU)
// =========================================================================
void TambahPaket_User() {
    try {
        static bool seeded = false;
        if (!seeded) {
            srand(time(0));
            seeded = true;
        }

        bersihkanLayar();
        cout << CYAN << BOLD << "==================================================" << RESET << endl;
        cout << BOLD << "                 TAMBAH PAKET BARU                " << RESET << endl;
        cout << CYAN << BOLD << "==================================================" << RESET << "\n\n";
        
        Paket paketBaru;

        // 1. INPUT DATA TEKS
        cout << " Nama Pengirim : ";
        getline(cin, paketBaru.namaPengirim);
        cekValiditasInputUser(paketBaru.namaPengirim, "Nama pengirim");

        cout << " Nama Penerima : ";
        getline(cin, paketBaru.namaPenerima);
        cekValiditasInputUser(paketBaru.namaPenerima, "Nama penerima");

        cout << " Alamat Tujuan : ";
        getline(cin, paketBaru.alamat);
        cekValiditasInputUser(paketBaru.alamat, "Alamat");

        // 2. INPUT LOKASI (Ubah ke Scroll)
        vector<string> menuLokasi = {
            "Dalam Kota (Rp 10.000 / Kg)",
            "Luar Kota  (Rp 20.000 / Kg)"
        };
        int opsiLokasi = menuScrollInput("PILIH LOKASI TUJUAN PAKET:", menuLokasi);
        paketBaru.lokasi = (opsiLokasi == 1) ? "Dalam Kota" : "Luar Kota";

        // 3. INPUT BERAT
        bersihkanLayar();
        cout << CYAN << BOLD << "==================================================" << RESET << endl;
        cout << BOLD << "                 INPUT BERAT PAKET                " << RESET << endl;
        cout << CYAN << BOLD << "==================================================" << RESET << "\n\n";
        cout << " Masukkan berat paket (gram): ";
        cin >> paketBaru.berat;

        if (cin.fail() || paketBaru.berat <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            throw "Berat harus angka positif!";
        }
        if (paketBaru.berat > 50000) {
            throw "Berat melebihi batas maksimal (50 Kg)!";
        }

        // 4. INPUT TIPE BARANG (Ubah ke Scroll)
        vector<string> menuTipe = {
            "Dokumen",
            "Elektronik (+Rp 20.000)",
            "Pecah Belah (+Rp 15.000)",
            "Lainnya     (+Rp 10.000)"
        };
        int opsiTipe = menuScrollInput("PILIH KATEGORI / TIPE BARANG:", menuTipe);
        
        if (opsiTipe == 1) paketBaru.tipe = "Dokumen";
        else if (opsiTipe == 2) paketBaru.tipe = "Elektronik";
        else if (opsiTipe == 3) paketBaru.tipe = "Pecah Belah";
        else paketBaru.tipe = "Lainnya";

        // HITUNG ONGKIR otomatis
        paketBaru.ongkir = hitungOngkir(paketBaru.berat, opsiLokasi, opsiTipe);

        // 5. PILIHAN METODE PEMBAYARAN (Ubah ke Scroll)
        vector<string> menuBayar = {
            "COD (Bayar di Tempat)",
            "Transfer Bank (Perlu Validasi Admin)"
        };
        int metodeBayar = menuScrollInput("PILIH METODE PEMBAYARAN:", menuBayar);

        paketBaru.pemilik = userAktif;

        bersihkanLayar();
        cout << HIJAU << BOLD << "==================================================" << RESET << endl;
        cout << HIJAU << BOLD << "               RINGKASAN TRANSAKSI                " << RESET << endl;
        cout << HIJAU << BOLD << "==================================================" << RESET << endl;
        cout << " Pengirim   : " << paketBaru.namaPengirim << endl;
        cout << " Penerima   : " << paketBaru.namaPenerima << endl;
        cout << " Tujuan     : " << paketBaru.lokasi << " (" << paketBaru.alamat << ")" << endl;
        cout << " Tipe       : " << paketBaru.tipe << " (" << paketBaru.berat << "g)" << endl;
        cout << " Total Tarif: " << KUNING << BOLD << "Rp " << paketBaru.ongkir << RESET << endl;
        cout << " Pembayaran : " << BOLD << (metodeBayar == 1 ? "COD" : "Transfer") << RESET << endl;
        cout << HIJAU << "--------------------------------------------------" << RESET << endl;

        // Eksekusi Simpan Berdasarkan Metode Pembayaran
        if (metodeBayar == 1) { // COD
            paketBaru.pembayaran = "COD";
            paketBaru.status = "Diproses";

            do {
                paketBaru.resi = generateResi();
            } while (cekResiDiJson(paketBaru.resi));

            savePaketToJson(paketBaru);

            cout << HIJAU << BOLD << " Transaksi Berhasil Dibuat!" << RESET << endl;
            cout << " Nomor Resi Anda : " << CYAN << BOLD << paketBaru.resi << RESET << endl;
        } 
        else { // TRANSFER
            paketBaru.pembayaran = "Transfer";
            paketBaru.status = "Menunggu Validasi Admin";
            paketBaru.resi = "BELUM_RILIS";

            savePaketToJson(paketBaru);

            cout << KUNING << BOLD << " Transaksi Berhasil Dikirim!" << RESET << endl;
            cout << " Status: Menunggu Validasi Pembayaran oleh Admin.\n";
        }
        
        cout << HIJAU << "==================================================" << RESET << "\n\n";
        cin.ignore();
        tekanEnter();
    }
    catch (const char* msg) {
        cout << MERAH << BOLD << "\n==================================================" << RESET << endl;
        cerr << MERAH << BOLD << " [ERROR]: " << msg << RESET << endl;
        cout << MERAH << BOLD << "==================================================" << RESET << "\n\n";
        cin.ignore();
        tekanEnter();
    }
}

// PROSEDUR SIMPAN JSON (Murni Konvensional & Simpel)
void savePaketToJson(Paket p) {
    ifstream inputFile("database/paket.json");
    json data = json::array();

    if (inputFile.is_open()) {
        if (inputFile.peek() != ifstream::traits_type::eof()) {
            inputFile >> data;
        }
        inputFile.close();
    }

    json paketBaru = {
        {"resi", p.resi},
        {"namaPengirim", p.namaPengirim},
        {"namaPenerima", p.namaPenerima},
        {"alamat", p.alamat},
        {"lokasi", p.lokasi},
        {"berat", p.berat},
        {"tipe", p.tipe},
        {"status", p.status},
        {"pemilik", p.pemilik},
        {"ongkir", p.ongkir},
        {"pembayaran", p.pembayaran}
    };

    data.push_back(paketBaru);
    ofstream outputFile("database/paket.json");
    outputFile << data.dump(4);
    outputFile.close();
}