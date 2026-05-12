#include <iostream>
#include <fstream>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern string userAktif;

void tekanEnter();

void LihatRiwayatPaket_User() {

    system("cls");

    cout << "=== RIWAYAT PAKET ===\n";

    bool ditemukan = false;

    ifstream inputFile("database/paket.json");

    json data;

    if (inputFile.peek() == ifstream::traits_type::eof()) {
        cout << "\nBelum ada riwayat paket.\n";

        inputFile.close();

        tekanEnter();
        return;
    }

    inputFile >> data;

    inputFile.close();

    for (auto paket : data) {

        if (paket["pemilik"] == userAktif) {

            ditemukan = true;

            cout << "\n============================\n";
            cout << "Nomor Resi     : " << paket["resi"] << endl;
            cout << "Nama Pengirim  : " << paket["namaPengirim"] << endl;
            cout << "Nama Penerima  : " << paket["namaPenerima"] << endl;
            cout << "Alamat Tujuan  : " << paket["alamat"] << endl;
            cout << "Berat Barang   : " << paket["berat"] << " gram" << endl;
            cout << "Tipe Barang    : " << paket["tipe"] << endl;
            cout << "Pembayaran     : " << paket["pembayaran"] << endl;
            cout << "Total Ongkir   : Rp " << paket["ongkir"] << endl;
            cout << "Status Paket   : " << paket["status"] << endl;
        }
    }

    if (!ditemukan) {
        cout << "\nBelum ada riwayat paket.\n";
    }

    tekanEnter();
}