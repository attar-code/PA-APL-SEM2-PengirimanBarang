#include <iostream>
#include <string>
#include "data.h"

using namespace std;

extern Paket paket[MAX_PAKET];
extern int jumlahPaket;

extern string userAktif;

void HapusRiwayatPaket_User() {

    system("cls");

    cout << "=== HAPUS RIWAYAT PAKET ===\n";

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

        cout << "\nBelum ada riwayat paket!\n";
        tekanEnter();
        return;
    }

    int nomor;

    cout << "\nPilih nomor paket yang ingin dihapus : ";
    cin >> nomor;
    cin.ignore();

    nomor--;

    if (nomor < 0 || nomor >= jumlahPaket) {

        cout << "\nNomor paket tidak valid!\n";
        tekanEnter();
        return;
    }

    if (paket[nomor].pemilik != userAktif) {

        cout << "\nAnda tidak dapat menghapus paket ini!\n";
        tekanEnter();
        return;
    }

    for (int i = nomor; i < jumlahPaket - 1; i++) {

        paket[i] = paket[i + 1];
    }

    jumlahPaket--;

    cout << "\nRiwayat paket berhasil dihapus!\n";

    tekanEnter();
}