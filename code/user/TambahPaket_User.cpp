#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include <vector>   
#include "../include/data.h"
#include "../database/json.hpp"

#ifdef _WIN32
#include <conio.h>
#endif

#define RESET   "\033[0m"
#define MERAH   "\033[31m"
#define HIJAU   "\033[32m"
#define KUNING  "\033[33m"
#define BIRU    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

using json = nlohmann::json;
using namespace std;

extern string userAktif;

string generateResi();
bool cekResiDiJson(const string& Cari);
void tekanEnter();
void savePaketToJson(Paket p);

void CekValidNAMA(const string& input, const string& namaField) {
    if (input.empty()) {
        throw "Input tidak boleh kosong!"; 
    }
    if (isspace(input[0]) || isspace(input.back())) {
        throw "Input tidak boleh diawali atau diakhiri oleh spasi!";
    }
    if (!isalpha(input[0]) || !isalpha(input.back())) {
        throw "Nama harus diawali dan diakhiri oleh huruf!";
    }

    bool SpasiSebelum = false;
    for (char c : input) {
        if (!isalpha(c) && !isspace(c)) {
            throw "Nama hanya boleh mengandung huruf dan spasi!";
        }
        if (isspace(c)) {
            if (SpasiSebelum) { 
                throw "Tidak boleh ada spasi ganda di tengah nama!";
            }
            SpasiSebelum = true;
        } else {
            SpasiSebelum = false;
        }
    }
}  

void CekValidAlamat(const string& input) {
    if (input.empty()) {
        throw "Alamat tidak boleh kosong!";
    }
    if (isspace(input[0]) || isspace(input.back())) {
        throw "Alamat tidak boleh diawali atau diakhiri oleh spasi!";
    }

    bool SpasiSebelum = false;
    for (char c : input) {
        if (!isalnum(c) && !isspace(c) && c != '.' && c != ',' && c != '/' && c != '-') {
            throw "Alamat mengandung karakter simbol yang tidak valid!";
        }
        if (isspace(c)) {
            if (SpasiSebelum) {
                throw "Tidak boleh ada spasi ganda di tengah alamat!";
            }
            SpasiSebelum = true;
        } else {
            SpasiSebelum = false;
        }
    }
}

void ValidAngka(const string& input, const string& namaField) {
    if (input.empty()) {
        throw (namaField + " tidak boleh kosong!").c_str();
    }
    if (isspace(input[0]) || isspace(input.back())) {
        throw (namaField + " tidak boleh diawali atau diakhiri oleh spasi!").c_str();
    }
    for (char c : input) {
        // Logika membatasi hanya angka 0-9 dari kodemu
        if (c < '0' || c > '9') {
            throw (namaField + " hanya boleh mengandung angka positif murni!").c_str();
        }
    }
}

int menuScrollInput(string judul, vector<string> pilihan) {
    int posisi = 0;
    int key;
    
    while (true) {
        bersihkanLayar();
        
        cout << CYAN << BOLD << "==================================================" << RESET << endl;
        cout << BOLD << "   " << judul << RESET << endl;
        cout << CYAN << BOLD << "==================================================" << RESET << endl;
        
        for (size_t i = 0; i < pilihan.size(); i++) {
            if (i == posisi) {
                cout << KUNING << BOLD << "  > [ " << pilihan[i] << " ] <" << RESET << endl;
            } else {
                cout << "      " << pilihan[i] << endl;
            }
        }
        cout << CYAN << "--------------------------------------------------" << RESET << endl;
        cout << "Gunakan Panah Atas/Bawah & Enter" << endl;

        key = _getch();
        if (key == 27) { 
            _getch(); 
            key = _getch();
            if (key == 65) key = 72;      
            else if (key == 66) key = 80; 
        }
        else if (key == 224) { 
            key = _getch(); 
        }

        if (key == 72) { // Panah Atas
            if (posisi > 0) posisi--;
            else posisi = pilihan.size() - 1;
        } 
        else if (key == 80) { // Panah Bawah
            if (posisi < pilihan.size() - 1) posisi++;
            else posisi = 0;
        } 
        else if (key == 13 || key == 10) { // Enter
            return posisi + 1; // Mengembalikan 1, 2, dst.
        }
    }
}

int HitungOngkirUser(int beratGram, int opsiLokasi = 1, int opsiTipe = 1) {
    int ongkirlokasi = (opsiLokasi == 1) ? 10000 : 20000;
    int tarifBerat = (beratGram * ongkirlokasi) / 1000;
    switch (opsiTipe) {
        case 1:
            return tarifBerat + 0; 
        case 2:
            return tarifBerat + 20000; 
        case 3:
            return tarifBerat + 15000; 
        case 4:
            return tarifBerat + 10000;
        default:
            return tarifBerat;      
    }
}

void tampilTambahPaket() {
    cout << CYAN << BOLD << "==================================================" << RESET << endl;
    cout << CYAN << BOLD << "                TAMBAH PAKET USER                 " << RESET << endl;
    cout << CYAN << BOLD << "==================================================" << RESET << endl;
}
void TambahPaket_User() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    Paket paketBaru;
    string input;
    string beratGram;
    int opsiLokasi;
    int opsiTipe;
    string resiBaru;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    do {
        bersihkanLayar();   
        tampilTambahPaket(); 
        cout << HIJAU << BOLD << "Nama Pengirim: " << RESET;
        getline(cin, input);
        
        try {
            CekValidNAMA(input, "Nama pengirim");
            paketBaru.namaPengirim = input;
            break;
        } catch (const char* msg) {
            cout << MERAH << BOLD << "\n [ERROR]: " << msg << RESET << endl;
            tekanEnter();
        }
    } while (true);

    do {
        bersihkanLayar();
        tampilTambahPaket();
        cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl; 
        cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;
        cout << HIJAU << BOLD << "Nama Penerima: " << RESET;
        getline(cin, input);
        
        try {
            CekValidNAMA(input, "Nama penerima");
            paketBaru.namaPenerima = input;
            break;
        } catch (const char* msg) {
            cout << MERAH << BOLD << "\n [ERROR]: " << msg << RESET << endl;
            tekanEnter();
        }
    } while (true);

    do {
        bersihkanLayar();
        tampilTambahPaket();
        cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl;
        cout << BIRU << BOLD << "Nama Penerima : " << RESET << paketBaru.namaPenerima << endl;
        cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;
        cout << HIJAU << BOLD << "Alamat: " << RESET;
        getline(cin, input);
        
        try {
            CekValidAlamat(input);
            paketBaru.alamat = input;
            break;
        } catch (const char* msg) {
            cout << MERAH << BOLD << "\n [ERROR]: " << msg << RESET << endl;
            tekanEnter();
        }
    } while (true);

    bersihkanLayar();
    tampilTambahPaket();
    cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl;
    cout << BIRU << BOLD << "Nama Penerima : " << RESET << paketBaru.namaPenerima << endl;
    cout << BIRU << BOLD << "Alamat        : " << RESET << paketBaru.alamat << endl;
    cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;
    
    vector<string> listLokasi = {
        "Dalam Kota (Rp 10.000 / Kg)",
        "Luar Kota  (Rp 20.000 / Kg)"
    };
    opsiLokasi = menuScrollInput("PILIH LOKASI PENGIRIMAN", listLokasi);
    paketBaru.lokasi = (opsiLokasi == 1) ? "Dalam Kota" : "Luar Kota";

    do {
        bersihkanLayar();
        tampilTambahPaket();
        cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl;
        cout << BIRU << BOLD << "Nama Penerima : " << RESET << paketBaru.namaPenerima << endl;
        cout << BIRU << BOLD << "Alamat        : " << RESET << paketBaru.alamat << endl;
        cout << BIRU << BOLD << "Lokasi        : " << RESET << paketBaru.lokasi << endl;
        cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;
        cout << HIJAU << BOLD << "Berat (gram) [Maks 50.000g]: " << RESET;
        getline(cin, beratGram);
        
        try {
            ValidAngka(beratGram, "Berat paket");
            int berat = stoi(beratGram); 
            
            if (berat > 0 && berat <= 50000) {
                paketBaru.berat = berat; 
                break; 
            } else {
                cout << MERAH << BOLD << "\n [ERROR]: Berat harus antara 1 gram sampai 50.000 gram!" << RESET << endl;
            }
        } 
        catch (const char* msg) {
            cout << MERAH << BOLD << "\n [ERROR]: " << msg << RESET << endl;
        }
        catch (const out_of_range&) {
            cout << MERAH << BOLD << "\n [ERROR]: Angka terlalu besar, melebihi kapasitas!" << RESET << endl;
        }
        tekanEnter();
    } while (true);

    bersihkanLayar();
    tampilTambahPaket();
    cout << BIRU << BOLD << "Nama Pengirim : " << RESET << paketBaru.namaPengirim << endl;
    cout << BIRU << BOLD << "Nama Penerima : " << RESET << paketBaru.namaPenerima << endl;
    cout << BIRU << BOLD << "Alamat        : " << RESET << paketBaru.alamat << endl;
    cout << BIRU << BOLD << "Lokasi        : " << RESET << paketBaru.lokasi << endl;
    cout << BIRU << BOLD << "Berat         : " << RESET << paketBaru.berat << " gram" << endl;
    cout << BIRU << BOLD << "--------------------------------------" << RESET << endl;

    vector<string> listTipe = {
        "Dokumen     (+Rp 0)",
        "Elektronik (+Rp 20.000)", 
        "Pecah Belah (+Rp 15.000)", 
        "Lainnya     (+Rp 10.000)"
    };

    opsiTipe = menuScrollInput("PILIH TIPE PAKET", listTipe);

    if (opsiTipe == 1) paketBaru.tipe = "Dokumen";
    else if (opsiTipe == 2) paketBaru.tipe = "Elektronik";
    else if (opsiTipe == 3) paketBaru.tipe = "Pecah Belah";
    else paketBaru.tipe = "Lainnya";

    paketBaru.ongkir = HitungOngkirUser(paketBaru.berat, opsiLokasi, opsiTipe);

    bersihkanLayar();
    tampilTambahPaket();
    cout << BIRU << BOLD << "Nama Pengirim  : " << RESET << paketBaru.namaPengirim << endl;
    cout << BIRU << BOLD << "Nama Penerima  : " << RESET << paketBaru.namaPenerima << endl;
    cout << BIRU << BOLD << "Alamat         : " << RESET << paketBaru.alamat << endl;
    cout << BIRU << BOLD << "Lokasi         : " << RESET << paketBaru.lokasi << endl;
    cout << BIRU << BOLD << "Berat          : " << RESET << paketBaru.berat << " gram" << endl;  
    cout << BIRU << BOLD << "Tipe Paket     : " << RESET << paketBaru.tipe << endl;
    cout << BIRU << BOLD << "------------------------------" << RESET << endl;
    cout << BIRU << BOLD << "Total Ongkir         : Rp" << RESET << paketBaru.ongkir << endl;
    cout << BIRU << BOLD << "=============================" << RESET << endl;
    
    vector<string> listBayar = {
        "COD (Bayar di Tempat)",
        "Transfer Bank (Perlu Validasi Admin)"
    };
    int opsiBayar = menuScrollInput("METODE PEMBAYARAN", listBayar);

    paketBaru.pemilik = userAktif; 

    if (opsiBayar == 1) { 
        paketBaru.pembayaran = "COD";
        paketBaru.status = "Diproses";

        do {
            resiBaru = generateResi();
        } while (cekResiDiJson(resiBaru)); 
        paketBaru.resi = resiBaru;
    } 
    else { 
        paketBaru.pembayaran = "Transfer";
        paketBaru.status = "Menunggu Validasi Admin";
        paketBaru.resi = "BELUM_RILIS";
    }

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
    cout << HIJAU << BOLD << " ==========================================" << endl;
    cout << "  " << HIJAU << BOLD << "Data telah tercatat atas pemilik: " << RESET << KUNING << BOLD << paketBaru.pemilik << RESET << endl;
    cout << endl;
    tekanEnter();
}

void savePaketToJson(Paket p) {
    ifstream inputFile("database/paket.json");
    json data = json::array(); 

    if (inputFile.is_open()) {
        if (inputFile.peek() != ifstream::traits_type::eof()) {
            inputFile >> data;
        }
        inputFile.close();
    }

    json paketBaruJson;
    paketBaruJson["resi"] = p.resi;
    paketBaruJson["namaPengirim"] = p.namaPengirim;
    paketBaruJson["namaPenerima"] = p.namaPenerima;
    paketBaruJson["alamat"] = p.alamat;
    paketBaruJson["lokasi"] = p.lokasi;
    paketBaruJson["berat"] = p.berat;
    paketBaruJson["tipe"] = p.tipe;
    paketBaruJson["ongkir"] = p.ongkir;
    paketBaruJson["pembayaran"] = p.pembayaran;
    paketBaruJson["status"] = p.status;
    paketBaruJson["pemilik"] = p.pemilik;

    data.push_back(paketBaruJson);

    ofstream outputFile("database/paket.json");
    if (outputFile.is_open()) {
        outputFile << data.dump(4);
        outputFile.close();
    }
}

