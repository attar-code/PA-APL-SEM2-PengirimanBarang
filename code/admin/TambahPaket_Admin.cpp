#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include "../include/admin.h"
#include "../include/data.h"
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

bool ValidasiNama(const string& input) {
    if (input.empty()) {
        cout << MERAH << BOLD << "INPUT TIDAK BOLEH KOSONG!" << RESET << endl;
        return false;
    }
    if (isspace(input[0]) || isspace(input.back())) {
    cout << MERAH << BOLD << "NAMA TIDAK BOLEH DI AWALI ATAU DI AKHIRI OLEH SPASI" << RESET << endl;
        return false;
    }
    if (!isalpha(input[0]) || !isalpha(input.back())) {
        cout << MERAH << BOLD << "NAMA HARUS DI AWALI ATAU DI AKHIRI OLEH HURUF" << RESET << endl;
        return false;
    }
    bool SpasiSebelum = false;
    for (char c : input) {
        if (!isalpha(c) && !isspace(c)) {
            cout << MERAH << BOLD << "NAMA HANYA BOLEH MENGANDUNG HURUF DAN SPASI" << RESET << endl;
            return false;
        }
        if (isspace(c)) {
            if (SpasiSebelum) {
                cout << MERAH << BOLD << "TIDAK BOLEH SPASI GANDA DI TENGAH NAMA" << RESET << endl;
                return false;
            }
            SpasiSebelum = true;
        } else {
            SpasiSebelum = false;
        }
    }
    return true;
}

bool ValidasiAlamat(const string& input) {
    if (input.empty()) {
        cout << MERAH << BOLD << "INPUT TIDAK BOLEH KOSONG!" << RESET << endl;
        return false;
    }
    if (isspace(input[0]) || isspace(input.back())) {
        cout << MERAH << BOLD << "ALAMAT TIDAK BOLEH DI AWALI ATAU DI AKHIRI OLEH SPASI" << RESET << endl;
        return false;
    }
    if (!isalnum(input[0]) || !isalnum(input.back())) {
        cout << MERAH << BOLD << "ALAMAT HARUS DI AWALI ATAU DI AKHIRI OLEH HURUF ATAU ANGKA" << RESET << endl;
        return false;
    }
        bool SpasiSebelum = false;
        bool TandaBacaSebelum = false;
    for (char c : input) {
        if (!isalnum(c) && !isspace(c) && c != ',' && c != '.' ) {
            cout << MERAH << BOLD << "ALAMAT HANYA BOLEH MENGANDUNG HURUF, ANGKA, SPASI, KOMA, DAN TITIK" << RESET << endl;
            return false;
        }
        if (isspace(c)) {
            if (SpasiSebelum) {
                cout << MERAH << BOLD << "TIDAK BOLEH SPASI GANDA DI TENGAH ALAMAT" << RESET << endl;
                return false;
            }
            SpasiSebelum = true;
            TandaBacaSebelum = false;
        } else if (c == ',' || c == '.') {
            if (TandaBacaSebelum) {
                cout << MERAH << BOLD << "TIDAK BOLEH TANDA BACA GANDA DI TENGAH ALAMAT" << RESET << endl;
                return false;
            }
            TandaBacaSebelum = true;
            SpasiSebelum = false;
        } else {
            SpasiSebelum = false;
            TandaBacaSebelum = false;
        }
    }
    return true;
}

bool ValidAngka(const string& input) {
    if (input.empty()) {
        cout << MERAH << BOLD << "INPUT TIDAK BOLEH KOSONG!" << RESET << endl;
        return false;
    }
    if (isspace(input[0]) || isspace(input.back())) {
        cout << MERAH << BOLD << "INPUT TIDAK BOLEH DI AWALI ATAU DI AKHIRI OLEH SPASI" << RESET << endl;
        return false;
    }
    for (char c : input) {
        if (c < '0' || c > '9') {
            cout << MERAH << BOLD << "INPUT HANYA BOLEH MENGANDUNG ANGKA POSITIF!" << RESET << endl;
            return false;
        }
    }
    return true;
}

extern int _getch();
int MenuScroll(string judul, vector<string> pilihan) { // 'string' huruf kecil
    int posisi = 0;
    int key;
    
    while (true) {
bersihkanLayar();
        cout << KUNING << BOLD << "==============================" << RESET << endl;
        cout << BOLD << "   " << judul << RESET << endl;
        cout << KUNING << BOLD << "==============================" << RESET << endl;
        
        for (size_t i = 0; i < pilihan.size(); i++) {
            if (i == posisi) {
                cout << BIRU << BOLD << "  > [ " << pilihan[i] << " ] <" << RESET << endl;
            } else {
                cout << "      " << pilihan[i] << endl;
            }
        }
        cout << KUNING << "------------------------------" << RESET << endl;
        cout << "Gunakan Panah & Enter" << endl;

        key = _getch();

        // 1. Logika Deteksi Input Lintas Platform
        if (key == 27) { // Sinyal Mac/Linux
            _getch(); 
            key = _getch();
            if (key == 65) key = 72;      // Panah Atas
            else if (key == 66) key = 80; // Panah Bawah
        }
        else if (key == 224) { // Sinyal Windows
            key = _getch(); 
        }

        // 2. Eksekusi Pergerakan (Sudah Seragam)
        if (key == 72) { // Atas
            if (posisi > 0) posisi--;
            else posisi = pilihan.size() - 1;
        } 
        else if (key == 80) { // Bawah
            if (posisi < pilihan.size() - 1) posisi++;
            else posisi = 0;
        } 
        else if (key == 13 || key == 10) { // Enter
            return posisi + 1; // Mengembalikan 1, 2, dst.
        }
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
        {"lokasi", paket.lokasi},
        {"berat", paket.berat},
        {"tipe", paket.tipe},
        {"status", paket.status},
        {"pemilik", paket.pemilik},
        {"ongkir", paket.ongkir},
        {"pembayaran", paket.pembayaran}
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

bool cekResiDiJson(const string& Cari) {
    ifstream file("database/paket.json");
    // Cek apakah file gagal dibuka atau kosong
    if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) {
        return false; 
    }
    json data;
    file >> data;
    file.close(); 
    for (const auto& item : data) {
        if (item.contains("resi") && item["resi"] == Cari) {
            return true;
        }
    }
    return false;
}

int HitungOngkir(int beratGram, int opsiLokasi = 1, int opsiTipe = 2) {
    int ongkirlokasi = (opsiLokasi == 1) ? 10000 : 20000;
    switch (opsiTipe) {
        case 1:
            return (beratGram * ongkirlokasi / 1000) + 20000; // Elektronik
        case 2:
            return (beratGram * ongkirlokasi / 1000) + 15000; // Pecah Belah
        case 3:
            return (beratGram * ongkirlokasi / 1000) + 10000; // Lainnya
        default:
            return (beratGram * ongkirlokasi / 1000);         // Dokumen
    }
}

// =========================================================================
// FUNGSI 2: Versi String (OVERLOADED - Dipakai saat baca database JSON)
// =========================================================================
int HitungOngkir(int beratGram, string lokasi, string tipe) {
    // 1. Tentukan ongkir berdasarkan teks lokasi
    int ongkirlokasi = (lokasi == "Dalam Kota") ? 10000 : 20000;
    
    // 2. Hitung total biaya berdasarkan teks tipe barang
    if (tipe == "Elektronik") {
        return (beratGram * ongkirlokasi / 1000) + 20000;
    } 
    else if (tipe == "Pecah Belah") {
        return (beratGram * ongkirlokasi / 1000) + 15000;
    } 
    else if (tipe == "Lainnya") {
        return (beratGram * ongkirlokasi / 1000) + 10000;
    } 
    else {
        return (beratGram * ongkirlokasi / 1000); // Dokumen
    }
}

void HeaderTambahPaket() {
    cout << HIJAU << BOLD << "======================" << RESET << endl;
    cout << HIJAU << BOLD << "   TAMBAH PAKET BARU  " << RESET << endl;
    cout << HIJAU << BOLD << "======================" << RESET << endl;
} 

// FUNGSI UTAMA: Bersih, Update Status Otomatis & Pemilik Admin
void TambahPaketAdmin() {
    srand(time(0)); 
    Paket paketBaru;
    string input;
    string beratGram;
    int opsiLokasi;
    int opsiTipe;
    string PilihanTipe;
    string resiBaru;
    string pilihanPembayaran;
    // Input Nama Pengirim

    do {
        cin.ignore(1000, '\n'); // Membersihkan buffer input sebelum getline
        bersihkanLayar();   
        HeaderTambahPaket();
        cout << HIJAU << BOLD << "Nama Pengirim: " << RESET;
        getline(cin, input);
        if (ValidasiNama(input)) {
            paketBaru.namaPengirim = input;
            break; 
        }
        tekanEnter();
    } while (true);
    // Input Nama Penerima
    do {
        bersihkanLayar();
        HeaderTambahPaket();
        cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl; 
        cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;
        cout << HIJAU << BOLD << "Nama Penerima: " << RESET;
        getline(cin, input);
        if (ValidasiNama(input)) {
            paketBaru.namaPenerima = input;
            break;
        }
        tekanEnter();
    } while (true);
    // Input Alamat
    do {
        bersihkanLayar();
        HeaderTambahPaket();
        cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl;
        cout << BIRU << BOLD << "Nama Penerima : " << RESET << paketBaru.namaPenerima << endl;
        cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;
        cout << HIJAU << BOLD << "Alamat: " << RESET;
        getline(cin, input);
        if (ValidasiAlamat(input)) {
            paketBaru.alamat = input;
            break;
        }
        tekanEnter();
    } while (true);
    // Input Lokasi
    bersihkanLayar();
    HeaderTambahPaket();
    cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl;
    cout << BIRU << BOLD << "Nama Penerima : " << RESET << paketBaru.namaPenerima << endl;
    cout << BIRU << BOLD << "Alamat        : " << RESET << paketBaru.alamat << endl;
    cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;
    cout << HIJAU << BOLD << "Pilih Lokasi Pengiriman:" << RESET << endl;
    vector<string> listLokasi = {
        "Dalam Kota (+Rp10.000)",
        "Luar Kota (+Rp20.000)"
    };
    opsiLokasi = MenuScroll("PILIH LOKASI PENGIRIMAN", listLokasi);
    if (opsiLokasi == 1) {
        paketBaru.lokasi = "Dalam Kota";
    } else {
        paketBaru.lokasi = "Luar Kota";
    }
    // Input Berat
    do {
        bersihkanLayar();
        HeaderTambahPaket();
        cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl;
        cout << BIRU << BOLD << "Nama Penerima : " << RESET << paketBaru.namaPenerima << endl;
        cout << BIRU << BOLD << "Alamat        : " << RESET << paketBaru.alamat << endl;
        cout << BIRU << BOLD << "Lokasi        : " << RESET << paketBaru.lokasi << endl;
        cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;
        cout << HIJAU << BOLD << "Berat (gram) [Maks 50.000g]: " << RESET;
        getline(cin, beratGram);
        bool inputValid = false; 
        if (ValidAngka(beratGram)) {
            try {
                int berat = stoi(beratGram); 
                
                if (berat > 0 && berat <= 50000) {
                    paketBaru.berat = berat; 
                    inputValid = true; 
                } else {
                    cout << "\nBERAT HARUS ANTARA 1 GRAM SAMPAI 50.000 GRAM!" << endl;
                }
            } 
            catch (const out_of_range&) {
                cout << "\nANGKA TERLALU BESAR, MELEBIHI KAPASITAS!!!" << endl;
            }
        } 
        if (inputValid) {
            break;
        }
        // Jalur penahan layar HANYA JIKA terjadi kesalahan input
        tekanEnter();
    } while (true);

    // Input Tipe Paket
    bersihkanLayar();
    HeaderTambahPaket();
    cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl;
    cout << BIRU << BOLD << "Nama Penerima : " << RESET << paketBaru.namaPenerima << endl;
    cout << BIRU << BOLD << "Alamat        : " << RESET << paketBaru.alamat << endl;
    cout << BIRU << BOLD << "Lokasi        : " << RESET << paketBaru.lokasi << endl;
    cout << BIRU << BOLD << "Berat         : " << RESET << paketBaru.berat << " gram" << endl;
    cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;
    cout << HIJAU << BOLD << "Pilih Tipe Paket:" << RESET << endl;
    // Siapkan daftar pilihan
    vector<string> listTipe = {
        "Elektronik (+Rp20.000)", 
        "Pecah Belah (+Rp15.000)", 
        "Lainnya (+Rp10.000)", 
        "Dokumen (Gratis)"
    };

    // Panggil MenuScroll
    opsiTipe = MenuScroll("PILIH TIPE PAKET", listTipe);

    // Simpan ke struct berdasarkan pilihan (1, 2, 3, atau 4)
    switch (opsiTipe) {
        case 1: 
            paketBaru.tipe = "Elektronik"; 
            break;
        case 2: 
            paketBaru.tipe = "Pecah Belah"; 
            break;
        case 3: 
            paketBaru.tipe = "Lainnya"; 
            break;
        case 4: 
            paketBaru.tipe = "Dokumen"; 
            break;
    }

    paketBaru.ongkir = HitungOngkir(paketBaru.berat, opsiLokasi, opsiTipe);

//metode pembayaran
    bersihkanLayar();
    HeaderTambahPaket();
    cout << BIRU << BOLD << "Nama Pengirim  : " << RESET << paketBaru.namaPengirim << endl;
    cout << BIRU << BOLD << "Nama Penerima  : " << RESET << paketBaru.namaPenerima << endl;
    cout << BIRU << BOLD << "Alamat         : " << RESET << paketBaru.alamat << endl;
    cout << BIRU << BOLD << "Lokasi         : " << RESET << paketBaru.lokasi << endl;
    cout << BIRU << BOLD << "Berat          : " << RESET << paketBaru.berat << " gram" << endl;  
    cout << BIRU << BOLD << "Tipe Paket     : " << RESET << paketBaru.tipe << endl;
    cout << BIRU << BOLD << "------------------------------" << RESET << endl;
    cout << BIRU << BOLD << "Total Ongkir         : Rp" << RESET << paketBaru.ongkir << endl;
    cout << BIRU << BOLD << "=============================" << RESET << endl;
    cout << HIJAU << BOLD << "Pilih Metode Pembayaran:" << RESET << endl;
    vector<string> listBayar = {
        "Bayar di Tempat",
        "COD (Bayar di Tujuan)"
    };
    // Panggil fungsi scroll
    int opsiBayar = MenuScroll("METODE PEMBAYARAN", listBayar);
        // Simpan ke struct berdasarkan pilihan (1 atau 2)
    paketBaru.pembayaran = listBayar[opsiBayar - 1];

        // Generate Resi dan Cek Unik
    do {
        resiBaru = generateResi();
    } while (cekResiDiJson(resiBaru)); // Pastikan resi unik di JSON

    paketBaru.resi = resiBaru;
    paketBaru.status = "Diproses";
    paketBaru.pemilik = "Admin";
    savePaketToJson(paketBaru);

bersihkanLayar();
    cout << HIJAU << BOLD << "==========================================" << RESET << endl;
    cout << HIJAU << BOLD << "         PAKET BERHASIL DISIMPAN          " << RESET << endl;
    cout << HIJAU << BOLD << "==========================================" << RESET << endl;
    cout << CYAN << " [1] " << BOLD << "No. Resi       : " << RESET << BOLD << paketBaru.resi << RESET << endl;
    cout << CYAN << " [2] " << BOLD << "Pengirim       : " << RESET << paketBaru.namaPengirim << endl;
    cout << CYAN << " [3] " << BOLD << "Penerima       : " << RESET << paketBaru.namaPenerima << endl;
    cout << CYAN << " [4] " << BOLD << "Alamat         : " << RESET << paketBaru.alamat << endl;
    cout << CYAN << " [5] " << BOLD << "Lokasi         : " << RESET << paketBaru.lokasi << endl;
    cout << CYAN << " [6] " << BOLD << "Berat Paket    : " << RESET << paketBaru.berat << " gram" << endl;  
    cout << CYAN << " [7] " << BOLD << "Tipe Barang    : " << RESET << paketBaru.tipe << endl;
    cout << CYAN << " [8] " << BOLD << "Metode Bayar   : " << RESET << paketBaru.pembayaran << endl;
    cout << CYAN << " [9] " << BOLD << "Status Paket   : " << RESET << HIJAU << paketBaru.status << RESET << endl;
    cout << HIJAU << BOLD << " ==========================================" << RESET << endl;
    cout << HIJAU << BOLD << "  TOTAL ONGKIR   : Rp " << paketBaru.ongkir << RESET << endl;
    cout << HIJAU << BOLD << " ==========================================" << RESET << endl;
    cout << "  " << HIJAU << BOLD << "Data telah tercatat di " << RESET << CYAN << "database/paket.json" << RESET << endl;
    cout << endl;
    tekanEnter();
}
