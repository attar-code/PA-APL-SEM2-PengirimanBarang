#include <iostream>
#include <fstream>
#include <limits>
#include "../include/data.h"
#include "../include/admin.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern string userAktif;

void tekanEnter();

bool bisaDibatalkan(string status) {

    return (
        status == "Menunggu Validasi Admin" ||
        status == "Diproses" ||
        status == "Diproses (COD)" ||
        status == "Diproses (Lunas)"
    );
}

void BatalkanPaket_Admin() {

    system("cls");
    tampilRiwayatPaket();
    
    cout << "=== BATALKAN PAKET ===\n";
    cout << "Ketik 'batal' untuk kembali ke menu admin\n\n";

    ifstream inputFile("database/paket.json");

    if (!inputFile.is_open()) {
        cout << "Gagal membuka database!\n";
        tekanEnter();
        return;
    }

    json data;
    inputFile >> data;
    inputFile.close();

    string resiCari;

    cout << "Masukkan nomor resi : ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, resiCari);

    // 🔥 FITUR EXIT
    if (resiCari == "batal") {
        cout << "\nKembali ke menu admin...\n";
        return;
    }

    bool ditemukan = false;

    for (auto &paket : data) {

        if (paket["resi"].get<string>() == resiCari) {

            ditemukan = true;

            string status = paket["status"].get<string>();

            if (status == "Dikirim" || status == "Selesai") {

                cout << "\nPaket sudah dikirim/selesai, tidak bisa dibatalkan!\n";
                tekanEnter();
                return;
            }

            if (!bisaDibatalkan(status)) {

                cout << "\nStatus tidak valid untuk pembatalan!\n";
                tekanEnter();
                return;
            }

            int pilihan;

            cout << "\nStatus saat ini: " << status << endl;
            cout << "Yakin ingin membatalkan?\n";
            cout << "1. Ya\n";
            cout << "2. Tidak\n";
            cout << "Pilih : ";
            cin >> pilihan;
            cin.ignore();

            if (pilihan == 1) {

                paket["status"] = "Dibatalkan";

                ofstream outputFile("database/paket.json");
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