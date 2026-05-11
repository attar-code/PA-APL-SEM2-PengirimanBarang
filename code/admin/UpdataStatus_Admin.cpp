#include<iostream>
#include "include/admin.h"
#include "include/data.h"

void UpdateStatusPaket(Paket DaftarPaket[], int jumlahPaket) {
    string cariResi;
    bool ditemukan = false;

    cout << "\n=== UPDATE STATUS PAKET ===\n";
    cout << "Masukkan resi paket: ";
    cin >> cariResi;

    for (int i = 0; i < jumlahPaket; i++) {

        if (DaftarPaket[i].resi == cariResi) {

            ditemukan = true;

            cout << "\nPaket ditemukan!\n";
            cout << "Nama Pengirim : " << DaftarPaket[i].namaPengirim << endl;
            cout << "Status Lama   : " << DaftarPaket[i].status << endl;

            cout << "\nPilih Status Baru\n";
            cout << "1. Diproses\n";
            cout << "2. Dikirim\n";
            cout << "3. Diterima\n";
            cout << "Pilih: ";

            int pilih;
            cin >> pilih;

            switch(pilih) {
                case 1:
                    DaftarPaket[i].status = "Diproses";
                    break;

                case 2:
                    DaftarPaket[i].status = "Dikirim";
                    break;

                case 3:
                    DaftarPaket[i].status = "Diterima";
                    break;

                default:
                    cout << "Pilihan tidak valid!\n";
                    return;
            }

            cout << "Status berhasil diperbarui!\n";
            cout << "Status sekarang: " << DaftarPaket[i].status << endl;

            break;
        }
    }

    if (!ditemukan) {
        cout << "Resi tidak ditemukan!\n";
    }
}