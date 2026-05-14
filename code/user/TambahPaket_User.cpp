#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

//VARIABEL GLOBAL
//mengambil data user login
extern string userAktif;

//DEKLARASI FUNGSI
//Materi: Fungsi
string generateResi();
int inputAngka(string pesan);
bool cekResiDiJson(const string& Cari);
long long hitungOngkir(long beratGram, int opsiLokasi, int opsiTipe);
void tekanEnter();
void savePaketToJson(Paket p);

//FUNGSI VALIDASI
//Materi: Fungsi
bool ValidasiInputUser(const string& input) {

    if (input.empty()) {
        return false;
    }

    for (char c : input) {

        if (
            !isalnum(c) &&
            !isspace(c) &&
            c != '.' &&
            c != ','
        ) {

            return false;
        }
    }

    return true;
}

//FUNGSI VALIDASI FIELD
//Materi: Fungsi
void cekValiditasInputUser(
    const string& input,
    const string& namaField
) {

    //ERROR HANDLING
    if (input.empty()) {
        throw (namaField + " tidak boleh kosong!").c_str();
    }

    if (!ValidasiInputUser(input)) {
        throw (namaField + " mengandung karakter tidak valid!").c_str();
    }
}

//FUNGSI HITUNG ONGKIR
//Materi: Fungsi
long long hitungOngkir(
    long beratGram,
    int opsiLokasi,
    int opsiTipe
) {

    long long hargaPerKg;

    if (opsiLokasi == 1) {
        hargaPerKg = 10000;
    }

    else {
        hargaPerKg = 20000;
    }

    long long tambahanTipe = 0;

    switch (opsiTipe) {
        case 1:
            tambahanTipe = 0;
            break;

        case 2:
            tambahanTipe = 20000;
            break;

        case 3:
            tambahanTipe = 15000;
            break;

        case 4:
            tambahanTipe = 10000;
            break;
    }

    return ((beratGram / 1000.0) * hargaPerKg) + tambahanTipe;
}

//PROSEDUR TAMBAH PAKET
//Materi: Prosedur
void TambahPaket_User() {

    //EXCEPTION HANDLING
    try {
        static bool seeded = false;

        if (!seeded) {
            srand(time(0));
            seeded = true;
        }

        system("cls");

        cout << "=== TAMBAH PAKET ===\n" << endl;
        //STRUCT + VARIABEL LOKAL
        Paket paketBaru;

        //INPUT DATA
        cout << "Nama Pengirim : ";
        getline(cin, paketBaru.namaPengirim);

        cekValiditasInputUser(
            paketBaru.namaPengirim,
            "Nama pengirim"
        );

        cout << "Nama Penerima : ";
        getline(cin, paketBaru.namaPenerima);

        cekValiditasInputUser(
            paketBaru.namaPenerima,
            "Nama penerima"
        );

        cout << "Alamat Tujuan : ";
        getline(cin, paketBaru.alamat);

        cekValiditasInputUser(
            paketBaru.alamat,
            "Alamat"
        );

        //INPUT LOKASI

        cout << "\nOPSI LOKASI TUJUAN:" << endl;
        cout << "1. Dalam Kota (10.000 / Kg)" << endl;
        cout << "2. Luar Kota (20.000 / Kg)" << endl;

        int opsiLokasi;

        while (true) {

            cout << "Pilih opsi lokasi tujuan (1-2): ";
            cin >> opsiLokasi;

            //ERROR HANDLING
            if (
                cin.fail() ||
                (opsiLokasi != 1 && opsiLokasi != 2)
            ) {

                cout << "Input tidak valid!\n";

                cin.clear();
                cin.ignore(1000, '\n');
            }

            else {
                break;
            }
        }

        //INPUT BERAT
        cout << "\nMasukkan berat (gram): ";
        cin >> paketBaru.berat;

        if (
            cin.fail() ||
            paketBaru.berat <= 0
        ) {

            cin.clear();
            cin.ignore(1000, '\n');

            throw "Berat harus angka positif!";
        }

        if (paketBaru.berat > 50000) {
            throw "Berat melebihi batas maksimal!";
        }

        //INPUT TIPE
        cout << "\nPilihan tipe barang:" << endl;
        cout << "1. Dokumen" << endl;
        cout << "2. Elektronik" << endl;
        cout << "3. Pecah Belah" << endl;
        cout << "4. Lainnya" << endl;

        int opsiTipe;

        while (true) {
            cout << "Pilih tipe barang (1-4): ";
            cin >> opsiTipe;

            if (
                cin.fail() ||
                opsiTipe < 1 ||
                opsiTipe > 4
            ) {

                cout << "Input tidak valid!\n";

                cin.clear();
                cin.ignore(1000, '\n');
            }

            else {
                break;
            }
        }

        cin.ignore();

        if (opsiTipe == 1) {
            paketBaru.tipe = "Dokumen";
        }

        else if (opsiTipe == 2) {
            paketBaru.tipe = "Elektronik";
        }

        else if (opsiTipe == 3) {
            paketBaru.tipe = "Pecah Belah";
        }

        else {
            paketBaru.tipe = "Lainnya";
        }

        //HITUNG ONGKIR
        paketBaru.ongkir = hitungOngkir(
            paketBaru.berat,
            opsiLokasi,
            opsiTipe
        );

        //PEMBAYARAN
        cout << "\n1. COD" << endl;
        cout << "2. Transfer" << endl;

        int metodeBayar;

        cin >> metodeBayar;

        paketBaru.pemilik = userAktif;

        //COD
        if (metodeBayar == 1) {
            paketBaru.pembayaran = "COD";
            paketBaru.status = "Diproses";

            //SEARCHING + LINEAR SEARCH
            do {

                paketBaru.resi = generateResi();

            } while (cekResiDiJson(paketBaru.resi));

            savePaketToJson(paketBaru);

            cout << "\nTransaksi berhasil!\n";
            cout << "Nomor Resi : " << paketBaru.resi << endl;

            tekanEnter();
        }

        //TRANSFER

        else if (metodeBayar == 2) {
            paketBaru.pembayaran = "Transfer";
            paketBaru.status = "Menunggu Validasi Admin";
            paketBaru.resi = "BELUM_RILIS";

            //PASS BY VALUE
            savePaketToJson(paketBaru);

            cout << "\nTransaksi berhasil dikirim!\n";

            tekanEnter();
        }
    }

    //EXCEPTION HANDLING
    catch (const char* msg) {
        cerr << "\nERROR: " << msg << endl;

        tekanEnter();
    }
}

//PROSEDUR SIMPAN JSON
//Materi: Prosedur + Pass by Value
void savePaketToJson(Paket p) {

    //FILE HANDLING
    ifstream inputFile("database/paket.json");

    json data = json::array();

    if (inputFile.is_open()) {
        if (
            inputFile.peek() !=
            ifstream::traits_type::eof()
        ) {

            inputFile >> data;
        }

        inputFile.close();
    }

    //STRUCT
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
    ofstream outputFile("database/paket.json");
    outputFile << data.dump(4);
    outputFile.close();
}