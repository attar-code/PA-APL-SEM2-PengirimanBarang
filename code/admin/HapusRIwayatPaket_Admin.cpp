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
    vector<string> pilihanMenu; 

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
            
            // Menu scroll cukup tampilkan informasi singkat yang manis
            pilihanMenu.push_back(p.resi + " -> " + p.namaPengirim);
        }
    }

    if (listRiwayat.empty()) {
        bersihkanLayar();
        cout << KUNING << BOLD << "==================================================" << RESET << endl;
        cout << KUNING << BOLD << "         TIDAK ADA RIWAYAT YANG SELESAI           " << RESET << endl;
        cout << KUNING << BOLD << "==================================================" << RESET << endl;
        cout << "Belum ada paket dengan status 'Selesai' atau 'Dibatalkan'.\n" << endl;
        tekanEnter();
        return;
    }

    // Pilihan kembali diletakkan di paling bawah
    pilihanMenu.push_back("<< Kembali ke Menu Utama >>");

    // 3. SATUKAN TABEL DATA LENGKAP KEDALAM STRINGSTREAM JUDUL
    // Trik ini memaksa MenuScroll menggambar ulang tabel SETIAP KALI panah ditekan (Anti-Menumpuk!)
    stringstream headerSs;
    headerSs << "========================================= DAFTAR RIWAYAT PAKET GUDANG =========================================\n"
             << left << BOLD
             << setw(12) << "Resi" 
             << setw(13) << "Pengirim" 
             << setw(13) << "Penerima" 
             << setw(22) << "Alamat" 
             << setw(10) << "Lokasi"
             << setw(8)  << "Berat" 
             << setw(12) << "Tipe"
             << setw(12) << "Ongkir" 
             << "Status" << RESET << "\n"
             << KUNING << string(110, '-') << RESET << "\n";

    for (const auto& p : listRiwayat) {
        headerSs << left
                 << setw(12) << p.resi
                 << setw(13) << (p.namaPengirim.length() > 11 ? p.namaPengirim.substr(0, 10) + ".." : p.namaPengirim)
                 << setw(13) << (p.namaPenerima.length() > 11 ? p.namaPenerima.substr(0, 10) + ".." : p.namaPenerima)
                 << setw(22) << (p.alamat.length() > 19 ? p.alamat.substr(0, 19) + "..." : p.alamat)
                 << setw(10) << p.lokasi
                 << setw(8)  << (to_string(p.berat) + "g")
                 << setw(12) << p.tipe
                 << "Rp " << setw(9) << p.ongkir
                 << (p.status == "Selesai" ? HIJAU : MERAH) << "[" << p.status << "]" << RESET << "\n";
    }
    headerSs << KUNING << BOLD << "===============================================================================================================\n" << RESET
             << "\nPILIH PAKET YANG INGIN DIHAPUS PERMANEN:";

    // 4. Panggil MenuScroll dengan judul super lengkap
    int indeksTerpilih = MenuScroll(headerSs.str(), pilihanMenu);

    // FIX LOGIKA KEMBALI: Jika milih paling bawah, langsung keluar tanpa konfirmasi
    if (indeksTerpilih == pilihanMenu.size() - 1) {
        return;
    }

    // Ambil target paket asli berdasarkan indeks scroller yang valid
    Paket paketTarget = listRiwayat[indeksTerpilih];

    // 5. Menu Konfirmasi Penghapusan
    vector<string> opsiKonfirmasi = {
        "Ya, Hapus Permanen",
        "Tidak Jadi (Kembali)"
    };
    
    int konfirmasi = MenuScroll("HAPUS RESI [" + paketTarget.resi + "] DARI RIWAYAT?", opsiKonfirmasi);

    // 6. Eksekusi hapus jika pilih opsi ke-0 ("Ya, Hapus Permanen")
    if (konfirmasi == 0) {
        json dataBaru = json::array();

        for (const auto& item : dataJson) {
            if (item.contains("resi") && item["resi"] == paketTarget.resi) {
                continue; 
            }
            dataBaru.push_back(item);
        }

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
            cout << MERAH << BOLD << "\n[EROR] Gagal menyimpan perubahan database!" << RESET << endl;
        }
    } else {
        bersihkanLayar();
        cout << KUNING << BOLD << "Penghapusan dibatalkan. Data riwayat tetap aman.\n" << RESET << endl;
    }

    tekanEnter();
}