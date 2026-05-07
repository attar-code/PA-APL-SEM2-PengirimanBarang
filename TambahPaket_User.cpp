#include <iostream>
#include <string>
#include "data.h"

using namespace std;

extern Paket paket[MAX_PAKET];
extern int jumlahPaket;

extern string userAktif;

void TambahPaket_User() {
    system("cls");

    cout << "\n=== TAMBAH PAKET ===\n";

    cout << "Nama Pengirim : ";
    getline(cin, paket[jumlahPaket].namaPengirim);

    cout << "Nama Penerima : ";
    getline(cin, paket[jumlahPaket].namaPenerima);

    cout << "Alamat Tujuan : ";
    getline(cin, paket[jumlahPaket].alamat);

    cout << "Berat Barang (satuan gram) : ";
    cin >> paket[jumlahPaket].berat;
    cin.ignore();

    cout << "Tipe Barang   : ";
    getline(cin, paket[jumlahPaket].tipe);

    if (
        paket[jumlahPaket].namaPengirim.empty() ||
        paket[jumlahPaket].namaPenerima.empty() ||
        paket[jumlahPaket].alamat.empty() ||
        paket[jumlahPaket].tipe.empty() ||
        paket[jumlahPaket].berat <= 0
    ) {

        cout << "\nERROR: Data tidak lengkap!\n";

        tekanEnter();
        return;
    }

    paket[jumlahPaket].resi = "RESI" + to_string(jumlahPaket + 1);

    paket[jumlahPaket].status = "Menunggu Diproses";

    paket[jumlahPaket].pemilik = userAktif;

    jumlahPaket++;

    cout << "\nPaket berhasil ditambahkan!\n";
    cout << "Nomor Resi : " << paket[jumlahPaket - 1].resi << endl;
    cout << "Status     : Menunggu Diproses\n";

    cout << "\nData pengiriman telah ditambahkan dan menunggu diproses admin.\n";

    tekanEnter();
}
