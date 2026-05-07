#include <iostream>
#include <string>
#include "data.h"

using namespace std;

extern Paket paket[MAX_PAKET];
extern int jumlahPaket;

extern string userAktif;

void UpdatePaket_User() {

    system("cls");

    cout << "=== UPDATE PAKET ===\n";

    bool ditemukan = false;

    for (int i = 0; i < jumlahPaket; i++) {

        if (paket[i].pemilik == userAktif) {

            cout << "\nData Paket Ke-" << i + 1 << endl;

            cout << "Resi           : "
                 << paket[i].resi << endl;

            cout << "Nama Pengirim  : "
                 << paket[i].namaPengirim << endl;

            cout << "Nama Penerima  : "
                 << paket[i].namaPenerima << endl;

            cout << "Alamat         : "
                 << paket[i].alamat << endl;

            cout << "Berat          : "
                 << paket[i].berat << " gram" << endl;

            cout << "Tipe Barang    : "
                 << paket[i].tipe << endl;

            cout << "Status         : "
                 << paket[i].status << endl;

            ditemukan = true;
        }
    }

    if (!ditemukan) {

        cout << "\nBelum ada paket!\n";
        tekanEnter();
        return;
    }

    int nomor;

    cout << "\nPilih nomor paket yang ingin diupdate : ";
    cin >> nomor;
    cin.ignore();

    nomor--;

    if (nomor < 0 || nomor >= jumlahPaket) {

        cout << "\nNomor paket tidak valid!\n";
        tekanEnter();
        return;
    }

    if (paket[nomor].pemilik != userAktif) {

        cout << "\nAnda tidak dapat mengubah paket ini!\n";
        tekanEnter();
        return;
    }

    if (paket[nomor].status != "Menunggu Diproses") {

        cout << "\nPaket sudah diproses admin dan tidak bisa diupdate!\n";
        tekanEnter();
        return;
    }

    cout << "\n=== UPDATE DATA PAKET ===\n";

    cout << "Nama Pengirim Baru : ";
    getline(cin, paket[nomor].namaPengirim);

    cout << "Nama Penerima Baru : ";
    getline(cin, paket[nomor].namaPenerima);

    cout << "Alamat Baru : ";
    getline(cin, paket[nomor].alamat);

    cout << "Berat Baru : ";
    cin >> paket[nomor].berat;
    cin.ignore();

    cout << "Tipe Barang Baru : ";
    getline(cin, paket[nomor].tipe);

    if (
        paket[nomor].namaPengirim.empty() ||
        paket[nomor].namaPenerima.empty() ||
        paket[nomor].alamat.empty() ||
        paket[nomor].tipe.empty() ||
        paket[nomor].berat <= 0
    ) {

        cout << "\nERROR: Data tidak valid!\n";
        tekanEnter();
        return;
    }

    cout << "\nData paket berhasil diupdate!\n";

    tekanEnter();
}