// #include <iostream>
// #include "admin.h"
// #include "data.h"
// #include "Database/json.hpp"

// using json = nlohmann::json;

// using namespace std;

// int HitungStatus(Paket DaftarPaket[], int jumlahPaket, string status){

//     int jumlah = 0;

//     for(int i = 0; i < jumlahPaket; i++){

//         if(paket[i].status == status){
//             jumlah++;
//         }
//     }

//     return jumlah;
// }

// void AntriandanUpdateStatus(){
//     loadPaket();

//     system("cls");

//     string resiCari;

//     int jumlahDiproses = 0;
//     int jumlahDikirim = 0;
//     int jumlahSelesai = 0;

//     for(int i = 0; i < jumlahPaket; i++){

//         if(paket[i].status == "Diproses"){
//             jumlahDiproses++;
//         }

//         else if(paket[i].status == "Dikirim"){
//             jumlahDikirim++;
//         }

//         else if(paket[i].status == "Selesai"){
//             jumlahSelesai++;
//         }
//     }

//     cout << "=========== STATUS GUDANG ===========\n\n";

//     cout << "Diproses : " << jumlahDiproses << " paket\n";
//     cout << "Dikirim  : " << jumlahDikirim << "/5 paket\n";
//     cout << "Selesai  : " << jumlahSelesai << " paket\n";

//     // TAMPILKAN PAKET AKTIF
//     cout << "\n=====================================\n";
//     cout << "        DAFTAR PAKET AKTIF\n";
//     cout << "=====================================\n";

//     bool adaPaket = false;

//     for(int i = 0; i < jumlahPaket; i++){

//         if(
//             paket[i].status == "Diproses" ||
//             paket[i].status == "Dikirim"
//         ){

//             adaPaket = true;

//             cout << "\nResi     : " << paket[i].resi << endl;
//             cout << "Pengirim : " << paket[i].namaPengirim << endl;
//             cout << "Status   : " << paket[i].status << endl;
//         }
//     }

//     if(!adaPaket){
//         cout << "\nTidak ada paket aktif.\n";
//         tekanEnter();
//         return;
//     }

//     cout << "\nMasukkan resi paket : ";
//     cin >> resiCari;

//     for(int i = 0; i < jumlahPaket; i++){

//         if(paket[i].resi == resiCari){

//             // DIPROSES -> DIKIRIM
//             if(paket[i].status == "Diproses"){

//                 if(jumlahDikirim >= 5){

//                     cout << "\n====================================\n";
//                     cout << "ANTRIAN PENGIRIMAN PENUH!\n";
//                     cout << "Paket harus menunggu.\n";
//                     cout << "====================================\n";

//                     tekanEnter();
//                     return;
//                 }

//                 paket[i].status = "Dikirim";
//                 savePaket();

//                 cout << "\n====================================\n";
//                 cout << "Status berhasil diubah menjadi DIKIRIM\n";
//                 cout << "====================================\n";

//                 tekanEnter();
//                 return;
//             }

//             // DIKIRIM -> SELESAI
//             else if(paket[i].status == "Dikirim"){

//                 paket[i].status = "Selesai";
//                 savePaket();

//                 cout << "\n====================================\n";
//                 cout << "Status berhasil diubah menjadi SELESAI\n";
//                 cout << "====================================\n";

//                 tekanEnter();
//                 return;
//             }
//         }
//     }

//     cout << "\nResi tidak ditemukan!\n";
//     tekanEnter();
// }


#include <iostream>
#include "admin.h"
#include "data.h"
#include "Database/json.hpp"

using json = nlohmann::json;
using namespace std;

/* =========================
   HITUNG STATUS
   ========================= */
int HitungStatus(Paket DaftarPaket[], int jumlahPaket, string status){

    int jumlah = 0;

    for(int i = 0; i < jumlahPaket; i++){
        if(DaftarPaket[i].status == status){
            jumlah++;
        }
    }

    return jumlah;
}

/* =========================
   ANTRIAN + UPDATE STATUS
   ========================= */
void AntriandanUpdateStatus(){

    loadPaket();
    system("cls");

    string resiCari;

    int jumlahDiproses = 0;
    int jumlahDikirim = 0;
    int jumlahSelesai = 0;

    /* =========================
       HITUNG STATUS
       ========================= */
    for(int i = 0; i < jumlahPaket; i++){

        if(paket[i].status == "Diproses"){
            jumlahDiproses++;
        }
        else if(paket[i].status == "Dikirim"){
            jumlahDikirim++;
        }
        else if(paket[i].status == "Selesai"){
            jumlahSelesai++;
        }
    }

    /* =========================
       STATUS GUDANG
       ========================= */
    cout << "=========== STATUS GUDANG ===========\n\n";
    cout << "Diproses : " << jumlahDiproses << " paket\n";
    cout << "Dikirim  : " << jumlahDikirim << "/5 paket\n";
    cout << "Selesai  : " << jumlahSelesai << " paket\n";

    /* =========================
       TAMPIL SEMUA PAKET DARI DATABASE
       ========================= */
    cout << "\n=====================================\n";
    cout << "        SEMUA PAKET (DATABASE)\n";
    cout << "=====================================\n";

    for(int i = 0; i < jumlahPaket; i++){

        cout << "\nResi     : " << paket[i].resi << endl;
        cout << "Pengirim : " << paket[i].namaPengirim << endl;
        cout << "Penerima  : " << paket[i].namaPenerima << endl;
        cout << "Status   : " << paket[i].status << endl;
        cout << "-------------------------------------";
    }

    /* =========================
       INPUT RESI
       ========================= */
    cout << "\n\nMasukkan resi paket : ";
    cin >> resiCari;

    /* =========================
       CARI & UPDATE
       ========================= */
    for(int i = 0; i < jumlahPaket; i++){

        if(paket[i].resi == resiCari){

            /* ===== DIPROSES -> DIKIRIM ===== */
            if(paket[i].status == "Diproses"){

                if(jumlahDikirim >= 5){

                    cout << "\n====================================\n";
                    cout << "ANTRIAN PENGIRIMAN PENUH!\n";
                    cout << "====================================\n";

                    tekanEnter();
                    return;
                }

                paket[i].status = "Dikirim";
                savePaket();

                cout << "\nStatus berhasil diubah menjadi DIKIRIM\n";

                tekanEnter();
                return;
            }

            /* ===== DIKIRIM -> SELESAI ===== */
            else if(paket[i].status == "Dikirim"){

                paket[i].status = "Selesai";
                savePaket();

                cout << "\nStatus berhasil diubah menjadi SELESAI\n";

                tekanEnter();
                return;
            }

            /* ===== STATUS TIDAK VALID ===== */
            else{
                cout << "\nPaket tidak bisa diupdate dari status ini.\n";
                tekanEnter();
                return;
            }
        }
    }

    cout << "\nResi tidak ditemukan!\n";
    tekanEnter();
}