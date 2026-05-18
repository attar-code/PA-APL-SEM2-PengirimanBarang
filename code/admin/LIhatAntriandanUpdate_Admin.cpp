#include <iostream>
#include <cctype>
#include <fstream>
#include "../include/data.h"
#include "../include/admin.h"
#include "../database/json.hpp"

// Tambahkan makro warna agar konsisten dengan menu tambah paket
#define RESET "\033[0m"
#define MERAH "\033[31m"
#define HIJAU "\033[32m"
#define KUNING "\033[33m"
#define BIRU "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

using json = nlohmann::json;
using namespace std;

extern Paket paket[100];
extern int jumlahPaket;

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

void loadPaket(){

    ifstream file("database/paket.json");
    json j;
    file >> j;

    jumlahPaket = j.size();

    for(int i = 0; i < jumlahPaket; i++){

        paket[i].resi = j[i]["resi"];
        paket[i].status = j[i]["status"];
        paket[i].namaPengirim = j[i]["namaPengirim"];
        paket[i].namaPenerima = j[i]["namaPenerima"];
        paket[i].alamat = j[i]["alamat"];
        paket[i].berat = j[i]["berat"];
        paket[i].tipe = j[i]["tipe"];
        paket[i].pembayaran = j[i]["pembayaran"];
        paket[i].ongkir = j[i]["ongkir"];
        paket[i].pemilik = j[i]["pemilik"];
    }
}

void savePaket(){
 
    json j = json::array();

    for(int i = 0; i < jumlahPaket; i++){

        j.push_back({
            {"resi", paket[i].resi},
            {"namaPengirim", paket[i].namaPengirim},
            {"namaPenerima", paket[i].namaPenerima},
            {"alamat", paket[i].alamat},
            {"berat", paket[i].berat},
            {"tipe", paket[i].tipe},
            {"pembayaran", paket[i].pembayaran},
            {"ongkir", paket[i].ongkir},
            {"pemilik", paket[i].pemilik},
            {"status", paket[i].status}
        });
    }

    ofstream file("database/paket.json");
    file << j.dump(4);
    file.close();
}

void AntriandanUpdateStatus() {

    loadPaket();
    bubbleSortStatus();
    bersihkanLayar(); // Diganti ke fungsi bawaan data.h agar aman di Mac

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

    cout << KUNING << BOLD << "=========== STATUS GUDANG ===========" << RESET << "\n\n";
    cout << "Menunggu Validasi Admin : " << BOLD << menunggu << RESET << " paket\n";
    cout << "Diproses                : " << BOLD << diproses << RESET << " paket\n";
    cout << "Dikirim                 : " << BOLD << dikirim << RESET << " paket\n";
    cout << "Selesai                 : " << BOLD << selesai << RESET << " paket\n";

    cout << KUNING << BOLD << "=====================================" << RESET << endl;
    cout << BOLD << "            SEMUA PAKET              " << RESET << endl;
    cout << KUNING << BOLD << "=====================================" << RESET << "";

    for (int i = 0; i < jumlahPaket; i++) {
        cout << "\n" << CYAN << "Resi       : " << RESET << BOLD << (ptr + i)->resi << RESET << endl;
        cout << "Pengirim   : " << (ptr + i)->namaPengirim << endl;
        cout << "Penerima   : " << (ptr + i)->namaPenerima << endl;
        cout << "Pembayaran : " << (ptr + i)->pembayaran << endl;
        cout << "Status     : " << BIRU << BOLD << (ptr + i)->status << RESET << endl;
        cout << KUNING << "-------------------------------------" << RESET << endl;
    }

    // DAFTAR AKTIF
    cout << KUNING << BOLD << "=====================================" << RESET << endl;
    cout << BOLD << "            PAKET AKTIF              " << RESET << endl;
    cout << KUNING << BOLD << "=====================================" << RESET << endl;

    bool ada = false;

    for (int i = 0; i < jumlahPaket; i++) {

        if (paketaktif((ptr + i)->status)) {

            ada = true;

            cout << "\n" << CYAN << "Resi     : " << RESET << BOLD << (ptr + i)->resi << RESET << endl;
            cout << "Pengirim : " << (ptr + i)->namaPengirim << endl;
            cout << "Status   : " << HIJAU << BOLD << (ptr + i)->status << RESET << endl;
        }
    }

    if (!ada) {
        cout << "\n" << KUNING << "Tidak ada paket aktif." << RESET << "\n";
        tekanEnter();
        return;
    }

    // INPUT RESI

    cout << endl << MERAH << BOLD << "KETIK 'BATAL' UNTUK KELUAR" << RESET << endl;
    cout << BOLD << "Masukkan resi paket: " << RESET;
    cin >> resiCari;

    for(char &huruf : resiCari){
        huruf = toupper(huruf);
    }

    if (resiCari == "BATAL"){
        cout << KUNING << BOLD << "=====================================" << RESET << endl;
        cout << MERAH << BOLD << "         UPDATE STATUS BATAL         " << RESET << endl;
        cout << KUNING << BOLD << "=====================================" << RESET << endl;
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

                    /* untuk paket menunggu validasi admin belum mndpt resi
                        di sini baru dapat resi dengan syarat admin menginput "BELUM_RILIS" 
                        nanti dia akan menyesuaikan antrian (yang input paket dluan yang akan di
                        validasi admin dluan). setelah di vallidasi admin baru akan mendapat resi baru*/
                    (ptr + i)->resi = generateResiUnik();

                    cout << "\n" << HIJAU << BOLD << "VALIDASI BERHASIL -> DIPROSES" << RESET << "\n";
                    cout << CYAN << "Resi baru: " << RESET << BOLD << (ptr + i)->resi << RESET << endl;
                }
                else {
                    cout << "\n" << MERAH << "COD tidak perlu validasi admin." << RESET << "\n";
                }
            }

            // diproses -> dikirim
            else if (paketdiproses((ptr + i)->status)) {

                (ptr + i)->status = "Dikirim";

                cout << "\n" << HIJAU << BOLD << "Status berubah: DIPROSES -> DIKIRIM" << RESET << "\n";
            }

            /// dikirim -> selesai
            else if ((ptr + i)->status == "Dikirim") {

                (ptr + i)->status = "Selesai";

                cout << "\n" << HIJAU << BOLD << "Status berubah: DIKIRIM -> SELESAI" << RESET << "\n";
            }

            else {
                cout << "\n" << MERAH << BOLD << "Status tidak bisa diubah lagi!" << RESET << "\n";
                tekanEnter();
                return;
            }

            savePaket();

            cout << "\n" << HIJAU << BOLD << "Update berhasil untuk resi: " << RESET << BOLD << resiCari << RESET << endl;
            tekanEnter();
            return;
        }
    }

    cout << "\n" << MERAH << BOLD << "Resi tidak ditemukan!" << RESET << "\n";
    tekanEnter();
}