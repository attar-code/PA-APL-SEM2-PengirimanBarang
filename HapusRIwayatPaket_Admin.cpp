#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "admin.h"
#include "data.h"
#include "database/json.hpp"

using json = nlohmann::json;
using namespace std;

void HapusRiwayatPaket_Admin() {
    try {
        ifstream inputFile("database/paket.json");
        json data = json::array();

        // 1. Membaca data dari database JSON
        if (inputFile.is_open()) {
            if (inputFile.peek() != ifstream::traits_type::eof()) {
                inputFile >> data;
            }
            inputFile.close();
        }

        if (data.empty()) {
            cout << "\n[!] Database kosong. Tidak ada riwayat paket untuk dihapus.\n";
            tekanEnter();
            return;
        }

        // Tampilkan semua paket yang statusnya "Diterima" sebagai referensi admin
        cout << "========================================================================================\n";
        cout << "                           DAFTAR RIWAYAT PAKET YANG BISA DIHAPUS                       \n";
        cout << "========================================================================================\n";
        cout << left 
            << setw(12) << "Resi" 
            << setw(15) << "Pengirim" 
            << setw(15) << "Penerima" 
            << setw(12) << "Ongkir" 
            << setw(12) << "Pemilik" 
            << "Status\n";
        cout << "----------------------------------------------------------------------------------------\n";

        int jumlahBisaDihapus = 0;
        for (const auto& item : data) {
            string status = item["status"].get<string>();
            if (status == "Diterima" || status.find("Selesai") != string::npos) {
                cout << left 
                    << setw(12) << item["resi"].get<string>()
                    << setw(15) << item["namaPengirim"].get<string>()
                    << setw(15) << item["namaPenerima"].get<string>()
                    << "Rp " << setw(9) << item["ongkir"].get<long long>()
                    << setw(12) << item["pemilik"].get<string>()
                    << status << "\n";
                jumlahBisaDihapus++;
            }
        }

        if (jumlahBisaDihapus == 0) {
            cout << "            Tidak ada paket dengan status 'Diterima' yang bisa dihapus.                 \n";
            cout << "========================================================================================\n";
            tekanEnter();
            return;
        }
        cout << "========================================================================================\n";

        // 2. Input Nomor Resi yang ingin dihapus
        string cariResi;
        cout << "Masukkan Nomor Resi paket yang ingin dihapus (ketik 'batal' untuk keluar): ";
        cin >> cariResi;

        if (cariResi == "batal" || cariResi == "BATAL") {
            cout << "\n[!] Penghapusan dibatalkan.\n";
            tekanEnter();
            return;
        }

        // 3. Cari resi di dalam JSON dan hapus jika statusnya valid
        bool ditemukan = false;
        for (auto it = data.begin(); it != data.end(); ++it) {
            if ((*it)["resi"].get<string>() == cariResi) {
                string status = (*it)["status"].get<string>();

                // Validasi keamanan: Pastikan paket yang dicari memang berstatus "Diterima"
                if (status == "Diterima" || status.find("Selesai") != string::npos) {
                    ditemukan = true;
                    
                    // Hapus data menggunakan iterator 'it'
                    data.erase(it); 
                    break; // Keluar dari loop setelah data dihapus
                } else {
                    cout << "\n[!] Paket ditemukan, tetapi statusnya masih '" << status << "'.\n";
                    cout << "    Hanya paket dengan status 'Diterima' yang boleh dihapus!\n";
                    tekanEnter();
                    return;
                }
            }
        }

        // 4. Jika resi tidak ditemukan di database
        if (!ditemukan) {
            cout << "\n[!] Nomor Resi '" << cariResi << "' tidak ditemukan atau tidak valid untuk dihapus.\n";
            tekanEnter();
            return;
        }

        // 5. Simpan kembali perubahan ke file JSON
        ofstream outputFile("database/paket.json");
        if (outputFile.is_open()) {
            outputFile << data.dump(4);
            outputFile.close();
            
            cout << "\n======================================" << endl;
            cout << "      RIWAYAT BERHASIL DIHAPUS!       " << endl;
            cout << "======================================" << endl;
            cout << "Resi " << cariResi << " telah dihapus permanen dari database." << endl;
            cout << "======================================" << endl;
        } else {
            throw "Gagal membuka database untuk memperbarui data!";
        }

        tekanEnter();
    }
    catch (const char* msg) {
        cerr << "\nError: " << msg << endl;
        tekanEnter();
    }
}