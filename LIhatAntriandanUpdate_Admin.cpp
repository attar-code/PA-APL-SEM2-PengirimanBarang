#include <iostream>
#include "admin.h"
#include "data.h"

using namespace std;

int HitungStatus(Paket DaftarPaket[], int jumlahPaket, string status){

    int jumlah = 0;

    for(int i = 0; i < jumlahPaket; i++){

        if(DaftarPaket[i].status == status){
            jumlah++;
        }
    }

    return jumlah;
}

void AntriandanUpdateStatus(Paket DaftarPaket[], int jumlahPaket){
    loadPaket();

    system("cls");

    string resiCari;

    int jumlahDiproses = 0;
    int jumlahDikirim = 0;
    int jumlahSelesai = 0;

    // HITUNG JUMLAH STATUS
    for(int i = 0; i < jumlahPaket; i++){

        if(DaftarPaket[i].status == "Diproses"){
            jumlahDiproses++;
        }

        else if(DaftarPaket[i].status == "Dikirim"){
            jumlahDikirim++;
        }

        else if(DaftarPaket[i].status == "Selesai"){
            jumlahSelesai++;
        }
    }

    // TAMPILKAN STATISTIK
    cout << "=========== STATUS GUDANG ===========\n\n";

    cout << "Diproses : " << jumlahDiproses << " paket\n";
    cout << "Dikirim  : " << jumlahDikirim << "/5 paket\n";
    cout << "Selesai  : " << jumlahSelesai << " paket\n";

    // TAMPILKAN PAKET AKTIF
    cout << "\n=====================================\n";
    cout << "        DAFTAR PAKET AKTIF\n";
    cout << "=====================================\n";

    bool adaPaket = false;

    for(int i = 0; i < jumlahPaket; i++){

        if(
            DaftarPaket[i].status == "Diproses" ||
            DaftarPaket[i].status == "Dikirim"
        ){

            adaPaket = true;

            cout << "\nResi     : " << DaftarPaket[i].resi << endl;
            cout << "Pengirim : " << DaftarPaket[i].namaPengirim << endl;
            cout << "Status   : " << DaftarPaket[i].status << endl;
        }
    }

    if(!adaPaket){
        cout << "\nTidak ada paket aktif.\n";
        tekanEnter();
        return;
    }

    // INPUT RESI
    cout << "\nMasukkan resi paket : ";
    cin >> resiCari;

    // CARI PAKET
    for(int i = 0; i < jumlahPaket; i++){

        if(DaftarPaket[i].resi == resiCari){

            // DIPROSES -> DIKIRIM
            if(DaftarPaket[i].status == "Diproses"){

                if(jumlahDikirim >= 5){

                    cout << "\n====================================\n";
                    cout << "ANTRIAN PENGIRIMAN PENUH!\n";
                    cout << "Paket harus menunggu.\n";
                    cout << "====================================\n";

                    tekanEnter();
                    return;
                }

                DaftarPaket[i].status = "Dikirim";
                savePaket();

                cout << "\n====================================\n";
                cout << "Status berhasil diubah menjadi DIKIRIM\n";
                cout << "====================================\n";

                tekanEnter();
                return;
            }

            // DIKIRIM -> SELESAI
            else if(DaftarPaket[i].status == "Dikirim"){

                DaftarPaket[i].status = "Selesai";
                savePaket();

                cout << "\n====================================\n";
                cout << "Status berhasil diubah menjadi SELESAI\n";
                cout << "====================================\n";

                tekanEnter();
                return;
            }
        }
    }

    cout << "\nResi tidak ditemukan!\n";
    tekanEnter();
}