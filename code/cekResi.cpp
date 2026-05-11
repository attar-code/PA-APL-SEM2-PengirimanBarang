#include <iostream>
#include <fstream>
#include <string>
#include "include/data.h"
#include "include/cekResi.h"
#include "Database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern string userAktif;

void tekanEnter();

void cekResi() {

    system("cls");

    cout << "=== CEK RESI ===\n";

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

        if (paket["resi"] == resiCari){
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
            cout << "Pemilik        : " << paket["pemilik"] << endl;

            cout << "\n=========== TRACKING ===========\n";

            if(paket["status"] == "Diproses"){

                cout << "[✓] Paket dikonfirmasi admin\n";
                cout << "[✓] Sedang diproses\n";
                cout << "[ ] Sedang dikirim\n";
                cout << "[ ] Paket selesai\n";
            }

            else if(paket["status"] == "Dikirim"){

                cout << "[✓] Paket dikonfirmasi admin\n";
                cout << "[✓] Sedang diproses\n";
                cout << "[✓] Sedang dikirim\n";
                cout << "[ ] Paket selesai\n";
            }

            else if(paket["status"] == "Selesai"){

                cout << "[✓] Paket dikonfirmasi admin\n";
                cout << "[✓] Sedang diproses\n";
                cout << "[✓] Sedang dikirim\n";
                cout << "[✓] Paket selesai\n";
            }

            else if(paket["status"] == "Dibatalkan"){

                cout << "[✓] Paket dikonfirmasi admin\n";
                cout << "[X] Paket dibatalkan\n";
            }

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

    tekanEnter();
}