#include <iostream>
#include <fstream>
#include "data.h"
#include "Database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern string userAktif;

void tekanEnter();

void BatalkanPaket_Admin() {

    system("cls");

    cout << "=== BATALKAN PAKET ===\n";

    ifstream inputFile("Database/paket.json");

    json data;

    inputFile >> data;

    inputFile.close();

    string resiCari;

    cout << "Masukkan nomor resi : ";
    getline(cin, resiCari);

    bool ditemukan = false;

    for (auto &paket : data) {

        if (paket["resi"] == resiCari) {

            ditemukan = true;

            if (paket["status"] != "Menunggu Diproses") {

                cout << "\nPaket tidak bisa dibatalkan!\n";

                tekanEnter();
                return;
            }

            int pilihan;

            cout << "\n1. Ya\n";
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

    if (!ditemukan) {

        cout << "\nPaket tidak ditemukan!\n";
    }

    tekanEnter();
}