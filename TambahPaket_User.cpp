#include <iostream>
#include <string>
#include "data.h"

using namespace std;

extern Paket paket[MAX_PAKET];
extern int jumlahPaket;
extern string userAktif;

string generateResi();

bool cekResi(
    Paket DaftarPaket[],
    int jumlahPaket,
    string Cari
);

int inputAngka(string pesan);

void TambahPaket_User() {

    system("cls");

    cout << "\n=== TAMBAH PAKET ===\n";

    cout << "Nama Pengirim : ";
    getline(cin, paket[jumlahPaket].namaPengirim);

    if (paket[jumlahPaket].namaPengirim.empty()) {

        cout << "\nERROR: Nama pengirim tidak boleh kosong!\n";

        tekanEnter();
        return;
    }

    cout << "Nama Penerima : ";
    getline(cin, paket[jumlahPaket].namaPenerima);

    if (paket[jumlahPaket].namaPenerima.empty()) {

        cout << "\nERROR: Nama penerima tidak boleh kosong!\n";

        tekanEnter();
        return;
    }

    cout << "Alamat Tujuan : ";
    getline(cin, paket[jumlahPaket].alamat);

    if (paket[jumlahPaket].alamat.empty()) {

        cout << "\nERROR: Alamat tujuan tidak boleh kosong!\n";

        tekanEnter();
        return;
    }

    cout << "Berat Barang (satuan gram) : ";
    cin >> paket[jumlahPaket].berat;

    if (cin.fail() || paket[jumlahPaket].berat <= 0) {

        cout << "\nERROR: Berat barang tidak valid!\n";

        cin.clear();
        cin.ignore(1000, '\n');

        tekanEnter();
        return;
}

    cout << "Tipe Barang : ";
    getline(cin, paket[jumlahPaket].tipe);

    if (paket[jumlahPaket].tipe.empty()) {

        cout << "\nERROR: Tipe barang tidak boleh kosong!\n";

        tekanEnter();
        return;
    }

    string pembayaran;

    cout << "Pembayaran (COD/Transfer) : ";
    getline(cin, pembayaran);

    if (pembayaran.empty()) {

        cout << "\nERROR: Metode pembayaran tidak boleh kosong!\n";

        tekanEnter();
        return;
    }

    paket[jumlahPaket].resi = generateResi();

    if (
        cekResi(
            paket,
            jumlahPaket,
            paket[jumlahPaket].resi
        )
    ) {

        cout << "\nERROR: Nomor resi sudah digunakan!\n";

        tekanEnter();
        return;
    }

    paket[jumlahPaket].pemilik = userAktif;

    if (
        pembayaran == "COD" ||
        pembayaran == "cod"
    ) {

        paket[jumlahPaket].status = "Menunggu Diproses";

        jumlahPaket++;

        cout << "\nPaket berhasil ditambahkan!\n";
        cout << "Nomor Resi : "
             << paket[jumlahPaket - 1].resi << endl;

        cout << "Status     : Menunggu Diproses\n";

        cout << "\nData pengiriman telah ditambahkan dan menunggu diproses admin.\n";

        tekanEnter();
    }

    else if (
        pembayaran == "Transfer" ||
        pembayaran == "transfer"
    ) {

        system("cls");

        cout << "=== PEMBAYARAN TRANSFER ===\n";

        cout << "No. Virtual Account : 082341154761\n";

        int pilih;

        cout << "\n1. Sudah Bayar\n";
        cout << "2. Kembali\n";

        pilih = inputAngka("Pilih menu : ");

        if (pilih == 1) {

            paket[jumlahPaket].status = "Menunggu Validasi Admin";

            jumlahPaket++;

            cout << "\nPembayaran berhasil!\n";
            cout << "Status : Menunggu Validasi Admin\n";

            cout << "\nSilakan cek resi pada menu riwayat paket setelah divalidasi admin.\n";

            tekanEnter();
        }

        else if (pilih == 2) {

            cout << "\nData terhapus...\n";
            cout << "Silakan input ulang jika ingin mengirim barang.\n";

            tekanEnter();
            return;
        }

        else {

            cout << "\nMenu tidak valid!\n";

            tekanEnter();
            return;
        }
    }

    else {

        cout << "\nERROR: Metode pembayaran tidak valid!\n";

        tekanEnter();
        return;
    }
}