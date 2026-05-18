#include <iomanip>
#include <iostream>
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

using namespace std;
using json = nlohmann::json;

int prioritas (string status){
    if (status == "Diproses") 
        return 1;
    else if 
        (status == "Dikirim") 
            return 2;
    else if 
        (status == "Selesai") 
            return 3;
    else if 
        (status == "Dibatalkan") 
            return 4;
    else 
        return 99; 
}

void BubbleSortPaket(vector<Paket>& pakets) {
    int n = pakets.size();
    bool swapped; 

    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            // Kita bandingkan menggunakan fungsi prioritas()
            if (prioritas(pakets[j].status) > prioritas(pakets[j + 1].status)) {
                // MATERI: Swap objek struct
                swap(pakets[j], pakets[j + 1]);
                swapped = true; 
            }
        }
        // Optimasi sesuai modul: Jika tidak ada pertukaran, berhenti
        if (swapped == false)
            break;
    }
}

void tampilRiwayatPaket() {
    ifstream file("database/paket.json");
    json dataJson;

    // Memanggil fungsi dari models.h yang sudah kita buat tadi
    bersihkanLayar();

    cout << CYAN << BOLD << setfill('=') << setw(135) << "" << "\n" << setfill(' ');
    cout << "                                     RIWAYAT PENGIRIMAN PAKET                                     \n";
    cout << "                          (Urutan: Diproses -> Dikirim -> Selesai)                                \n";
    cout << setfill('=') << setw(135) << "" << "\n" << RESET << setfill(' ');

    if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) {
        cout << "                    " << KUNING << "Belum ada riwayat (Database Kosong)." << RESET << "\n";
        cout << CYAN << "==================================================================================================\n" << RESET;
        tekanEnter();
        return;
    }

    try {
        file >> dataJson;
        file.close();

        // MATERI: Filter Data & Konversi ke Vector of Struct
        vector<Paket> listPaket;
        for (const auto& item : dataJson) {
            // Sesuai diskusi: Jangan tampilkan yang masih menunggu validasi
            if (item["status"] != "Menunggu Validasi Admin") {
                Paket p;
                p.resi         = item.value("resi", "-");
                p.namaPengirim = item.value("namaPengirim", "-");
                p.namaPenerima = item.value("namaPenerima", "-");
                p.alamat       = item.value("alamat", "-");
                p.lokasi       = item.value("lokasi", "-");
                p.berat        = item.value("berat", 0);
                p.tipe         = item.value("tipe", "-");
                p.ongkir       = item.value("ongkir", 0LL);
                p.status       = item.value("status", "-");
                p.pemilik      = item.value("pemilik", "Unknown"); // Admin atau Username User
                
                listPaket.push_back(p);
            }
        }

        if (listPaket.empty()) {
            cout << "                    " << KUNING << "Tidak ada paket yang telah divalidasi oleh Admin." << RESET << "\n";
            tekanEnter();
            return;
        }

        // EKSEKUSI SORTING
        BubbleSortPaket(listPaket);

        // TAMPILKAN TABEL
        cout << BOLD << left 
            << setw(10) << "Resi" 
            << setw(14) << "Pengirim" 
            << setw(14) << "Penerima" 
            << setw(30) << "Alamat" 
            << setw(12)  << "Lokasi"
            << setw(8)  << "Berat" 
            << setw(12) << "Tipe"
            << setw(12) << "Pemilik" 
            << setw(12) << "Ongkir" 
            << "Status\n" << RESET;
        cout << setfill('-') << setw(135) << "" << "\n" << setfill(' ');

        for (const auto& p : listPaket) {
            string warna;
            if (p.status == "Diproses") warna = KUNING;
            else if (p.status == "Dikirim") warna = BIRU;
            else if (p.status == "Selesai") warna = HIJAU;
            else warna = RESET;

            cout << left 
                << setw(10) << p.resi
                << setw(14) << p.namaPengirim
                << setw(14) << p.namaPenerima
                << setw(30) << p.alamat
                << setw(12) << p.lokasi
                << setw(8)  << (to_string(p.berat) + "g")
                << setw(12) << p.tipe
                << setw(12) << p.pemilik
                << "Rp " << setw(9) << p.ongkir
                << warna << BOLD << p.status << RESET << "\n";
        }

    } catch (const exception& e) {
        cout << MERAH << "Kesalahan: " << e.what() << RESET << endl;
    }

    cout << CYAN << setfill('=') << setw(135) << "" << "\n" << RESET << setfill(' ');
    tekanEnter();
}