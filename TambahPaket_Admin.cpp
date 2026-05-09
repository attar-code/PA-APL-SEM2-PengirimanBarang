#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>
#include "admin.h"
#include "data.h"

using namespace std;
// Fungsi untuk menghasilkan nomor resi unik
string generateResi() {
    string kumpulanKarakter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string resi = "";
    for (int i = 0; i < 8; i++) {
        resi += kumpulanKarakter[rand() % kumpulanKarakter.length()];
    }
    return resi;
}
// Fungsi untuk mengecek apakah nomor resi sudah ada
bool cekResi(Paket DaftarPaket[], int jumlahPaket, string Cari) {
    for (int i = 0; i < jumlahPaket; i++) {
        if (DaftarPaket[i].resi == Cari) {
            return true;
        }
    }
    return false;
}
// Fungsi untuk validasi input (hanya mengizinkan huruf, angka, spasi, titik, dan koma)
bool ValidasiInput(string input) {
    for (char c : input) {
        // Mengizinkan: Huruf, Angka, Spasi, Titik, dan Koma
        if (!isalnum(c) && !isspace(c) && c != '.' && c != ',') {
            return false;
        }
    }
    return true;
}
// Fungsi untuk menghitung ongkir berdasarkan berat, lokasi, dan tipe barang
long long hitungOngkir(int beratGram, int opsiLokasi, int opsiTipe) {
    long long hargaPerKg;
    long long tambahanTipe;

    // 1. Penentuan Harga per Kg berdasarkan lokasi
    if (opsiLokasi == 1) {
        hargaPerKg = 10000; // Dalam Kota
    } else {
        hargaPerKg = 20000; // Luar Kota
    }

    // 2. Logika Biaya Tambahan Tipe Barang
    switch (opsiTipe) {
        case 1: tambahanTipe = 0; break;      // Dokumen
        case 2: tambahanTipe = 20000; break;  // Elektronik (Sesuai menu kalian)
        case 3: tambahanTipe = 15000; break;  // Pecah Belah (Sesuai menu kalian)
        case 4: tambahanTipe = 10000; break;  // Lainnya
        default: tambahanTipe = 0; break;
    }

    // 3. Hitung Total
    long long total = (long long)((beratGram / 1000.0) * hargaPerKg) + tambahanTipe;
    return total;
}

void TambahPaketAdmin(Paket DaftarPaket[], int &jumlahPaket) {
    try {
        if (jumlahPaket >= MAX_PAKET) {
            throw "Gudang Penuh!";
        }
        cout << "=== TAMBAH PAKET (ADMIN) ===" << endl;
        static bool seeded = false;
        if (!seeded) {
            srand(time(0));
            seeded = true;
        }
        string resiBaru = generateResi();
        do {
            resiBaru = generateResi();
        } while (cekResi(DaftarPaket, jumlahPaket, resiBaru));
        cin.ignore();
        Paket paketBaru;
        paketBaru.resi = resiBaru;
        cout << "Resi otomatis: " << paketBaru.resi << endl;

        cout << "Masukkan nama pengirim: ";
        getline(cin, paketBaru.namaPengirim);
        if (paketBaru.namaPengirim.empty()) {
            throw "Nama pengirim tidak boleh kosong!";
            tekanEnter();
            return;
        }
        if (!ValidasiInput(paketBaru.namaPengirim)) {
            throw "Nama pengirim mengandung karakter tidak valid!";
            tekanEnter();
            return;
        }
        
        cout << "Masukkan nama penerima: ";
        getline(cin, paketBaru.namaPenerima);
        if (paketBaru.namaPenerima.empty()) {
            throw "Nama penerima tidak boleh kosong!";
            tekanEnter();
            return;
        }
        if (!ValidasiInput(paketBaru.namaPenerima)) {
            throw "Nama penerima mengandung karakter tidak valid!";
            tekanEnter();
            return;
        }
        cout << "Masukkan alamat: ";
        getline(cin, paketBaru.alamat);
        if (paketBaru.alamat.empty()) {
            throw "Alamat tidak boleh kosong!";
            tekanEnter();
            return;
        }
        if (!ValidasiInput(paketBaru.alamat)) {
            throw "Alamat mengandung karakter tidak valid!";
            tekanEnter();
            return;
        }

        cout << "OPSI LOKASI TUJUAN:" << endl;
        cout << "1. Dalam Kota (10.000)" << endl;
        cout << "2. Luar Kota (20.000)" << endl;
        int opsiLokasi;
        while (true) {
            cout << "Pilih opsi lokasi tujuan (1-2): ";
            cin >> opsiLokasi;
            if (cin.fail() || (opsiLokasi != 1 && opsiLokasi != 2)) {
                cout << "Input tidak valid! Masukkan angka 1 atau 2." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            } else {
                break;
            }
        }

        cout << "Masukkan berat (gram) (1000gram x 10.000rb): ";
        cin >> paketBaru.berat;
        if (cin.fail() || paketBaru.berat <= 0) {
            throw "Berat harus berupa angka positif!";
        }
        cin.ignore();
        cout << "Pilihan tipe barang:" << endl;
        cout << "1. Dokumen (0)" << endl;
        cout << "2. Elektronik (20.000)" << endl;
        cout << "3. Pecah Belah (15.000)" << endl;
        cout << "4. Lainnya (10.000)" << endl;
        int opsiTipe;
        while (true) {
            cout << "Pilih opsi tipe barang (1-4): ";
            cin >> opsiTipe;
            if (cin.fail() || opsiTipe < 1 || opsiTipe > 4) {
                cout << "Input tidak valid! Masukkan angka antara 1 dan 4." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            } else {
                break;
            }
        }

        if (opsiTipe == 1) paketBaru.tipe = "Dokumen";
        else if (opsiTipe == 2) paketBaru.tipe = "Elektronik";
        else if (opsiTipe == 3) paketBaru.tipe = "Pecah Belah";
        else paketBaru.tipe = "Lainnya";

        paketBaru.ongkir = hitungOngkir(paketBaru.berat, opsiLokasi, opsiTipe);

        cout << "\n===============================" << endl;
        cout << "Tipe Barang  : " << paketBaru.tipe << endl;
        cout << "Total Ongkir : Rp " << paketBaru.ongkir << endl;
        cout << "===============================" << endl;

        paketBaru.status = "Diproses";
        paketBaru.pemilik = "Admin";
        
        DaftarPaket[jumlahPaket] = paketBaru;
        jumlahPaket++;
        
        cout << "Paket berhasil ditambahkan." << endl;
        tekanEnter();
    }
    catch (const char* msg) {
        cerr << "Error: " << msg << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        tekanEnter();
    }
}