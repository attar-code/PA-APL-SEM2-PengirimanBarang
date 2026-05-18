#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "../database/json.hpp"
#include "../include/admin.h"
#include "../include/data.h"

#define RESET   "\033[0m"
#define MERAH   "\033[31m"
#define HIJAU   "\033[32m"
#define KUNING  "\033[33m"
#define BIRU    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

using namespace std;
using json = nlohmann::json;

void HapusRiwayatPaket_Admin() {
    ifstream file("database/paket.json");
    json dataJson;

    // 1. Validasi awal jika database kosong
    if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) {
        bersihkanLayar();
        cout << KUNING << BOLD << "==================================================" << RESET << endl;
        cout << MERAH << BOLD << "            DATABASE RIWAYAT KOSONG!              " << RESET << endl;
        cout << KUNING << BOLD << "==================================================" << RESET << endl;
        cout << "Tidak ada riwayat paket yang bisa dihapus.\n" << endl;
        tekanEnter();
        return;
    }

    file >> dataJson;
    file.close();

    // 2. Filter data: Hanya ambil paket yang "Selesai" atau "Dibatalkan"
    vector<Paket> listRiwayat;
    vector<string> pilihanMenu; // Menampung baris tabel untuk dilempar ke MenuScroll

    for (const auto& item : dataJson) {
        string status = item.value("status", "-");
        if (status == "Selesai" || status == "Dibatalkan") {
            Paket p;
            p.resi         = item.value("resi", "-");
            p.namaPengirim = item.value("namaPengirim", "-");
            p.namaPenerima = item.value("namaPenerima", "-");
            p.alamat       = item.value("alamat", "-");
            p.lokasi       = item.value("lokasi", "-");
            p.berat        = item.value("berat", 0LL);
            p.tipe         = item.value("tipe", "-");
            p.ongkir       = item.value("ongkir", 0LL);
            p.status       = status;
            p.pemilik      = item.value("pemilik", "-");
            
            listRiwayat.push_back(p);

            // Trik cerdas: Satukan data lengkap menjadi satu baris string dengan format tabel (setw)
            stringstream ss;
            ss << left 
            << setw(10) << p.resi
            << setw(14) << p.namaPengirim
            << setw(14) << p.namaPenerima
            << setw(25) << (p.alamat.length() > 22 ? p.alamat.substr(0, 22) + "..." : p.alamat)
            << setw(12) << p.lokasi
            << setw(8)  << (to_string(p.berat) + "g")
            << setw(12) << p.tipe
            << "Rp " << setw(9) << p.ongkir
            << "[" << p.status << "]";
            
            pilihanMenu.push_back(ss.str());
        }
    }

    // Jika tidak ada data riwayat sama sekali
    if (listRiwayat.empty()) {
        bersihkanLayar();
        cout << KUNING << BOLD << "==================================================" << RESET << endl;
        cout << KUNING << BOLD << "         TIDAK ADA RIWAYAT YANG SELESAI           " << RESET << endl;
        cout << KUNING << BOLD << "==================================================" << RESET << endl;
        cout << "Belum ada paket dengan status 'Selesai' atau 'Dibatalkan'.\n" << endl;
        tekanEnter();
        return;
    }

    // Tambahkan opsi kembali di paling bawah
    pilihanMenu.push_back("<< Kembali ke Menu Utama >>");

    // 3. Buat judul MenuScroll berbentuk Header Tabel agar pas dilempar ke MenuScroll posisinya presisi
    stringstream headerSs;
    headerSs << "HAPUS RIWAYAT PAKET (Gunakan Panah & Enter)\n"
            << "   " << setfill('-') << setw(115) << "" << setfill(' ') << "\n"
            << "   " << left 
            << setw(10) << "Resi" 
            << setw(14) << "Pengirim" 
            << setw(14) << "Penerima" 
            << setw(25) << "Alamat" 
            << setw(12) << "Lokasi"
            << setw(8)  << "Berat" 
            << setw(12) << "Tipe"
            << setw(12) << "Ongkir" 
            << "Status\n"
            << "   " << setfill('-') << setw(115) << "" << setfill(' ');

    // 4. Panggil MenuScroll dengan header tabel lengkap
    int indeksTerpilih = MenuScroll(headerSs.str(), pilihanMenu);

    // Jika user memilih opsi paling bawah (Kembali)
    if (indeksTerpilih == pilihanMenu.size()) {
        return;
    }

    // Ambil paket asli yang dipilih berdasarkan indeks
    Paket paketTarget = listRiwayat[indeksTerpilih - 1];

    // 5. Menu Konfirmasi Penghapusan
    bersihkanLayar();
    vector<string> opsiKonfirmasi = {
        "Ya, Hapus Permanen dari Riwayat",
        "Tidak Jadi (Kembali)"
    };
    
    int konfirmasi = MenuScroll("HAPUS RESI [" + paketTarget.resi + "] DARI RIWAYAT?", opsiKonfirmasi);

    // 6. Eksekusi jika pilih "Ya"
    if (konfirmasi == 1) {
        json dataBaru = json::array();

        // Cari dan eliminasi paket yang dipilih
        for (const auto& item : dataJson) {
            if (item.contains("resi") && item["resi"] == paketTarget.resi) {
                continue; // Skip data yang dihapus
            }
            dataBaru.push_back(item);
        }

        // Tulis kembali ke file paket.json
        ofstream outputFile("database/paket.json");
        if (outputFile.is_open()) {
            outputFile << dataBaru.dump(4);
            outputFile.close();

            bersihkanLayar();
            cout << HIJAU << BOLD << "==================================================" << RESET << endl;
            cout << HIJAU << BOLD << "          RIWAYAT BERHASIL DIHAPUS                " << RESET << endl;
            cout << HIJAU << BOLD << "==================================================" << RESET << endl;
            cout << " Data paket dengan Resi " << CYAN << BOLD << paketTarget.resi << RESET 
                << " telah dihapus permanen dari riwayat.\n" << endl;
        } else {
            cout << MERAH << BOLD << "\n[EROR] Gagal membuka database untuk menyimpan perubahan!" << RESET << endl;
        }
    } else {
        bersihkanLayar();
        cout << KUNING << BOLD << "Penghapusan dibatalkan. Data riwayat tetap aman.\n" << RESET << endl;
    }

    tekanEnter();
}