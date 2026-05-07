#include <iostream>
#include <iomanip>
#include <string>
#include "admin.h"
#include "data.h"
using namespace std;

void LihatSemuaPaket(Paket DaftarPaket[], int jumlahPaket) {
    cout << "\n=============================================================================================" << endl;
    cout <<"| " << left << setw(10) << "RESI" 
        << "| " << setw(15) << "PENGIRIM" 
        << "| " << setw(15) << "PENERIMA" 
        << "| " << setw(10) << "BERAT" 
        << "| " << setw(10) << "TIPE" 
        << "| " << setw(12) << "STATUS" << " |" 
        << "| " << setw(12) << "ONGKIR" << " |" << endl;    
    cout << "=============================================================================================" << endl;

    if (jumlahPaket == 0) {
        cout << "|                      TIDAK ADA DATA PAKET DI DALAM GUDANG                        |" << endl;
    } else {
        for (int i = 0; i < jumlahPaket; i++) {
            cout << "| " << left << setw(10) << DaftarPaket[i].resi
                << "| " << setw(15) << DaftarPaket[i].namaPengirim
                << "| " << setw(15) << DaftarPaket[i].namaPenerima
                << "| " << setw(7) << DaftarPaket[i].berat << " gr "
                << "| " << setw(10) << DaftarPaket[i].tipe
                << "| " << setw(12) << DaftarPaket[i].status << " |"
                << "| " << setw(12) << DaftarPaket[i].ongkir << " |" << endl;
        }
    }
    cout << "=============================================================================================" << endl;
    
    // Fungsi bantuan untuk menjeda layar (pastikan sudah didefinisikan)
    tekanEnter(); 
}