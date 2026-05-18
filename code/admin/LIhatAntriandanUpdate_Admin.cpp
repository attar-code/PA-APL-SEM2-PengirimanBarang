#include <iostream>
#include <cctype>
#include "../include/data.h"
#include "../include/admin.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

bool paketdiproses(string status) {
    return (
        status == "Diproses" ||
        status == "Diproses (COD)" ||
        status == "Diproses (Lunas)"
    );
}

bool paketaktif(string status) {
    return (
        status == "Menunggu Validasi Admin" ||
        status == "Diproses" ||
        status == "Diproses (COD)" ||
        status == "Diproses (Lunas)" ||
        status == "Dikirim"
    );
}

bool resiSudahAda(string resi){

    for(int i=0;i<jumlahPaket;i++){

        if(paket[i].resi==resi){

            return true;
        }
    }

    return false;
}

string generateResiUnik(){

    string resiBaru;

    do{

        resiBaru = generateResi();

    }while(resiSudahAda(resiBaru));

    return resiBaru;
}

int prioritasStatus(string status){

    if(status == "Menunggu Validasi Admin")
        return 1;

    else if(
        status == "Diproses" ||
        status == "Diproses (COD)" ||
        status == "Diproses (Lunas)")
        return 2;

    else if(status == "Dikirim")
        return 3;

    else if(status == "Selesai")
        return 4;

    else if(status == "Dibatalkan")
        return 5;

    return 999;
}


void bubbleSortStatus(){

    for(int i=0;i<jumlahPaket-1;i++){

        for(int j=0;
            j<jumlahPaket-i-1;
            j++)
        {

            if(
              prioritasStatus(
                paket[j].status
              )

              >

              prioritasStatus(
                paket[j+1].status
              )
            ){

                swap(
                    paket[j],
                    paket[j+1]
                );
            }

        }

    }

}

void AntriandanUpdateStatus() {

    loadPaket();
    bubbleSortStatus();
    system("cls");

    string resiCari;

    int menunggu = 0;
    int diproses = 0;
    int dikirim = 0;
    int selesai = 0;

    Paket *ptr = paket;

    // menghitung status yg ada
    for (int i = 0; i < jumlahPaket; i++) {

        if ((ptr + i)->status == "Menunggu Validasi Admin") {
            menunggu++;
        }
        else if (paketdiproses((ptr + i)->status)) {
            diproses++;
        }
        else if ((ptr + i)->status == "Dikirim") {
            dikirim++;
        }
        else if ((ptr + i)->status == "Selesai") {
            selesai++;
        }
    }

    cout << "=========== STATUS GUDANG ===========\n\n";
    cout << "Menunggu Validasi Admin : " << menunggu << " paket\n";
    cout << "Diproses                : " << diproses << " paket\n";
    cout << "Dikirim                 : " << dikirim << " paket\n";
    cout << "Selesai                 : " << selesai << " paket\n";

    cout << "=====================================" << endl;
    cout << "            SEMUA PAKET              " << endl;
    cout << "=====================================";

    for (int i = 0; i < jumlahPaket; i++) {
        cout << "\nResi       : " << (ptr + i)->resi << endl;
        cout << "Pengirim   : " << (ptr + i)->namaPengirim << endl;
        cout << "Penerima   : " << (ptr + i)->namaPenerima << endl;
        cout << "Pembayaran : " << (ptr + i)->pembayaran << endl;
        cout << "Status     : " << (ptr + i)->status << endl;
        cout << "-------------------------------------" << endl;
    }

    // DAFTAR AKTIF
    cout << "=====================================" << endl;
    cout << "            PAKET AKTIF              " << endl;
    cout << "=====================================" << endl;

    bool ada = false;

    for (int i = 0; i < jumlahPaket; i++) {

        if (paketaktif((ptr + i)->status)) {

            ada = true;

            cout << "\nResi     : " << (ptr + i)->resi << endl;
            cout << "Pengirim : " << (ptr + i)->namaPengirim << endl;
            cout << "Status   : " << (ptr + i)->status << endl;
        }
    }

    if (!ada) {
        cout << "\nTidak ada paket aktif.\n";
        tekanEnter();
        return;
    }

    // INPUT RESI

    cout << endl << "KETIK 'BATAL' UNTUK KELUAR" << endl;
    cout << "Masukkan resi paket: ";
    cin >> resiCari;

    for(char &huruf : resiCari){
        huruf = toupper(huruf);
    }

    if (resiCari == "BATAL"){
        cout << "=====================================" << endl;
        cout << "         UPDATE STATUS BATAL         " << endl;
        cout << "=====================================" << endl;
        tekanEnter();
        return;
    }

    for (int i = 0; i < jumlahPaket; i++) {
        if ((ptr + i)->resi == resiCari) {
            string statusLama = (ptr + i)->status;

            // validasi admin -> diproses
            if ((ptr + i)->status == "Menunggu Validasi Admin") {

                if ((ptr + i)->pembayaran == "Transfer") {

                    (ptr + i)->status = "Diproses";

                    /*  untuk paket menunggu validasi admin belum mndpt resi
                        di sini baru dapat resi dengan syarat admin menginput "BELUM_RILIS" 
                        nanti dia akan menyesuaikan antrian (yang input paket dluan yang akan di
                        validasi admin dluan). setelah di vallidasi admin baru akan mendapat resi baru*/
                    (ptr + i)->resi = generateResiUnik();

                    cout << "\nVALIDASI BERHASIL -> DIPROSES\n";
                    cout << "Resi baru: " << (ptr + i)->resi << endl;
                }
                else {
                    cout << "\nCOD tidak perlu validasi admin.\n";
                }
            }

            // ===============================
            // 2. DIPROSES → DIKIRIM
            // ===============================
            else if (paketdiproses((ptr + i)->status)) {

                (ptr + i)->status = "Dikirim";

                cout << "\nStatus berubah: DIPROSES -> DIKIRIM\n";
            }

            // ===============================
            // 3. DIKIRIM → SELESAI
            // ===============================
            else if ((ptr + i)->status == "Dikirim") {

                (ptr + i)->status = "Selesai";

                cout << "\nStatus berubah: DIKIRIM -> SELESAI\n";
            }

            else {
                cout << "\nStatus tidak bisa diubah lagi!\n";
                tekanEnter();
                return;
            }

            savePaket();

            cout << "\nUpdate berhasil untuk resi: " << resiCari << endl;
            tekanEnter();
            return;
        }
    }

    cout << "\nResi tidak ditemukan!\n";
    tekanEnter();
}