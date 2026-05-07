#include <iostream>
#include "data.h"

using namespace std;

extern Paket paket[MAX_PAKET];
extern int jumlahPaket;

extern string userAktif;

void LihatRiwayatPaket_User() {
    system("cls");

    cout << "=== RIWAYAT PAKET ===\n";

    bool ditemukan = false;

    for (int i = 0; i < jumlahPaket; i++) {

        if (paket[i].pemilik == userAktif) {

            ditemukan = true;

            cout << "\n============================\n";
            cout << "Nomor Resi     : " << paket[i].resi << endl;
            cout << "Nama Pengirim  : " << paket[i].namaPengirim << endl;
            cout << "Nama Penerima  : " << paket[i].namaPenerima << endl;
            cout << "Alamat Tujuan  : " << paket[i].alamat << endl;
            cout << "Berat Barang   : " << paket[i].berat << " gram" << endl;
            cout << "Tipe Barang    : " << paket[i].tipe << endl;
            cout << "Status Paket   : " << paket[i].status << endl;
        }
    }

    if (!ditemukan) {
        cout << "\nBelum ada riwayat paket.\n";
    }

    tekanEnter();
}