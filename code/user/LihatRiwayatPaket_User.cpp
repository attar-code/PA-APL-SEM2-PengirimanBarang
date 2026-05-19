#include <iostream>
#include <fstream>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

//VARIABEL GLOBAL
extern string userAktif;

//DEKLARASI FUNGSI
void tekanEnter();

//Materi: Prosedur
void LihatRiwayatPaket_User() {
    system("cls");
    cout << "=== RIWAYAT PAKET ===\n";

    //VARIABEL LOKAL
    bool ditemukan = false;

    //FILE HANDLING
    ifstream inputFile("database/paket.json");

    json data;

    //ERROR HANDLING
    if (inputFile.peek() == ifstream::traits_type::eof()) {
        cout << "\nBelum ada riwayat paket.\n";
        inputFile.close();
        tekanEnter();
        return;
    }

    inputFile >> data;

    inputFile.close();

    //SEARCHING & LINEAR SEARCH
    for (auto paket : data) {

        if (paket["pemilik"] == userAktif) {
            ditemukan = true;
            cout << "\n============================\n";
            cout << "Nomor Resi     : "
                 << paket["resi"] << endl;

            cout << "Nama Pengirim  : "
                 << paket["namaPengirim"] << endl;

            cout << "Nama Penerima  : "
                 << paket["namaPenerima"] << endl;

            cout << "Alamat Tujuan  : "
                 << paket["alamat"] << endl;

            cout << "Berat Barang   : "
                 << paket["berat"] << " gram" << endl;

            cout << "Tipe Barang    : "
                 << paket["tipe"] << endl;

            cout << "Pembayaran     : "
                 << paket["pembayaran"] << endl;

            cout << "Total Ongkir   : Rp "
                 << paket["ongkir"] << endl;

            cout << "Status Paket   : "
                 << paket["status"] << endl;
        }
    }

    //ERROR HANDLING
    if (!ditemukan) {
        cout << "\nBelum ada riwayat paket.\n";
    }

    tekanEnter();
}