    #include <iostream>
    #include <fstream>
    #include "../include/data.h"
    #include "../database/json.hpp"
    #include "../include/user.h"
    #include <stdexcept>

    using json = nlohmann::json;
    using namespace std;

    //VARIABEL GLOBAL
    extern string userAktif;

    //ARRAY
    string menuHapus[2] = {
        "Ya",
        "Tidak"
    };

    //DEKLARASI FUNGSI
    void tekanEnter();

    //FUNGSI REKURSIF
    int hitungData(int n) {

        if (n == 0) {
            return 0;
        }

        return 1 + hitungData(n - 1);
    }

    //POINTER SEBAGAI PARAMETER
    void tampilJumlah(int *jumlah) {

        cout << "\nJumlah data : "
            << *jumlah << endl;
    }

    //POINTER PADA STRUCT
    void tampilResi(Paket *p) {

        cout << "Contoh akses pointer struct : "
            << p->resi << endl;
    }

    //PROSEDUR
    void HapusRiwayatPaket_User() {

        system("cls");

        cout << "=== HAPUS RIWAYAT PAKET ===\n";

        json data;

        //EXCEPTION HANDLING
        try {

            //FILE HANDLING
            ifstream inputFile("Database/paket.json");

            //ERROR HANDLING
            if (!inputFile.is_open()) {

                throw runtime_error(
                    "File paket.json tidak ditemukan!"
                );
            }

            //ERROR HANDLING
            if (
                inputFile.peek() ==
                ifstream::traits_type::eof()
            ) {

                throw runtime_error(
                    "Data paket kosong!"
                );
            }

            inputFile >> data;

            inputFile.close();
        }

        //EXCEPTION HANDLING
        catch (exception &e) {

            cout << "\nError : "
                << e.what() << endl;

            tekanEnter();
            return;
        }

        //VARIABEL LOKAL
        bool ditemukan = false;

        cout << "\n=== DAFTAR PAKET ===\n";

        //SEARCHING + LINEAR SEARCH
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
                    << paket["berat"]
                    << " gram" << endl;

                cout << "Tipe Barang    : "
                    << paket["tipe"] << endl;

                cout << "Status Paket   : "
                    << paket["status"] << endl;
            }
        }

        //ERROR HANDLING
        if (!ditemukan) {

            cout << "\nBelum ada paket!\n";

            tekanEnter();
            return;
        }

        //VARIABEL LOKAL
        string resiCari;

        cout << "\nMasukkan nomor resi yang ingin dihapus : ";
        getline(cin, resiCari);

        bool paketDitemukan = false;

        //SEARCHING + LINEAR SEARCH
        for (int i = 0; i < data.size(); i++) {

            if (
                data[i]["resi"] == resiCari &&
                data[i]["pemilik"] == userAktif
            ) {

                paketDitemukan = true;

                int pilihan;

                cout << "\nApakah yakin ingin menghapus paket?\n";

                cout << "1. "
                    << menuHapus[0] << endl;

                cout << "2. "
                    << menuHapus[1] << endl;

                cout << "Pilih : ";

                cin >> pilihan;

                //ERROR HANDLING
                while (
                    cin.fail() ||
                    pilihan < 1 ||
                    pilihan > 2
                ) {

                    cout << "Input tidak valid!\n";

                    cin.clear();
                    cin.ignore(1000, '\n');

                    cout << "Pilih : ";
                    cin >> pilihan;
                }

                cin.ignore();

                //PERCABANGAN
                if (pilihan == 1) {

                    data.erase(data.begin() + i);

                    //FILE HANDLING
                    ofstream outputFile(
                        "Database/paket.json"
                    );

                    outputFile
                        << data.dump(4);

                    outputFile.close();

                    cout << "\nRiwayat paket berhasil dihapus!\n";
                }

                else {

                    cout << "\nPenghapusan dibatalkan.\n";
                }

                break;
            }
        }

        //ERROR HANDLING
        if (!paketDitemukan) {

            cout << "\nPaket tidak ditemukan!\n";
        }

        //POINTER
        int jumlahData = data.size();

        tampilJumlah(&jumlahData);

        //FUNGSI REKURSIF
        cout << "Total perhitungan data : "
            << hitungData(jumlahData)
            << endl;

        tampilPesan();

        tekanEnter();
    }