#include <iostream>
#include <fstream>
#include <string>
#include "data.h"
#include "Database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern string userAktif;

void tekanEnter();

void UpdatePaket_User() {

    system("cls");

    cout << "=== UPDATE PAKET ===\n";

    ifstream inputFile("Database/paket.json");

    if (!inputFile.is_open()) {

        cout << "\nFile paket.json tidak ditemukan!\n";

        tekanEnter();
        return;
    }

    json data;

    if (inputFile.peek() == ifstream::traits_type::eof()) {

        cout << "\nBelum ada data paket!\n";

        inputFile.close();

        tekanEnter();
        return;
    }

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

    cout << "\nMasukkan nomor resi yang ingin diupdate : ";
    getline(cin, resiCari);

    bool paketDitemukan = false;

    for (auto &paket : data) {

        if (paket["resi"] == resiCari &&
            paket["pemilik"] == userAktif) {

            paketDitemukan = true;

            if (paket["status"] != "Menunggu Diproses") {

                cout << "\nPaket tidak bisa diupdate!\n";
                cout << "Status paket sudah diproses admin.\n";

                tekanEnter();
                return;
            }

            string namaPengirimBaru;
            string namaPenerimaBaru;
            string alamatBaru;
            string tipeBaru;
            int beratBaru;

            cout << "\nNama Pengirim Baru : ";
            getline(cin, namaPengirimBaru);

            cout << "Nama Penerima Baru : ";
            getline(cin, namaPenerimaBaru);

            cout << "Alamat Baru : ";
            getline(cin, alamatBaru);

            cout << "Tipe Barang Baru : ";
            getline(cin, tipeBaru);

            cout << "Berat Baru : ";
            cin >> beratBaru;
            cin.ignore();

            paket["namaPengirim"] = namaPengirimBaru;
            paket["namaPenerima"] = namaPenerimaBaru;
            paket["alamat"] = alamatBaru;
            paket["tipe"] = tipeBaru;
            paket["berat"] = beratBaru;

            break;
        }
    }

    if (!paketDitemukan) {

        cout << "\nPaket tidak ditemukan!\n";

        tekanEnter();
        return;
    }

    ofstream outputFile("Database/paket.json");

    outputFile << data.dump(4);

    outputFile.close();

    cout << "\nData paket berhasil diupdate!\n";

    tekanEnter();
}