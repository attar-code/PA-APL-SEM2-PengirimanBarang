#include <iostream>
#include <fstream>
#include "data.h"
#include "Database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern string userAktif;

void tekanEnter();

void HapusRiwayatPaket_User() {

    system("cls");

    cout << "=== HAPUS RIWAYAT PAKET ===\n";

    ifstream inputFile("Database/paket.json");

    json data;

    inputFile >> data;

    inputFile.close();

    string resiCari;

    cout << "Masukkan nomor resi : ";
    getline(cin, resiCari);

    bool ditemukan = false;

    for (int i = 0; i < data.size(); i++) {

        if (data[i]["resi"] == resiCari &&
            data[i]["pemilik"] == userAktif) {

            ditemukan = true;

            int pilihan;

            cout << "\n1. Ya\n";
            cout << "2. Tidak\n";
            cout << "Pilih : ";
            cin >> pilihan;
            cin.ignore();

            if (pilihan == 1) {

                data.erase(data.begin() + i);

                ofstream outputFile("Database/paket.json");

                outputFile << data.dump(4);

                outputFile.close();

                cout << "\nRiwayat paket berhasil dihapus!\n";
            }

            else {

                cout << "\nPenghapusan dibatalkan.\n";
            }

            break;
        }
    }

    if (!ditemukan) {

        cout << "\nPaket tidak ditemukan!\n";
    }

    tekanEnter();
}