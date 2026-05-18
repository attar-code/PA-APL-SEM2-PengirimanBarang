#include <iostream>
#include <fstream>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;

extern Paket paket[100];
extern int jumlahPaket;

void loadPaket(){

    ifstream file("database/paket.json");
    json j;
    file >> j;

    jumlahPaket = j.size();

    for(int i = 0; i < jumlahPaket; i++){

        paket[i].resi = j[i]["resi"];
        paket[i].status = j[i]["status"];
        paket[i].namaPengirim = j[i]["namaPengirim"];
        paket[i].namaPenerima = j[i]["namaPenerima"];
        paket[i].alamat = j[i]["alamat"];
        paket[i].lokasi = j[i]["lokasi"];
        paket[i].berat = j[i]["berat"];
        paket[i].tipe = j[i]["tipe"];
        paket[i].pembayaran = j[i]["pembayaran"];
        paket[i].ongkir = j[i]["ongkir"];
        paket[i].pemilik = j[i]["pemilik"];
    }
}

void savePaket(){
 
    json j = json::array();

    for(int i = 0; i < jumlahPaket; i++){

        j.push_back({
            {"resi", paket[i].resi},
            {"namaPengirim", paket[i].namaPengirim},
            {"namaPenerima", paket[i].namaPenerima},
            {"alamat", paket[i].alamat},
            {"lokasi", paket[i].lokasi},
            {"berat", paket[i].berat},
            {"tipe", paket[i].tipe},
            {"pembayaran", paket[i].pembayaran},
            {"ongkir", paket[i].ongkir},
            {"pemilik", paket[i].pemilik},
            {"status", paket[i].status}
        });
    }

    ofstream file("database/paket.json");
    file << j.dump(4);
    file.close();
}