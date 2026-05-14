#include <iostream>
#include <fstream>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern string userAktif;

void tekanEnter();

void BatalkanPaket_User() {

    system("cls");

    cout << "=== BATALKAN PAKET ===\n";

    ifstream inputFile("Database/paket.json");

    json data;

    inputFile >> data;

    inputFile.close();

    bool ditemukan = false;

    cout << "\n=== DAFTAR PAKET ===\n";

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
            cout << "Status Paket   : " << paket["status"] << endl;
        }
    }

    if (!ditemukan) {

        cout << "\nBelum ada paket!\n";

        tekanEnter();
        return;
    }

    string resiCari;

    cout << "\nMasukkan nomor resi yang ingin dibatalkan : ";
    getline(cin, resiCari);

    bool paketDitemukan = false;

    for (auto &paket : data) {

        if (paket["resi"] == resiCari &&
            paket["pemilik"] == userAktif) {

            paketDitemukan = true;

            if (paket["status"] != "Menunggu Diproses") {

                cout << "\nPaket tidak bisa dibatalkan!\n";
                cout << "Status paket sudah diproses admin.\n";

                tekanEnter();
                return;
            }

            int pilihan;

            cout << "\nApakah yakin ingin membatalkan paket?\n";
            cout << "1. Ya\n";
            cout << "2. Tidak\n";
            cout << "Pilih : ";
            cin >> pilihan;
            cin.ignore();

            if (pilihan == 1) {

                paket["status"] = "Dibatalkan";

                ofstream outputFile("Database/paket.json");

                outputFile << data.dump(4);

                outputFile.close();

                cout << "\nPaket berhasil dibatalkan!\n";
            }

            else {

                cout << "\nPembatalan dibatalkan.\n";
            }

            break;
        }
    }

    if (!paketDitemukan) {

        cout << "\nPaket tidak ditemukan!\n";
    }

    tekanEnter();
}