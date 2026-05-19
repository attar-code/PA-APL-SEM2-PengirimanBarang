#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "../include/data.h"
#include "../include/cekResi.h"
#include "../database/json.hpp"

#define RESET "\033[0m"
#define MERAH "\033[31m"
#define HIJAU "\033[32m"
#define KUNING "\033[33m"
#define BIRU "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

using json = nlohmann::json;
using namespace std;

extern string userAktif;

void tekanEnter();

void cekResi() {
    bersihkanLayar();

    cout << CYAN << BOLD << setfill('=') << setw(110) << "" << RESET << setfill(' ') << endl;
    cout << BOLD << "                                       CEK RESI PAKET" << RESET << endl;
    cout << CYAN << BOLD << setfill('=') << setw(110) << "" << RESET << setfill(' ') << endl;

    ifstream inputFile("database/paket.json");

    if (!inputFile.is_open()) {
        cout << MERAH << endl << "File paket.json tidak ditemukan!" << endl << RESET;
        tekanEnter();
        return;
    }

    json data;

    if (inputFile.peek() == ifstream::traits_type::eof()) {
        cout << KUNING << "\nBelum ada data paket!\n" << RESET;
        inputFile.close();

        tekanEnter();
        return;
    }

    inputFile >> data;
    inputFile.close();

    string resiCari;

    cout << endl << "Masukkan Nomor Resi : ";
    getline(cin, resiCari);

    bool ditemukan = false;

    for (auto &paket : data) {
        bersihkanLayar();
        
        if (paket["resi"] == resiCari) {

            ditemukan = true;

            string status = paket["status"];
            string warnaStatus = RESET;

            if(status == "Diproses")
                warnaStatus = KUNING;

            else if(status == "Dikirim")
                warnaStatus = BIRU;

            else if(status == "Selesai")
                warnaStatus = HIJAU;

            else if(status == "Dibatalkan")
                warnaStatus = MERAH;

            cout << endl;
            cout << CYAN << BOLD << setfill('=') << setw(110) << "" << RESET << setfill(' ') << endl;
            cout << CYAN << setfill('-') << setw(110) << "" << RESET << setfill(' ') << endl;
            cout << left << setw(20) << "Nomor Resi" << ": " << paket["resi"] << endl;
            cout << setw(20) << "Nama Pengirim" << ": " << paket["namaPengirim"] << endl;
            cout << setw(20) << "Nama Penerima" << ": " << paket["namaPenerima"] << endl;
            cout << setw(20) << "Alamat Tujuan" << ": " << paket["alamat"] << endl;
            cout << setw(20) << "Berat Barang" << ": " << to_string((int)paket["berat"]) << " gram\n";
            cout << setw(20) << "Tipe Barang" << ": " << paket["tipe"] << endl;
            cout << setw(20) << "Pembayaran" << ": " << paket["pembayaran"] << endl;
            cout << setw(20) << "Total Ongkir" << ": Rp " << paket["ongkir"] << endl;
            cout << setw(20) << "Pemilik" << ": " << paket["pemilik"] << endl;
            cout << setw(20) << "Status Paket" << ": " << warnaStatus << BOLD << paket["status"] << RESET << endl;
            cout << CYAN << setfill('-') << setw(110) << "" << RESET << setfill(' ') << endl;
            cout << CYAN << BOLD << setfill('=') << setw(110) << "" << RESET << setfill(' ') << endl;
            break;
        }
    }

    if (!ditemukan) {

        cout << "\n"
             << MERAH
             << BOLD
             << "Paket tidak ditemukan!"
             << RESET
             << endl;

        tekanEnter();
        return;
    }
    ofstream outputFile("database/paket.json");

    outputFile << data.dump(4);

    outputFile.close();

    tekanEnter();
}