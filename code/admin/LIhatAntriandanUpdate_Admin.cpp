#include <iostream>
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

void AntriandanUpdateStatus() {

    loadPaket();
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

            // validasi admin to diproses
            if ((ptr + i)->status == "Menunggu Validasi Admin") {

                if ((ptr + i)->pembayaran == "Transfer") {

                    (ptr + i)->status = "Diproses";

                    // RESI BARU DI GENERATE DI SINI
                    (ptr + i)->resi = generateResi();

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