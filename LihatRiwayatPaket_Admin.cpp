#include <iomanip>
#include <iostream>
#include <fstream>
#include "admin.h"
#include "data.h"
#include "database/json.hpp"

using namespace std;
using json = nlohmann::json;

void tampilRiwayatPaket() {
    ifstream file("database/paket.json");
    json data;

    cout << "==================================================================================================\n";
    cout << "                                     RIWAYAT PENGIRIMAN PAKET                                     \n";
    cout << "==================================================================================================\n";
    if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) {
        cout << "                    Belum ada riwayat pengiriman paket (Database Kosong).                     \n";
        cout << "==================================================================================================\n";
        tekanEnter();
        return;
    }

    file >> data;
    file.close();

    // Header Tabel (Ditambahkan kolom Pemilik)
    cout << left 
        << setw(12) << "Resi" 
        << setw(15) << "Pengirim" 
        << setw(15) << "Penerima" 
        << setw(10) << "Berat" 
        << setw(12) << "Tipe" 
        << setw(12) << "Ongkir" 
        << setw(12) << "Pemilik" 
        << "Status\n";
        
    cout << "--------------------------------------------------------------------------------------------------\n";

    // Looping data dari JSON untuk ditampilkan
    for (const auto& item : data) {
        cout << left 
            << setw(12) << item["resi"].get<string>()
            << setw(15) << item["namaPengirim"].get<string>()
            << setw(15) << item["namaPenerima"].get<string>()
            << setw(10) << to_string(item["berat"].get<int>()) + "g"
            << setw(12) << item["tipe"].get<string>()
            << "Rp " << setw(9) << item["ongkir"].get<long long>()
            << setw(12) << item["pemilik"].get<string>() // Menampilkan pemilik paket (Admin/User)
            << item["status"].get<string>() << "\n";
    }
    cout << "==================================================================================================\n";
}