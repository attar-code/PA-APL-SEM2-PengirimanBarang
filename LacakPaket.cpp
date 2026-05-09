#include <iostream>
#include "data.h"

void LacakPaket(Paket DaftarPaket[], int jumlahPaket){

    system("cls");
    
    string resiCari;
    bool ditemukan = false;

    cout << "========== LACAK PAKET ==========\n";

    cout << "Masukkan nomor resi : ";
    cin >> resiCari;

    // SEARCHING
    for(int i = 0; i < jumlahPaket; i++){

        if(DaftarPaket[i].resi == resiCari){

            ditemukan = true;

            cout << "\n=========== DATA PAKET ===========\n";

            cout << "Resi            : " << DaftarPaket[i].resi << endl;
            cout << "Nama Pengirim   : " << DaftarPaket[i].namaPengirim << endl;
            cout << "Nama Penerima   : " << DaftarPaket[i].namaPenerima << endl;
            cout << "Alamat          : " << DaftarPaket[i].alamat << endl;
            cout << "Berat           : " << DaftarPaket[i].berat << " gram" << endl;
            cout << "Tipe Barang     : " << DaftarPaket[i].tipe << endl;
            cout << "Status Paket    : " << DaftarPaket[i].status << endl;
            cout << "Total Ongkir    : Rp " << DaftarPaket[i].ongkir << endl;

            // STATUS VISUAL
            cout << "\n=========== TRACKING ===========\n";

            if(DaftarPaket[i].status == "Diproses"){

                cout << "[✓] Paket dibuat\n";
                cout << "[✓] Sedang diproses\n";
                cout << "[ ] Sedang dikirim\n";
                cout << "[ ] Paket selesai\n";
            }

            else if(DaftarPaket[i].status == "Dikirim"){

                cout << "[✓] Paket dibuat\n";
                cout << "[✓] Sedang diproses\n";
                cout << "[✓] Sedang dikirim\n";
                cout << "[ ] Paket selesai\n";
            }

            else if(DaftarPaket[i].status == "Selesai"){

                cout << "[✓] Paket dibuat\n";
                cout << "[✓] Sedang diproses\n";
                cout << "[✓] Sedang dikirim\n";
                cout << "[✓] Paket selesai\n";
            }

            else if(DaftarPaket[i].status == "Dibatalkan"){

                cout << "[✓] Paket dibuat\n";
                cout << "[X] Paket dibatalkan\n";
            }

            break;
        }
    }

    if(!ditemukan){

        cout << "\nResi tidak ditemukan!\n";
    }

    tekanEnter();
}