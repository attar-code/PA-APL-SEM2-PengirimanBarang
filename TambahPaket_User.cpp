#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <cctype>
#include "data.h"
#include "Database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern Paket paket[MAX_PAKET];
extern int jumlahPaket;
extern string userAktif;

string generateResi();
bool cekResi(Paket DaftarPaket[], int jumlahPaket, string Cari);
int inputAngka(string pesan);
long long hitungOngkir(int beratGram, int opsiLokasi, int opsiTipe);
void tekanEnter();

void savePaketToJson(Paket p);

void TambahPaket_User() {

    system("cls");

    cout << "\n=== TAMBAH PAKET ===\n";
    cout << "Nama Pengirim : ";
    getline(cin, paket[jumlahPaket].namaPengirim);

    if (paket[jumlahPaket].namaPengirim.empty()) {
        cout << "\nERROR: Nama pengirim tidak boleh kosong!\n";
        tekanEnter();
        return;
    }

    for (char c : paket[jumlahPaket].namaPengirim) {
        if (!isalpha(c) && c != ' ') {

            cout << "\nERROR: Nama pengirim hanya boleh huruf!\n";
            tekanEnter();
            return;
        }
    }

    cout << "Nama Penerima : ";
    getline(cin, paket[jumlahPaket].namaPenerima);

    if (paket[jumlahPaket].namaPenerima.empty()) {
        cout << "\nERROR: Nama penerima tidak boleh kosong!\n";
        tekanEnter();
        return;
    }

    for (char c : paket[jumlahPaket].namaPenerima) {
        if (!isalpha(c) && c != ' ') {

            cout << "\nERROR: Nama penerima hanya boleh huruf!\n";
            tekanEnter();
            return;
        }
    }

    cout << "Alamat Tujuan : ";
    getline(cin, paket[jumlahPaket].alamat);

    if (paket[jumlahPaket].alamat.empty()) {
        cout << "\nERROR: Alamat tujuan tidak boleh kosong!\n";
        tekanEnter();
        return;
    }

    cout << "\nOPSI LOKASI TUJUAN:" << endl;
    cout << "1. Dalam Kota (10.000)" << endl;
    cout << "2. Luar Kota (20.000)" << endl;

    int opsiLokasi = inputAngka("Pilih opsi lokasi tujuan (1-2): ");

    if (opsiLokasi < 1 || opsiLokasi > 2) {
        cout << "\nERROR: Pilihan lokasi hanya 1 atau 2!\n";
        tekanEnter();
        return;
    }

    cout << "\nBerat Barang (satuan gram) : ";
    cin >> paket[jumlahPaket].berat;

    if (cin.fail() || paket[jumlahPaket].berat <= 0) {
        cout << "\nERROR: Berat barang tidak valid!\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        tekanEnter();
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nPilihan tipe barang:" << endl;
    cout << "1. Dokumen (0)" << endl;
    cout << "2. Elektronik (20.000)" << endl;
    cout << "3. Pecah Belah (15.000)" << endl;
    cout << "4. Lainnya (10.000)" << endl;

    int opsiTipe = inputAngka("Pilih opsi tipe barang (1-4): ");

    if (opsiTipe < 1 || opsiTipe > 4) {
        cout << "\nERROR: Pilihan tipe barang hanya 1-4!\n";
        tekanEnter();
        return;
    }

    if (opsiTipe == 1) {
        paket[jumlahPaket].tipe = "Dokumen";
    }

    else if (opsiTipe == 2) {
        paket[jumlahPaket].tipe = "Elektronik";
    }

    else if (opsiTipe == 3) {
        paket[jumlahPaket].tipe = "Pecah Belah";
    }

    else {
        paket[jumlahPaket].tipe = "Lainnya";
    }

    paket[jumlahPaket].ongkir = hitungOngkir(
        paket[jumlahPaket].berat,
        opsiLokasi,
        opsiTipe
    );

    cout << "\nMetode Pembayaran:" << endl;
    cout << "1. COD" << endl;
    cout << "2. Transfer" << endl;

    int pembayaran = inputAngka("Pilih metode pembayaran (1-2): ");

    if (pembayaran < 1 || pembayaran > 2) {
        cout << "\nERROR: Pilihan pembayaran hanya 1 atau 2!\n";
        tekanEnter();
        return;
    }

    paket[jumlahPaket].resi = generateResi();
    paket[jumlahPaket].pemilik = userAktif;

    if (pembayaran == 1) {
        paket[jumlahPaket].pembayaran = "COD";
        paket[jumlahPaket].status = "Menunggu Diproses";

        cout << "\n=== RINCIAN PENGIRIMAN ===" << endl;
        cout << "Total Ongkir : Rp " << paket[jumlahPaket].ongkir << endl;
        cout << "Nomor Resi   : " << paket[jumlahPaket].resi << endl;
        cout << "Pembayaran   : COD" << endl;
        cout << "Status       : Menunggu Diproses" << endl;
        cout << "==========================" << endl;

        savePaketToJson(paket[jumlahPaket]);

        jumlahPaket++;

        cout << "\nPaket berhasil ditambahkan! Menunggu diproses admin.\n";

        tekanEnter();
    }

    else if (pembayaran == 2) {

        system("cls");

        cout << "=== PEMBAYARAN TRANSFER ===\n";
        cout << "Total Ongkir        : Rp " << paket[jumlahPaket].ongkir << endl;
        cout << "No. Virtual Account : 082341154761\n";

        cout << "\n1. Sudah Bayar\n";
        cout << "2. Kembali\n";

        int pilih = inputAngka("Pilih menu : ");

        if (pilih == 1) {
            paket[jumlahPaket].pembayaran = "Transfer";
            paket[jumlahPaket].status = "Menunggu Validasi Admin";

            savePaketToJson(paket[jumlahPaket]);

            jumlahPaket++;

            cout << "\nPembayaran berhasil!\n";
            cout << "Nomor Resi : " << paket[jumlahPaket - 1].resi << endl;
            cout << "Pembayaran : Transfer" << endl;
            cout << "Status     : Menunggu Validasi Admin\n";

            tekanEnter();
        }

        else if (pilih == 2) {
            cout << "\nData terhapus...\n";

            tekanEnter();
            return;
        }

        else {
            cout << "\nERROR: Menu hanya 1 atau 2!\n";

            tekanEnter();
            return;
        }
    }
}

void savePaketToJson(Paket p) {

    ifstream inputFile("Database/paket.json");

    json data;

    inputFile >> data;

    inputFile.close();

    json paketBaru = {
        {"resi", p.resi},
        {"namaPengirim", p.namaPengirim},
        {"namaPenerima", p.namaPenerima},
        {"alamat", p.alamat},
        {"berat", p.berat},
        {"tipe", p.tipe},
        {"status", p.status},
        {"pemilik", p.pemilik},
        {"ongkir", p.ongkir},
        {"pembayaran", p.pembayaran}
    };

    data.push_back(paketBaru);

    ofstream outputFile("Database/paket.json");

    outputFile << data.dump(4);

    outputFile.close();
}