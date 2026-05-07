#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>
#include "admin.h"
#include "data.h"

using namespace std;

string generateResi() {
    string kumpulanKarakter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string resi = "";
    for (int i = 0; i < 8; i++) {
        resi += kumpulanKarakter[rand() % kumpulanKarakter.length()];
    }
    return resi;
}

bool cekResi(Paket DaftarPaket[], int jumlahPaket, string Cari) {
    for (int i = 0; i < jumlahPaket; i++) {
        if (DaftarPaket[i].resi == Cari) {
            return true;
        }
    }
    return false;
}

bool ValidasiInput(string input) {
    for (char c : input) {
        // Mengizinkan: Huruf, Angka, Spasi, Titik, dan Koma
        if (!isalnum(c) && !isspace(c) && c != '.' && c != ',') {
            return false;
        }
    }
    return true;
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
        }
        if (!ValidasiInput(paketBaru.namaPengirim)) {
            throw "Nama pengirim mengandung karakter tidak valid!";
        }
        cout << "Masukkan nama penerima: ";
        getline(cin, paketBaru.namaPenerima);
        if (paketBaru.namaPenerima.empty()) {
            throw "Nama penerima tidak boleh kosong!";
        }
        if (!ValidasiInput(paketBaru.namaPenerima)) {
            throw "Nama penerima mengandung karakter tidak valid!";
        }
        cout << "Masukkan alamat: ";
        getline(cin, paketBaru.alamat);
        if (paketBaru.alamat.empty()) {
            throw "Alamat tidak boleh kosong!";
        }
        if (!ValidasiInput(paketBaru.alamat)) {
            throw "Alamat mengandung karakter tidak valid!";
        }
        cout << "Masukkan berat (gram): ";
        cin >> paketBaru.berat;
        if (cin.fail() || paketBaru.berat <= 0) {
            throw "Berat harus berupa angka positif!";
        }
        cin.ignore();
        cout << "Masukkan tipe (Reguler/Express): ";
        cin >> paketBaru.tipe;
        if (paketBaru.tipe != "Reguler" && paketBaru.tipe != "Express") {
            throw "Tipe harus 'Reguler' atau 'Express'!";
        }
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