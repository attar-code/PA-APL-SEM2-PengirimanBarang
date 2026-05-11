#include <iostream>
#include "admin.h"
#include "data.h"
#include "TambahPaket_Admin.h"
#include "database/json.hpp"

using json = nlohmann::json;

using namespace std;

int HitungStatus(Paket DaftarPaket[], int jumlahPaket, string status){

    int jumlah = 0;

    for(int i = 0; i < jumlahPaket; i++){

        if(paket[i].status == status){
            jumlah++;
        }
    }

    return jumlah;
}

void AntriandanUpdateStatus(){
    loadPaket();
    system("cls");

    string resiCari;

    int jumlahDikonfirmasi= 0;
    int jumlahDiproses = 0;
    int jumlahDikirim = 0;
    int jumlahSelesai = 0;

    for(int i = 0; i < jumlahPaket; i++){

        if(paket[i].status == "Menunggu Validasi Admin"){
            jumlahDikonfirmasi++;
        }

        else if(paket[i].status == "Diproses" || "Diproses (COD)" || "Diproses (Lunas)"){
            jumlahDiproses++;
        }

        else if(paket[i].status == "Dikirim"){
            jumlahDikirim++;
        }

        else if(paket[i].status == "Selesai"){
            jumlahSelesai++;
        }
    }

    cout << "=========== STATUS GUDANG ===========\n\n";

    cout << "Menunggu Validasi Admin : " << jumlahDikonfirmasi << " paket\n";
    cout << "Diproses                : " << jumlahDiproses << " paket\n";
    cout << "Dikirim                 : " << jumlahDikirim << "/5 paket\n";
    cout << "Selesai                 : " << jumlahSelesai << " paket\n";

    cout << "\n=====================================\n";
    cout << "        SEMUA PAKET (DATABASE)\n";
    cout << "=====================================\n";

    for (int i = 0; i < jumlahPaket; i++) {

        cout << "\nResi       : " << paket[i].resi << endl;
        cout << "Pengirim   : " << paket[i].namaPengirim << endl;
        cout << "Penerima   : " << paket[i].namaPenerima << endl;
        cout << "Pembayaran : " << paket[i].pembayaran << endl;
        cout << "Status     : " << paket[i].status << endl;
        cout << "-------------------------------------";
    }

    // TAMPILKAN PAKET AKTIF
    cout << "\n=====================================\n";
    cout << "        DAFTAR PAKET AKTIF\n";
    cout << "=====================================\n";

    bool adaPaket = false;

    for(int i = 0; i < jumlahPaket; i++){

        if(
            paket[i].status == "Menunggu Validasi Admin" ||
            "Diproses" || "Diproses (COD)" || 
            "Diproses (Lunas)" || "Dikirim"
        ){

            adaPaket = true;

            cout << "\nResi     : " << paket[i].resi << endl;
            cout << "Pengirim : " << paket[i].namaPengirim << endl;
            cout << "Status   : " << paket[i].status << endl;
        }
    }

    if(!adaPaket){
        cout << "\nTidak ada paket aktif.\n";
        tekanEnter();
        return;
    }

    cout << "\nMasukkan resi paket : ";
    cin >> resiCari;

    for(int i = 0; i < jumlahPaket; i++) {

        if(paket[i].resi == resiCari) {

            string statusLama = paket[i].status;

            // vaidasi admin ke diproses
            if(paket[i].status == "Menunggu Validasi Admin") {

                string resiBaru;
                resiBaru = generateResi();
                paket[i].resi = resiBaru;

                paket[i].status = "Diproses";

                cout << "\nStatus berubah: MENUNGGU → DIPROSES\n";
                jumlahDiproses++;
            }

            // proses ke dikirim
            else if(paket[i].status == "Diproses" || "Diproses (COD)" || "Diproses (Lunas)") {

                if(jumlahDikirim >= 10) {
                    cout << "\nANTRIAN PENUH!\n";
                    tekanEnter();
                    return;
                }

                paket[i].status = "Dikirim";

                cout << "\nStatus berubah: DIPROSES → DIKIRIM\n";
                jumlahDiproses--;
                jumlahDikirim++;
            }

            // dikirim ke selesai
            else if(paket[i].status == "Dikirim") {

                paket[i].status = "Selesai";

                cout << "\nStatus berubah: DIKIRIM → SELESAI\n";
                jumlahDikirim--;
                jumlahSelesai++;
            }

            else {
                cout << "\nStatus tidak bisa diubah lagi!\n";
                tekanEnter();
                return;
            }

            savePaket();

            cout << "Update berhasil untuk resi: " << resiCari << endl;

            tekanEnter();
            return;
        }
    }
    cout << "\nResi tidak ditemukan!\n";
    tekanEnter();
}