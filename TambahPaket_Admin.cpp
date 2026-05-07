#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
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

void TambahPaketAdmin(Paket DaftarPaket[], int &jumlahPaket) {
    try {
        if (jumlahPaket >= MAX_PAKET) {
            throw "Jumlah paket sudah mencapai batas maksimum.";
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
        cin >> paketBaru.namaPengirim;
        cout << "Masukkan nama penerima: ";
        cin >> paketBaru.namaPenerima;
        cout << "Masukkan alamat: ";
        cin >> paketBaru.alamat;
        cout << "Masukkan berat (kg): ";
        cin >> paketBaru.berat;
        cout << "Masukkan tipe (Reguler/Express): ";
        cin >> paketBaru.tipe;
        cout << "Masukkan status: ";
        cin >> paketBaru.status;
        cout << "Masukkan pemilik: ";
        cin >> paketBaru.pemilik;
        DaftarPaket[jumlahPaket] = paketBaru;
        jumlahPaket++;
        cout << "Paket berhasil ditambahkan." << endl;
    }
    catch (const char* msg) {
        cerr << "Error: " << msg << endl;
    }
}