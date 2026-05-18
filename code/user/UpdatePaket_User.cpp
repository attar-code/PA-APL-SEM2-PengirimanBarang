#include <iostream>
#include <fstream>
#include <string>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

// VARIABEL GLOBAL
extern string userAktif;

//DEKLARASI FUNGSI
void tekanEnter();

// PROSEDUR
void UpdatePaket_User() {

    system("cls");

    cout << "=== UPDATE PAKET ===\n";

    json data;

    //EXEPTION HANDLING
    try {

        //FILE HANDLING
        ifstream inputFile("Database/paket.json");

        if (!inputFile.is_open()) {

            throw runtime_error("File paket.json tidak ditemukan!");
        }

        if (inputFile.peek() == ifstream::traits_type::eof()) {

            throw runtime_error("Belum ada data paket!");
        }

        inputFile >> data;

        inputFile.close();
    }

    //EXCEPTION HANDLING
    catch (exception &e) {

        cout << "\nError : " << e.what() << endl;

        tekanEnter();
        return;
    }

    // VARIABEL LOKAL
    bool ditemukan = false;

    cout << "\n=== DAFTAR PAKET ===\n";

     //SEARCHING + LINEAR SEARCH
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

    // EERROR HANDLING
    if (!ditemukan) {

        cout << "\nBelum ada paket!\n";

        tekanEnter();
        return;
    }

    string resiCari;

    cout << "\nMasukkan nomor resi yang ingin diupdate : ";
    getline(cin, resiCari);

    bool paketDitemukan = false;

    //SEARCHING + LINEAR SEARCH
    //PASS BY REFERENCE
    for (auto &paket : data) {

        if (paket["resi"] == resiCari &&
            paket["pemilik"] == userAktif) {

            paketDitemukan = true;

            // PERCABANGAN
            if (paket["status"] == "Dikirim") {

                cout << "\nPaket tidak bisa diupdate!\n";
                cout << "Status paket sudah dikirim.\n";

                tekanEnter();
                return;
            }

            // VARIABEL LOKAL
            string namaPengirimBaru;
            string namaPenerimaBaru;
            string alamatBaru;
            string tipeBaru;
            int beratBaru;
            int opsiTipe;

            cout << "\nNama Pengirim Baru : ";
            getline(cin, namaPengirimBaru);

            cout << "Nama Penerima Baru : ";
            getline(cin, namaPenerimaBaru);

            cout << "Alamat Baru : ";
            getline(cin, alamatBaru);

            // BERAT 
            cout << "\nMasukkan berat baru (gram): ";
            cin >> beratBaru;

            // ERROR HANDLING
            while (cin.fail() || beratBaru <= 0) {

                cout << "Input berat tidak valid!\n";

                cin.clear();
                cin.ignore(1000, '\n');

                cout << "Masukkan berat baru (gram): ";
                cin >> beratBaru;
            }

            cout << "\nPilihan tipe barang:" << endl;
            cout << "1. Dokumen" << endl;
            cout << "2. Elektronik" << endl;
            cout << "3. Pecah Belah" << endl;
            cout << "4. Lainnya" << endl;

            //PERULANGAN
            while (true) {

                cout << "Pilih tipe barang (1-4): ";
                cin >> opsiTipe;

                //EROR HANDLING
                if (
                    cin.fail() ||
                    opsiTipe < 1 ||
                    opsiTipe > 4
                ) {

                    cout << "Input tidak valid!\n";

                    cin.clear();
                    cin.ignore(1000, '\n');
                }

                else {
                    break;
                }
            }

            cin.ignore();

            // PERCABANGAN
            if (opsiTipe == 1) {
                tipeBaru = "Dokumen";
            }

            else if (opsiTipe == 2) {
                tipeBaru = "Elektronik";
            }

            else if (opsiTipe == 3) {
                tipeBaru = "Pecah Belah";
            }

            else {
                tipeBaru = "Lainnya";
            }

            paket["namaPengirim"] = namaPengirimBaru;
            paket["namaPenerima"] = namaPenerimaBaru;
            paket["alamat"] = alamatBaru;
            paket["tipe"] = tipeBaru;
            paket["berat"] = beratBaru;

            break;
        }
    }

    // EROR HANDLING
    if (!paketDitemukan) {

        cout << "\nPaket tidak ditemukan!\n";

        tekanEnter();
        return;
    }

    //FILE HANDLING
    ofstream outputFile("Database/paket.json");

    outputFile << data.dump(4);

    outputFile.close();

    cout << "\nData paket berhasil diupdate!\n";

    tekanEnter();
}