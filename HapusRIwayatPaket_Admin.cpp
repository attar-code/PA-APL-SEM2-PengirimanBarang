#include<iostream>
#include<string>
#include<iomanip>
#include "admin.h"
#include "data.h"   

using namespace std;
void HapusRiwayatPaket(Paket DaftarPaket[], int &jumlahPaket) {
    if (jumlahPaket == 0) {
        cout << "\nTidak ada paket yang dapat dihapus.\n";
        tekanEnter();
        return;
    }

    string resiHapus;
    system("clear"); 
    
    cout << "============================================================" << endl;
    cout << "                DAFTAR PAKET DI GUDANG                     " << endl;
    cout << "============================================================" << endl;
    cout << left << setw(12) << "RESI" << " | " << setw(20) << "PENERIMA" << " | " << "STATUS" << endl;
    cout << "------------------------------------------------------------" << endl;
    
    for (int i = 0; i < jumlahPaket; i++) {
        cout << left << setw(12) << DaftarPaket[i].resi << " | " 
            << setw(20) << DaftarPaket[i].namaPenerima << " | " 
            << DaftarPaket[i].status << endl;
    }
    cout << "------------------------------------------------------------" << endl;
    
    cout << "Masukkan nomor resi yang ingin dihapus (Case Sensitive): ";
    cin >> resiHapus;

    int indexHapus = -1;
    for (int i = 0; i < jumlahPaket; i++) {
        if (DaftarPaket[i].resi == resiHapus) {
            indexHapus = i;
            break;
        }
    }

    if (indexHapus == -1) {
        cout << "\n[!] Paket dengan nomor resi " << resiHapus << " tidak ditemukan.\n";
    } else {
        for (int i = indexHapus; i < jumlahPaket - 1; i++) {
            DaftarPaket[i] = DaftarPaket[i + 1];
        }
        jumlahPaket--; 
        cout << "\nPaket dengan nomor resi " << resiHapus << " berhasil dihapus.\n";
    }

    tekanEnter();
}