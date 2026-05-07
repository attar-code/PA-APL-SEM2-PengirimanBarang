#include <iostream>
#include <string>
#include <limits>
#include "data.h"

using namespace std;

extern Paket paket[MAX_PAKET];
extern int jumlahPaket;
extern string userAktif;

string generateResi();
bool cekResi(Paket DaftarPaket[], int jumlahPaket, string Cari);
int inputAngka(string pesan);
long long hitungOngkir(int beratGram, int opsiLokasi, int opsiTipe);
void tekanEnter();

void TambahPaket_User() {
    system("cls");

    cout << "\n=== TAMBAH PAKET ===\n";
    cout << "Nama Pengirim : ";
    getline(cin, paket[jumlahPaket].namaPengirim);
    if (paket[jumlahPaket].namaPengirim.empty()) {
        cout << "\nERROR: Nama pengirim tidak boleh kosong!\n";
        tekanEnter(); return;
    }

    cout << "Nama Penerima : ";
    getline(cin, paket[jumlahPaket].namaPenerima);
    if (paket[jumlahPaket].namaPenerima.empty()) {
        cout << "\nERROR: Nama penerima tidak boleh kosong!\n";
        tekanEnter(); return;
    }

    cout << "Alamat Tujuan : ";
    getline(cin, paket[jumlahPaket].alamat);
    if (paket[jumlahPaket].alamat.empty()) {
        cout << "\nERROR: Alamat tujuan tidak boleh kosong!\n";
        tekanEnter(); return;
    }

    cout << "OPSI LOKASI TUJUAN:" << endl;
    cout << "1. Dalam Kota (10.000)" << endl;
    cout << "2. Luar Kota (20.000)" << endl;
    int opsiLokasi;
    cout << "Pilih opsi lokasi tujuan (1-2): ";
    cin >> opsiLokasi;

    cout << "Berat Barang (satuan gram) : ";
    cin >> paket[jumlahPaket].berat;
    if (cin.fail() || paket[jumlahPaket].berat <= 0) {
        cout << "\nERROR: Berat barang tidak valid!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        tekanEnter(); return;
    }

    cout << "Pilihan tipe barang:" << endl;
    cout << "1. Dokumen (0)" << endl;
    cout << "2. Elektronik (20.000)" << endl;
    cout << "3. Pecah Belah (15.000)" << endl;
    cout << "4. Lainnya (10.000)" << endl;
    int opsiTipe;
    cout << "Pilih opsi tipe barang (1-4): ";
    cin >> opsiTipe;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (opsiTipe == 1) paket[jumlahPaket].tipe = "Dokumen";
    else if (opsiTipe == 2) paket[jumlahPaket].tipe = "Elektronik";
    else if (opsiTipe == 3) paket[jumlahPaket].tipe = "Pecah Belah";
    else paket[jumlahPaket].tipe = "Lainnya";

    paket[jumlahPaket].ongkir = hitungOngkir(paket[jumlahPaket].berat, opsiLokasi, opsiTipe);

    string pembayaran;
    cout << "Pembayaran (COD/Transfer) : ";
    getline(cin, pembayaran);

    if (pembayaran.empty()) {
        cout << "\nERROR: Metode pembayaran tidak boleh kosong!\n";
        tekanEnter(); return;
    }

    paket[jumlahPaket].resi = generateResi();
    paket[jumlahPaket].pemilik = userAktif;

    if (pembayaran == "COD" || pembayaran == "cod") {
        paket[jumlahPaket].status = "Menunggu Diproses";

        cout << "\n=== RINCIAN PENGIRIMAN ===" << endl;
        cout << "Total Ongkir : Rp " << paket[jumlahPaket].ongkir << endl;
        cout << "Nomor Resi   : " << paket[jumlahPaket].resi << endl;
        cout << "Status       : Menunggu Diproses" << endl;
        cout << "==========================" << endl;

        jumlahPaket++;
        cout << "\nPaket berhasil ditambahkan! Menunggu diproses admin.\n";
        tekanEnter();
    }
    else if (pembayaran == "Transfer" || pembayaran == "transfer") {
        system("cls");
        cout << "=== PEMBAYARAN TRANSFER ===\n";
        cout << "Total Ongkir        : Rp " << paket[jumlahPaket].ongkir << endl;
        cout << "No. Virtual Account : 082341154761\n";

        int pilih;
        cout << "\n1. Sudah Bayar\n";
        cout << "2. Kembali\n";
        pilih = inputAngka("Pilih menu : ");

        if (pilih == 1) {
            paket[jumlahPaket].status = "Menunggu Validasi Admin";
            
            cout << "\nPembayaran berhasil!\n";
            cout << "Nomor Resi : " << paket[jumlahPaket].resi << endl;
            
            jumlahPaket++;
            tekanEnter();
        }
        else {
            cout << "\nData terhapus...\n";
            tekanEnter();
            return;
        }
    }
    else {
        cout << "\nERROR: Metode pembayaran tidak valid!\n";
        tekanEnter();
    }
}