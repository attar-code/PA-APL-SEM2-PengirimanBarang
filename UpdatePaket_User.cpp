#include <iostream>
#include <fstream>
#include <string>
#include "data.h"
#include "database/json.hpp"

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

    string resiCari;

    cout << "Masukkan nomor resi : ";
    getline(cin, resiCari);

    bool ditemukan = false;

    for (auto &paket : data) {

        if (paket["resi"] == resiCari &&
            paket["pemilik"] == userAktif) {

            ditemukan = true;

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

    if (!ditemukan) {

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