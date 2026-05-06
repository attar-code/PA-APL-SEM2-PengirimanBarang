#ifndef MODELS_H
#define MODELS_H

#include <string>
using namespace std;

struct Paket {
    string resi;
    string namaPengirim;
    string namaPenerima;
    string alamat;
    int berat;
    string tipe;
    string status;
    string pemilik;
};

struct User {
    string username;
    string password;
};

#define MAX_PAKET 100
#endif