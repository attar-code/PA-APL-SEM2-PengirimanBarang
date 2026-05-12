#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

// Mengambil variabel global dari sesi login user
extern string userAktif;

// Deklarasi fungsi eksternal/pembantu agar sinkron saat kompilasi
string generateResi();
int inputAngka(string pesan);
bool cekResiDiJson(const string& Cari);
long long hitungOngkir(int beratGram, int opsiLokasi, int opsiTipe);
void tekanEnter();
void savePaketToJson(Paket p);

// Fungsi untuk validasi input (hanya mengizinkan huruf, angka, spasi, titik, dan koma)
bool ValidasiInputUser(const string& input) {
    if (input.empty()) return false;
    for (char c : input) {
        if (!isalnum(c) && !isspace(c) && c != '.' && c != ',') {
            return false;
        }
    }
    return true;
}

// Fungsi pembantu untuk mempermudah lemparan error validasi teks
void cekValiditasInputUser(const string& input, const string& namaField) {
    if (input.empty()) {
        throw (namaField + " tidak boleh kosong!").c_str();
    }
    if (!ValidasiInputUser(input)) {
        throw (namaField + " mengandung karakter tidak valid!").c_str();
    }
}

// FUNGSI UTAMA: Tambah Paket sisi Customer/User
void TambahPaket_User() {
    try {
        static bool seeded = false;
        if (!seeded) {
            srand(time(0));
            seeded = true;
        }

        // Deteksi Sistem Operasi untuk membersihkan layar terminal
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        cout << "=== TAMBAH PAKET (CUSTOMER) ===\n" << endl;

        Paket paketBaru;

        // 1. Input & Validasi Data Pelanggan
        cout << "Nama Pengirim : ";
        getline(cin, paketBaru.namaPengirim);
        cekValiditasInputUser(paketBaru.namaPengirim, "Nama pengirim");

        cout << "Nama Penerima : ";
        getline(cin, paketBaru.namaPenerima);
        cekValiditasInputUser(paketBaru.namaPenerima, "Nama penerima");

        cout << "Alamat Tujuan : ";
        getline(cin, paketBaru.alamat);
        cekValiditasInputUser(paketBaru.alamat, "Alamat");

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

        // 3. Input Berat (Maksimal 50kg)
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

        // Hitung total ongkos kirim
        paketBaru.ongkir = hitungOngkir(paketBaru.berat, opsiLokasi, opsiTipe);

        // Menampilkan Ringkasan Biaya sebelum bayar
        cout << "\n======================================" << endl;
        cout << "          RINGKASAN TRANSAKSI         " << endl;
        cout << "======================================" << endl;
        cout << "Pengirim     : " << paketBaru.namaPengirim << endl;
        cout << "Penerima     : " << paketBaru.namaPenerima << endl;
        cout << "Tipe Barang  : " << paketBaru.tipe << endl;
        cout << "Berat Barang : " << paketBaru.berat << " gram" << endl;
        cout << "Total Ongkir : Rp " << paketBaru.ongkir << endl;
        cout << "======================================" << endl;

        // 5. Opsi Metode Pembayaran
        cout << "METODE PEMBAYARAN:" << endl;
        cout << "1. COD (Bayar di Tujuan)" << endl;
        cout << "2. Transfer (Virtual Account)" << endl;
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

        // Mengeset otomatis pemilik paket sesuai akun yang sedang login saat ini
        paketBaru.pemilik = userAktif;

        // --- ALUR 1: JIKA MEMILIH COD ---
        if (metodeBayar == 1) {
            paketBaru.pembayaran = "COD";
            paketBaru.status = "Diproses";

            // Generate resi unik untuk COD
            do {
                paketBaru.resi = generateResi();
            } while (cekResiDiJson(paketBaru.resi));

            // Simpan langsung ke database JSON
            savePaketToJson(paketBaru);

            // Tampilkan rincian transaksi & nomor resi ke user
            cout << "\n======================================" << endl;
            cout << "     TRANSAKSI COD BERHASIL dibuat!   " << endl;
            cout << "======================================" << endl;
            cout << "NOMOR RESI ANDA : " << paketBaru.resi << endl;
            cout << "Status Paket    : " << paketBaru.status << endl;
            cout << "======================================" << endl;
            cout << "Silakan serahkan paket ke kurir/toko terdekat." << endl;
            
            tekanEnter();
        } 
        
        // --- ALUR 2: JIKA MEMILIH TRANSFER ---
        else if (metodeBayar == 2) {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif

            cout << "=== PEMBAYARAN VIA TRANSFER ===\n" << endl;
            cout << "Total Tagihan       : Rp " << paketBaru.ongkir << endl;
            cout << "No. Virtual Account : 082341154761 (DANA/Bank)" << endl;
            cout << "--------------------------------------" << endl;
            cout << "1. Sudah Bayar (Kirim bukti ke Admin)" << endl;
            cout << "2. Batalkan Transaksi" << endl;
            
            int konfirmasi;
            while (true) {
                cout << "Pilih status pembayaran (1-2): ";
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
                paketBaru.pembayaran = "Transfer";
                paketBaru.status = "Menunggu Validasi Admin";
                
                // Resi diset "BELUM_RILIS" sampai admin mengonfirmasi pembayaran
                paketBaru.resi = "BELUM_RILIS"; 

                // Simpan data paket ke database agar bisa divalidasi oleh admin
                savePaketToJson(paketBaru);

                cout << "\n======================================" << endl;
                cout << "      SUBMIT TRANSAKSI BERHASIL!      " << endl;
                cout << "======================================" << endl;
                cout << "Status Paket : Menunggu Validasi Admin" << endl;
                cout << "======================================" << endl;
                cout << "(*) Nomor resi akan terbit setelah dikonfirmasi oleh Admin." << endl;
                cout << "Silakan cek berkala di menu Lihat Riwayat Paket Anda." << endl;

                tekanEnter();
            } 
            else {
                cout << "\n[!] Transaksi dibatalkan. Data tidak disimpan." << endl;
                tekanEnter();
                return;
            }
        }
    }
    catch (const char* msg) {
        cerr << "\nError: " << msg << endl;
        tekanEnter();
    }
}

// Fungsi Simpan ke database/paket.json (Menggunakan folder lowercase 'database')
void savePaketToJson(Paket p) {
    ifstream inputFile("database/paket.json");
    json data = json::array();

    if (inputFile.is_open()) {
        if (inputFile.peek() != ifstream::traits_type::eof()) {
            inputFile >> data;
        }
        inputFile.close();
    }

    json paketBaru = {
        {"resi", p.resi},
        {"namaPengirim", p.namaPengirim},
        {"namaPenerima", p.namaPenerima},
        {"alamat", p.alamat},
        {"berat", p.berat},
        {"tipe", p.tipe},
        {"status", p.status},
        {"pemilik", p.pemilik},
        {"ongkir", p.ongkir},
        {"pembayaran", p.pembayaran}
    };

    data.push_back(paketBaru);

    ofstream outputFile("database/paket.json");
    if (outputFile.is_open()) {
        outputFile << data.dump(4);
        outputFile.close();
    }
}