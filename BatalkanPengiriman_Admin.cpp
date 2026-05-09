#include <iostream>
#include "admin.h"
#include "data.h"

void BatalkanPaket(Paket DaftarPaket[], int &jumlahPaket){

    system("cls");

    string resiCari;

    cout << "========== PEMBATALAN PAKET ==========\n";

    bool adaPaket = false;

    // TAMPILKAN PAKET YANG MASIH DIPROSES
    for(int i = 0; i < jumlahPaket; i++){

        if(DaftarPaket[i].status == "Diproses"){

            adaPaket = true;

            cout << "\nResi     : " << DaftarPaket[i].resi << endl;
            cout << "Pengirim : " << DaftarPaket[i].namaPengirim << endl;
            cout << "Penerima : " << DaftarPaket[i].namaPenerima << endl;
            cout << "Status   : " << DaftarPaket[i].status << endl;
        }
    }

    if(!adaPaket){

        cout << "\nTidak ada paket yang bisa dibatalkan.\n";
        tekanEnter();
        return;
    }

    // INPUT RESI
    cout << "\nMasukkan resi paket yang ingin dibatalkan : ";
    cin >> resiCari;

    // CARI PAKET
    for(int i = 0; i < jumlahPaket; i++){

        if(DaftarPaket[i].resi == resiCari){

            // CEK STATUS
            if(DaftarPaket[i].status != "Diproses"){

                cout << "\nPaket sudah dikirim dan tidak bisa dibatalkan.\n";
                tekanEnter();
                return;
            }

            // HAPUS DATA
            for(int j = i; j < jumlahPaket - 1; j++){

                DaftarPaket[j] = DaftarPaket[j + 1];
            }

            jumlahPaket--;

            cout << "\nPaket berhasil dibatalkan.\n";
            tekanEnter();
            return;
        }
    }

    cout << "\nResi tidak ditemukan.\n";
    tekanEnter();
}