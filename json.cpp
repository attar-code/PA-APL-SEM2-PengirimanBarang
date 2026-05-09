#include <fstream>
#include "data.h"
#include "database/json.hpp"

using json = nlohmann::json;

extern Paket paket[100];
extern int jumlahPaket;

void loadPaket() {

    ifstream inputFile("paket.json");

    if (!inputFile.is_open()) {
        return;
    }

    json data;

    inputFile >> data;

    inputFile.close();

    jumlahPaket = 0;

    for (int i = 0; i < data.size(); i++) {

        paket[i].resi = data[i]["resi"];
        paket[i].namaPengirim = data[i]["namaPengirim"];
        paket[i].namaPenerima = data[i]["namaPenerima"];
        paket[i].alamat = data[i]["alamat"];
        paket[i].berat = data[i]["berat"];
        paket[i].tipe = data[i]["tipe"];
        paket[i].status = data[i]["status"];
        paket[i].pemilik = data[i]["pemilik"];
        paket[i].ongkir = data[i]["ongkir"];

        jumlahPaket++;
    }
}

void savePaket() {

    json data = json::array();

    for (int i = 0; i < jumlahPaket; i++) {

        json item = {

            {"resi", paket[i].resi},
            {"namaPengirim", paket[i].namaPengirim},
            {"namaPenerima", paket[i].namaPenerima},
            {"alamat", paket[i].alamat},
            {"berat", paket[i].berat},
            {"tipe", paket[i].tipe},
            {"status", paket[i].status},
            {"pemilik", paket[i].pemilik},
            {"ongkir", paket[i].ongkir}
        };

        data.push_back(item);
    }

    ofstream outputFile("paket.json");

    outputFile << data.dump(4);

    outputFile.close();
}