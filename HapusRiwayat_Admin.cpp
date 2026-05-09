#include<iostream>
#include<string>
#include<iomanip>
#include "admin.h"
#include "data.h"  


void HapusRiwayatPaketAdmin(Paket DaftarPaket[], int &jumlahPaket){

    system("cls");

    string resiCari;

    cout << "======= HAPUS RIWAYAT PAKET =======\n";

    bool adaSelesai = false;

    // TAMPILKAN PAKET YANG SUDAH SELESAI
    for(int i = 0; i < jumlahPaket; i++){

        if(DaftarPaket[i].status == "Selesai"){

            adaSelesai = true;

            cout << "\nResi     : " << DaftarPaket[i].resi << endl;
            cout << "Pengirim : " << DaftarPaket[i].namaPengirim << endl;
            cout << "Penerima : " << DaftarPaket[i].namaPenerima << endl;
            cout << "Status   : " << DaftarPaket[i].status << endl;
        }
    }

    if(!adaSelesai){

        cout << "\nTidak ada riwayat paket selesai.\n";
        tekanEnter();
        return;
    }

    // INPUT RESI
    cout << "\nMasukkan resi paket yang ingin dihapus : ";
    cin >> resiCari;

    // CARI DAN HAPUS
    for(int i = 0; i < jumlahPaket; i++){

        if(
            DaftarPaket[i].resi == resiCari &&
            DaftarPaket[i].status == "Selesai"
        ){

            // GESER ARRAY
            for(int j = i; j < jumlahPaket - 1; j++){

                DaftarPaket[j] = DaftarPaket[j + 1];
            }

            jumlahPaket--;

            cout << "\nRiwayat paket berhasil dihapus.\n";
            tekanEnter();
            return;
        }
    }

    cout << "\nResi tidak ditemukan atau paket belum selesai.\n";
    tekanEnter();
}