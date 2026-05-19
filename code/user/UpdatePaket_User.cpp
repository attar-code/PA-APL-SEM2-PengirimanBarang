#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>   
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

// VARIABEL GLOBAL EXTERNAL
extern string userAktif;
extern int HitungOngkirUser(int beratGram, int opsiLokasi, int opsiTipe);

// DEKLARASI FUNGSI BANTUAN
void tekanEnter();

// =========================================================================
// FUNGSI VALIDASI LOKAL
// =========================================================================
void ValidNamaUpdate(const string& input) {
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
            if (SpasiSebelum) throw "Tidak boleh ada spasi ganda di tengah nama!";
            SpasiSebelum = true;
        } else {
            SpasiSebelum = false;
        }
    }
}

void ValidAlamatUpdate(const string& input) {
    if (isspace(input[0]) || isspace(input.back())) {
        throw "Alamat tidak boleh diawali atau diakhiri oleh spasi!";
    }
    bool SpasiSebelum = false;
    for (char c : input) {
        if (!isalnum(c) && !isspace(c) && c != '.' && c != ',' && c != '/' && c != '-') {
            throw "Alamat mengandung karakter simbol yang tidak valid!";
        }
        if (isspace(c)) {
            if (SpasiSebelum) throw "Tidak boleh ada spasi ganda di tengah alamat!";
            SpasiSebelum = true;
        } else {
            SpasiSebelum = false;
        }
    }
}

void ValidAngkaUpdate(const string& input, const string& namaField) {
    if (isspace(input[0]) || isspace(input.back())) {
        static string msg1;
        msg1 = namaField + " tidak boleh diawali atau diakhiri oleh spasi!";
        throw msg1.c_str();
    }
    for (char c : input) {
        if (c < '0' || c > '9') {
            static string msg2;
            msg2 = namaField + " hanya boleh mengandung angka positif murni!";
            throw msg2.c_str();
        }
    }
}

// =========================================================================
// FUNGSI TAMPILAN INTERAKTIF MENU SCROLL TABEL UTAMA
// =========================================================================
int menuScrollTabelUser(string judul, const vector<int>& indeksPaketUser, const json& dataJson) {
    int posisi = 0;
    size_t totalPilihan = indeksPaketUser.size() + 1; // +1 untuk menu "Kembali"

    while (true) {
        bersihkanLayar();
        cout << KUNING << BOLD << "========================================= UPDATE DATA PAKET USER =========================================\n" << RESET;
        cout << judul << "\n";
        cout << "----------------------------------------------------------------------------------------------------------------\n";
        cout << left << "   " << setw(5) << "NO" << setw(15) << "RESI" << setw(22) << "PENGIRIM" << setw(22) << "PENERIMA" << setw(15) << "STATUS" << setw(15) << "ONGKIR" << "\n";
        cout << "----------------------------------------------------------------------------------------------------------------\n";

        for (size_t i = 0; i < indeksPaketUser.size(); i++) {
            int idx = indeksPaketUser[i];
            string resi = dataJson[idx].value("resi", "-");
            string pengirim = dataJson[idx].value("namaPengirim", "-");
            string penerima = dataJson[idx].value("namaPenerima", "-");
            string status = dataJson[idx].value("status", "-");
            long long ongkir = dataJson[idx].value("ongkir", 0LL);

            if (pengirim.length() > 18) pengirim = pengirim.substr(0, 15) + "...";
            if (penerima.length() > 18) penerima = penerima.substr(0, 15) + "...";

            if (i == (size_t)posisi) {
                cout << HIJAU << BOLD << " > " 
                     << left << setw(5)  << (i + 1)
                     << setw(15) << resi 
                     << setw(22) << pengirim 
                     << setw(22) << penerima 
                     << setw(15) << status
                     << "Rp " << setw(12) << ongkir << " <" << RESET << "\n";
            } else {
                cout << "   " 
                     << left << setw(5)  << (i + 1)
                     << setw(15) << resi 
                     << setw(22) << pengirim 
                     << setw(22) << penerima 
                     << BIRU << setw(15) << status << RESET
                     << HIJAU << "Rp " << setw(12) << ongkir << RESET << "\n";
            }
        }
        cout << "----------------------------------------------------------------------------------------------------------------\n";

        if ((size_t)posisi == indeksPaketUser.size()) {
            cout << MERAH << BOLD << " > [KEMBALI KE MENU UTAMA] <" << RESET << "\n";
        } else {
            cout << "   [KEMBALI KE MENU UTAMA]\n";
        }
        cout << "----------------------------------------------------------------------------------------------------------------\n";
        cout << " " << BOLD << "Petunjuk:" << RESET << " Gunakan tombol " << CYAN << "Panah Atas / Bawah" << RESET << " lalu tekan " << CYAN << "Enter" << RESET << " untuk memilih.\n";

        int key = _getch();
        if (key == 27) { 
            _getch(); 
            key = _getch(); 
            if (key == 65) key = 72;      
            else if (key == 66) key = 80; 
        } else if (key == 224) { 
            key = _getch(); 
        }

        if (key == 72) { 
            if (posisi > 0) posisi--;
            else posisi = totalPilihan - 1;
        } else if (key == 80) { 
            if (posisi < (int)totalPilihan - 1) posisi++;
            else posisi = 0;
        } else if (key == 13 || key == 10) { 
            return posisi + 1; 
        }
    }
}

// =========================================================================
// FUNGSI SCROLL PILIHAN FORM (SINKRON DOWNWARD LIST)
// =========================================================================
int menuScrollUpdateForm(string judul, vector<string> pilihan, string resi, string pengirim, string penerima, string alamat, string berat, string tambahanInfo = "") {
    int posisi = 0;
    while (true) {
        bersihkanLayar();
        cout << KUNING << BOLD << "============= FORM PERUBAHAN DATA PAKET =============" << RESET << endl;
        cout << BIRU << BOLD << "Resi Paket    : " << RESET << resi << endl;
        cout << BIRU << BOLD << "Nama Pengirim : " << RESET << pengirim << endl;
        cout << BIRU << BOLD << "Nama Penerima : " << RESET << penerima << endl;
        cout << BIRU << BOLD << "Alamat Tujuan : " << RESET << alamat << endl;
        if (!berat.empty()) cout << BIRU << BOLD << "Berat Barang  : " << RESET << berat << " gram" << endl;
        if (!tambahanInfo.empty()) cout << tambahanInfo;
        cout << BIRU << BOLD << "-----------------------------------------------------" << RESET << endl;
        cout << BOLD << judul << " :" << RESET << endl << endl;

        for (size_t i = 0; i < pilihan.size(); i++) {
            if (i == (size_t)posisi) {
                cout << HIJAU << BOLD << "  > " << pilihan[i] << " <" << RESET << "\n";
            } else {
                cout << "    " << pilihan[i] << "\n";
            }
        }
        cout << "\n-----------------------------------------------------\n";
        cout << " [Gunakan Panah Atas/Bawah & Tekan Enter untuk Memilih]\n";

        int key = _getch();
        if (key == 27) { _getch(); key = _getch(); if (key == 65) key = 72; else if (key == 66) key = 80; }
        else if (key == 224) { key = _getch(); }

        if (key == 72) { 
            if (posisi > 0) posisi--;
            else posisi = pilihan.size() - 1;
        } else if (key == 80) { 
            if (posisi < (int)pilihan.size() - 1) posisi++;
            else posisi = 0;
        } else if (key == 13 || key == 10) { 
            return posisi + 1; 
        }
    }
}

// =========================================================================
// PROSEDUR UTAMA
// =========================================================================
void UpdatePaket_User() {
    bersihkanLayar();
    json data;

    try {
        ifstream inputFile("database/paket.json");
        if (!inputFile.is_open()) throw runtime_error("File paket.json tidak ditemukan!");
        if (inputFile.peek() == ifstream::traits_type::eof()) throw runtime_error("Belum ada data paket!");
        inputFile >> data;
        inputFile.close();
    }
    catch (exception &e) {
        cout << "\nError : " << e.what() << endl;
        tekanEnter();
        return;
    }

    // FILTER STRATEGIS: Hanya paket milik User Aktif yang berstatus "Diproses"
    vector<int> indeksPaketUser;
    for (size_t i = 0; i < data.size(); i++) {
        string status = data[i].value("status", "-");
        if (data[i]["pemilik"] == userAktif && 
           (status == "Diproses" || status == "Diproses (COD)" || status == "Diproses (Lunas)")) {
            indeksPaketUser.push_back(i);
        }
    }

    if (indeksPaketUser.empty()) {
        cout << KUNING << BOLD << "=========== UPDATE DATA PAKET ===========" << RESET << "\n\n";
        cout << MERAH << BOLD << "ANDA TIDAK MEMILIKI DATA PAKET DENGAN STATUS 'DIPROSES' SAAT INI." << RESET << "\n";
        cout << "Paket yang sedang dalam pengiriman atau sudah sampai tidak dapat diubah.\n\n";
        tekanEnter();
        return;
    }

    int pilihan = menuScrollTabelUser("PILIH SALAH SATU BARIS DATA PAKET YANG INGIN DIUBAH:", indeksPaketUser, data);

    if (pilihan == (int)indeksPaketUser.size() + 1) {
        bersihkanLayar();
        cout << MERAH << BOLD << "UPDATE DATA PAKET DIBATALKAN." << RESET << endl;
        tekanEnter();
        return;
    }

    int targetIndeks = indeksPaketUser[pilihan - 1];
    auto &paketTarget = data[targetIndeks];

    cin.clear();
    fflush(stdin);

    // Ambil data dasar lama agar aman tidak kosong
    string namaPengirimBaru = paketTarget["namaPengirim"];
    string namaPenerimaBaru = paketTarget["namaPenerima"];
    string alamatBaru       = paketTarget["alamat"];
    int beratBaru           = paketTarget.value("berat", 0);
    
    string inputTemp;
    string lokasiBaru, tipeBaru;
    int opsiLokasi = 1; 
    int opsiTipe = 1;   

    while (true) {
        bersihkanLayar();
        cout << KUNING << BOLD << "============= FORM PERUBAHAN DATA PAKET =============" << RESET << endl;
        cout << " Resi Paket : " << BOLD << paketTarget["resi"] << RESET << endl;
        cout << " *Catatan   : Langsung tekan [ENTER] jika tidak ingin diubah" << "\n-----------------------------------------------------\n\n";
        
        cout << " Nama Pengirim Baru [" << paketTarget["namaPengirim"] << "] : ";
        getline(cin, inputTemp);
        
        if (inputTemp.empty()) { 
            break;
        }
        
        try {
            ValidNamaUpdate(inputTemp); 
            namaPengirimBaru = inputTemp; 
            break; 
        } catch (const char* errorPesan) {
            cout << MERAH << BOLD << " [!] " << errorPesan << RESET << "\n Tekan ENTER untuk coba lagi...";
            cin.get();
        }
    }

    while (true) {
        bersihkanLayar();
        cout << KUNING << BOLD << "============= FORM PERUBAHAN DATA PAKET =============" << RESET << endl;
        cout << " Resi Paket    : " << BOLD << paketTarget["resi"] << RESET << "\n";
        cout << " Nama Pengirim : " << HIJAU << namaPengirimBaru << RESET << "\n";
        cout << "-----------------------------------------------------\n\n";
        
        cout << " Nama Penerima Baru [" << paketTarget["namaPenerima"] << "] : ";
        getline(cin, inputTemp);
        
        if (inputTemp.empty()) {
            break;
        }
        
        try {
            ValidNamaUpdate(inputTemp);
            namaPenerimaBaru = inputTemp;
            break; 
        } catch (const char* errorPesan) {
            cout << MERAH << BOLD << " [!] " << errorPesan << RESET << "\n Tekan ENTER untuk coba lagi...";
            cin.get();
        }
    }

    while (true) {
        bersihkanLayar();
        cout << KUNING << BOLD << "============= FORM PERUBAHAN DATA PAKET =============" << RESET << endl;
        cout << " Resi Paket    : " << BOLD << paketTarget["resi"] << RESET << "\n";
        cout << " Nama Pengirim : " << HIJAU << namaPengirimBaru << RESET << "\n";
        cout << " Nama Penerima : " << HIJAU << namaPenerimaBaru << RESET << "\n";
        cout << "-----------------------------------------------------\n\n";
        
        cout << " Alamat Baru [" << paketTarget["alamat"] << "] : ";
        getline(cin, inputTemp);
        
        if (inputTemp.empty()) {
            break;
        }
        
        try {
            ValidAlamatUpdate(inputTemp);
            alamatBaru = inputTemp;
            break; 
        } catch (const char* errorPesan) {
            cout << MERAH << BOLD << " [!] " << errorPesan << RESET << "\n Tekan ENTER untuk coba lagi...";
            cin.get();
        }
    }

    while (true) {
        bersihkanLayar();
        cout << KUNING << BOLD << "============= FORM PERUBAHAN DATA PAKET =============" << RESET << endl;
        cout << " Resi Paket    : " << BOLD << paketTarget["resi"] << RESET << "\n";
        cout << " Nama Pengirim : " << HIJAU << namaPengirimBaru << RESET << "\n";
        cout << " Nama Penerima : " << HIJAU << namaPenerimaBaru << RESET << "\n";
        cout << " Alamat Tujuan : " << HIJAU << alamatBaru << RESET << "\n";
        cout << "-----------------------------------------------------\n\n";
        
        cout << " Masukkan berat baru (gram) [" << paketTarget["berat"] << "g] : ";
        getline(cin, inputTemp);
        
        if (inputTemp.empty()) {
            break;
        }
        
        try {
            ValidAngkaUpdate(inputTemp, "Berat barang");
            int beratTemp = stoi(inputTemp);
            if (beratTemp == 0) {
                throw "Berat barang harus lebih besar dari 0 gram!";
            }
            beratBaru = beratTemp;
            break; 
        } catch (const char* errorPesan) {
            cout << MERAH << BOLD << " [!] " << errorPesan << RESET << "\n Tekan ENTER untuk coba lagi...";
            cin.get();
        }
    }

    vector<string> listLokasi = {
        "Dalam Kota  (Tarif Normal)",
        "Luar Kota   (Tarif Luar)"
    };
    
    string labelLamaLokasi = " [Lama: " + paketTarget.value("lokasi", "-") + "]\n";
    opsiLokasi = menuScrollUpdateForm("PILIH LOKASI BARU" + labelLamaLokasi, listLokasi, paketTarget["resi"], namaPengirimBaru, namaPenerimaBaru, alamatBaru, to_string(beratBaru));
    
    if (opsiLokasi == 1) lokasiBaru = "Dalam Kota";
    else lokasiBaru = "Luar Kota";

    vector<string> listTipe = {
        "Dokumen     (+Rp 0)",
        "Elektronik (+Rp 20.000)", 
        "Pecah Belah (+Rp 15.000)", 
        "Lainnya     (+Rp 10.000)"
    };

    string infoTambahan = BIRU BOLD "Lokasi        : " RESET + lokasiBaru + "\n";
    string labelLamaTipe = " [Lama: " + paketTarget.value("tipe", "-") + "]\n";
    
    opsiTipe = menuScrollUpdateForm("PILIH TIPE PAKET BARU" + labelLamaTipe, listTipe, paketTarget["resi"], namaPengirimBaru, namaPenerimaBaru, alamatBaru, to_string(beratBaru), infoTambahan);

    if (opsiTipe == 1) tipeBaru = "Dokumen";
    else if (opsiTipe == 2) tipeBaru = "Elektronik";
    else if (opsiTipe == 3) tipeBaru = "Pecah Belah";
    else tipeBaru = "Lainnya";

    long long ongkirBaru = HitungOngkirUser(beratBaru, opsiLokasi, opsiTipe);

    paketTarget["namaPengirim"] = namaPengirimBaru;
    paketTarget["namaPenerima"] = namaPenerimaBaru;
    paketTarget["alamat"]       = alamatBaru;
    paketTarget["lokasi"]       = lokasiBaru;
    paketTarget["tipe"]         = tipeBaru;
    paketTarget["berat"]        = beratBaru;
    paketTarget["ongkir"]       = ongkirBaru; 

    ofstream outputFile("database/paket.json");
    outputFile << data.dump(4);
    outputFile.close();

    bersihkanLayar();
    cout << KUNING << BOLD << "=====================================================" << RESET << endl;
    cout << HIJAU << BOLD  << "       [SUKSES] DATA PAKET BERHASIL DIUPDATE!        " << RESET << endl;
    cout << KUNING << BOLD << "=====================================================" << RESET << endl;
    
    cout << left << setw(18) << " Nomor Resi"     << " : " << BOLD << paketTarget["resi"] << RESET << "\n";
    cout << left << setw(18) << " Nama Pengirim"   << " : " << namaPengirimBaru << "\n";
    cout << left << setw(18) << " Nama Penerima"   << " : " << namaPenerimaBaru << "\n";
    cout << left << setw(18) << " Alamat Tujuan"   << " : " << alamatBaru << "\n";
    cout << left << setw(18) << " Wilayah/Lokasi"  << " : " << lokasiBaru << "\n";
    cout << left << setw(18) << " Tipe Barang"     << " : " << tipeBaru << "\n";
    cout << left << setw(18) << " Berat Barang"    << " : " << beratBaru << " gram" << "\n";
    cout << KUNING << "-----------------------------------------------------" << RESET << "\n";
    
    cout << left << setw(18) << " Biaya Ongkir"    << " : " << HIJAU << BOLD << "Rp " << ongkirBaru << RESET << "\n";
    cout << left << setw(18) << " Status Paket"    << " : " << BIRU << BOLD << paketTarget["status"] << RESET << "\n";
    cout << KUNING << BOLD << "=====================================================" << RESET << endl;
    
    tekanEnter();
}