#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>
#include <fstream>
#include "admin.h"
#include "data.h"
#include "database/json.hpp"

using json = nlohmann::json;
using namespace std;

// Fungsi untuk validasi input (hanya mengizinkan huruf, angka, spasi, titik, dan koma)
bool ValidasiInput(const string& input) {
    if (input.empty()) return false;
    for (char c : input) {
        if (!isalnum(c) && !isspace(c) && c != '.' && c != ',') {
            return false;
        }
    }
    return true;
}

// Fungsi pembantu untuk mempermudah lemparan error validasi teks
void cekValiditasInput(const string& input, const string& namaField) {
    if (input.empty()) {
        throw (namaField + " tidak boleh kosong!").c_str();
    }
    if (!ValidasiInput(input)) {
        throw (namaField + " mengandung karakter tidak valid!").c_str();
    }
}

// Fungsi untuk menyimpan satu objek paket ke database/paket.json
void savePaketToJson(const Paket& paket) {
    ifstream inputFile("database/paket.json");
    json data = json::array();
    
    if (inputFile.is_open()) {
        if (inputFile.peek() != ifstream::traits_type::eof()) {
            inputFile >> data;
        }
        inputFile.close();
    }

    json paketBaru = {
        {"resi", paket.resi},
        {"namaPengirim", paket.namaPengirim},
        {"namaPenerima", paket.namaPenerima},
        {"alamat", paket.alamat},
        {"berat", paket.berat},
        {"tipe", paket.tipe},
        {"status", paket.status},
        {"pemilik", paket.pemilik},
        {"ongkir", paket.ongkir}
    };

    data.push_back(paketBaru);

    ofstream outputFile("database/paket.json");
    if (outputFile.is_open()) {
        outputFile << data.dump(4);
        outputFile.close();
    }
}

// Fungsi untuk menghasilkan nomor resi acak sepanjang 8 karakter
string generateResi() {
    string kumpulanKarakter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string resi = "";
    for (int i = 0; i < 8; i++) {
        resi += kumpulanKarakter[rand() % kumpulanKarakter.length()];
    }
    return resi;
}

// Fungsi untuk mengecek duplikasi nomor resi di database JSON (Tunggal & Efisien)
bool cekResiDiJson(const string& Cari) {
    ifstream file("database/paket.json");
    json data;
    if (file.is_open() && file.peek() != ifstream::traits_type::eof()) {
        file >> data;
        file.close();
        for (const auto& item : data) {
            if (item["resi"] == Cari) {
                return true;
            }
        }
    }
    return false;
}

// Fungsi untuk menghitung ongkir berdasarkan berat, lokasi, dan tipe barang
long long hitungOngkir(int beratGram, int opsiLokasi, int opsiTipe) {
    long long hargaPerKg = (opsiLokasi == 1) ? 10000 : 20000;
    long long tambahanTipe = 0;

    switch (opsiTipe) {
        case 1: tambahanTipe = 0; break;      // Dokumen
        case 2: tambahanTipe = 20000; break;  // Elektronik
        case 3: tambahanTipe = 15000; break;  // Pecah Belah
        case 4: tambahanTipe = 10000; break;  // Lainnya
        default: tambahanTipe = 0; break;
    }

    return (long long)((beratGram / 1000.0) * hargaPerKg) + tambahanTipe;
}

// FUNGSI UTAMA: Bersih, Update Status Otomatis & Pemilik Admin
void TambahPaketAdmin() {
    try {
        cout << "=== TAMBAH PAKET (ADMIN - TOKO OFFLINE) ===" << endl;
        
        Paket paketBaru;
        cin.ignore(); // Membersihkan sisa buffer input sebelum getline
        
        // 1. Input Data Pelanggan
        cout << "Masukkan nama pengirim: ";
        getline(cin, paketBaru.namaPengirim);
        cekValiditasInput(paketBaru.namaPengirim, "Nama pengirim");
        
        cout << "Masukkan nama penerima: ";
        getline(cin, paketBaru.namaPenerima);
        cekValiditasInput(paketBaru.namaPenerima, "Nama penerima");
        
        cout << "Masukkan alamat: ";
        getline(cin, paketBaru.alamat);
        cekValiditasInput(paketBaru.alamat, "Alamat");

        // 2. Input Lokasi Tujuan
        cout << "\nOPSI LOKASI TUJUAN:" << endl;
        cout << "1. Dalam Kota (10.000 / Kg)" << endl;
        cout << "2. Luar Kota (20.000 / Kg)" << endl;
        int opsiLokasi;
        while (true) {
            cout << "Pilih opsi lokasi tujuan (1-2): ";
            cin >> opsiLokasi;
            if (cin.fail() || (opsiLokasi != 1 && opsiLokasi != 2)) {
                cout << "Input tidak valid! Masukkan angka 1 atau 2." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            } else {
                break;
            }
        }

        // 3. Input Berat (Dengan Validasi Maksimal 50kg)
        cout << "Masukkan berat (gram) [Maksimal 50.000 gram]: ";
        cin >> paketBaru.berat;
        if (cin.fail() || paketBaru.berat <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            throw "Berat harus berupa angka positif!";
        }
        if (paketBaru.berat > 50000) {
            throw "Berat paket melebihi batas maksimal (Maksimal 50.000 gram / 50 kg)!";
        }
        
        // 4. Input Tipe Barang
        cout << "\nPilihan tipe barang:" << endl;
        cout << "1. Dokumen (Bebas Biaya Tambahan)" << endl;
        cout << "2. Elektronik (+Rp 20.000)" << endl;
        cout << "3. Pecah Belah (+Rp 15.000)" << endl;
        cout << "4. Lainnya (+Rp 10.000)" << endl;
        int opsiTipe;
        while (true) {
            cout << "Pilih opsi tipe barang (1-4): ";
            cin >> opsiTipe;
            if (cin.fail() || opsiTipe < 1 || opsiTipe > 4) {
                cout << "Input tidak valid! Masukkan angka antara 1 dan 4." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            } else {
                break;
            }
        }

        if (opsiTipe == 1)      paketBaru.tipe = "Dokumen";
        else if (opsiTipe == 2) paketBaru.tipe = "Elektronik";
        else if (opsiTipe == 3) paketBaru.tipe = "Pecah Belah";
        else                    paketBaru.tipe = "Lainnya";

        // Hitung total ongkir
        paketBaru.ongkir = hitungOngkir(paketBaru.berat, opsiLokasi, opsiTipe);

        // Menampilkan Ringkasan Biaya sebelum bayar (Sesuai update barumu)
        cout << "\n======================================" << endl;
        cout << "          RINGKASAN TRANSAKSI         " << endl;
        cout << "======================================" << endl;
        cout << "Pengirim     : " << paketBaru.namaPengirim << endl;
        cout << "Penerima     : " << paketBaru.namaPenerima << endl;
        cout << "Tipe Barang  : " << paketBaru.tipe << endl;
        cout << "Berat Barang : " << paketBaru.berat << " gram" << endl;
        cout << "Total Ongkir : Rp " << paketBaru.ongkir << endl;
        cout << "Status Paket : Diproses" << endl;
        cout << "Pemilik      : Admin (Toko Offline)" << endl;
        cout << "======================================" << endl;

        // 5. Pilihan Metode Pembayaran
        cout << "METODE PEMBAYARAN:" << endl;
        cout << "1. COD (Bayar di Tujuan)" << endl;
        cout << "2. Bayar Sekarang (Cash / Transfer di Toko)" << endl;
        int metodeBayar;
        while (true) {
            cout << "Pilih metode pembayaran (1-2): ";
            cin >> metodeBayar;
            if (cin.fail() || (metodeBayar != 1 && metodeBayar != 2)) {
                cout << "Input tidak valid! Masukkan angka 1 atau 2." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            } else {
                break;
            }
        }

        bool transaksiSukses = false;

        if (metodeBayar == 1) {
            paketBaru.status = "Diproses (COD)";
            transaksiSukses = true;
        } 
        else if (metodeBayar == 2) {
            cout << "\n[KONFIRMASI PEMBAYARAN]" << endl;
            cout << "1. Sudah Bayar (Lunas)" << endl;
            cout << "2. Cancel (Batalkan Pesanan)" << endl;
            int konfirmasi;
            while (true) {
                cout << "Pilih status (1-2): ";
                cin >> konfirmasi;
                if (cin.fail() || (konfirmasi != 1 && konfirmasi != 2)) {
                    cout << "Input tidak valid! Masukkan angka 1 atau 2." << endl;
                    cin.clear();
                    cin.ignore(1000, '\n');
                } else {
                    break;
                }
            }

            if (konfirmasi == 1) {
                paketBaru.status = "Diproses (Lunas)";
                transaksiSukses = true;
            } else {
                cout << "\n[!] Transaksi dibatalkan oleh Admin." << endl;
                tekanEnter();
                return; 
            }
        }

        // 6. Jika transaksi sukses, buat resi dan simpan ke JSON
        if (transaksiSukses) {
            static bool seeded = false;
            if (!seeded) {
                srand(time(0));
                seeded = true;
            }
            
            string resiBaru;
            do {
                resiBaru = generateResi();
            } while (cekResiDiJson(resiBaru));

            paketBaru.resi = resiBaru;
            paketBaru.pemilik = "Admin"; // Pemilik otomatis diset "Admin" di database

            // Simpan data langsung ke Database paket.json
            savePaketToJson(paketBaru);

            // Tampilkan Resi Sukses
            cout << "\n======================================" << endl;
            cout << "   TRANSAKSI SUKSES & DATA DISIMPAN!  " << endl;
            cout << "======================================" << endl;
            cout << "NOMOR RESI ANDA : " << paketBaru.resi << endl;
            cout << "Status Paket    : " << paketBaru.status << endl;
            cout << "Pemilik         : " << paketBaru.pemilik << endl;
            cout << "======================================" << endl;
            cout << "Silakan tempel resi ini pada paket." << endl;
            
            tekanEnter();
        }
    }
    catch (const char* msg) {
        cerr << "\nError: " << msg << endl;
        tekanEnter();
    }
}