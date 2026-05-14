#include <iostream>
#include "../include/data.h"
#include "../include/admin.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

bool isDiproses(string status) {
    return (
        status == "Diproses" ||
        status == "Diproses (COD)" ||
        status == "Diproses (Lunas)"
    );
}

bool isAktif(string status) {
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

    // HITUNG STATUS
    for (int i = 0; i < jumlahPaket; i++) {

        if (paket[i].status == "Menunggu Validasi Admin") {
            menunggu++;
        }
        else if (isDiproses(paket[i].status)) {
            diproses++;
        }
        else if (paket[i].status == "Dikirim") {
            dikirim++;
        }
        else if (paket[i].status == "Selesai") {
            selesai++;
        }
    }

    cout << "=========== STATUS GUDANG ===========\n\n";
    cout << "Menunggu Validasi Admin : " << menunggu << " paket\n";
    cout << "Diproses                : " << diproses << " paket\n";
    cout << "Dikirim                 : " << dikirim << " paket\n";
    cout << "Selesai                 : " << selesai << " paket\n";

    cout << "\n=====================================\n";
    cout << "        SEMUA PAKET\n";
    cout << "=====================================\n";

    for (int i = 0; i < jumlahPaket; i++) {
        cout << "\nResi       : " << paket[i].resi << endl;
        cout << "Pengirim   : " << paket[i].namaPengirim << endl;
        cout << "Penerima   : " << paket[i].namaPenerima << endl;
        cout << "Pembayaran : " << paket[i].pembayaran << endl;
        cout << "Status     : " << paket[i].status << endl;
        cout << "-------------------------------------";
    }

    // DAFTAR AKTIF
    cout << "\n=====================================\n";
    cout << "        PAKET AKTIF\n";
    cout << "=====================================\n";

    bool ada = false;

    for (int i = 0; i < jumlahPaket; i++) {

        if (isAktif(paket[i].status)) {

            ada = true;

            cout << "\nResi     : " << paket[i].resi << endl;
            cout << "Pengirim : " << paket[i].namaPengirim << endl;
            cout << "Status   : " << paket[i].status << endl;
        }
    }

    if (!ada) {
        cout << "\nTidak ada paket aktif.\n";
        tekanEnter();
        return;
    }

    // INPUT RESI
    cout << "\nMasukkan resi paket: ";
    cin >> resiCari;

    for (int i = 0; i < jumlahPaket; i++) {

        if (paket[i].resi == resiCari) {

            string statusLama = paket[i].status;

            // ===============================
            // 1. VALIDASI ADMIN (TRANSFER)
            // ===============================
            if (paket[i].status == "Menunggu Validasi Admin") {

                if (paket[i].pembayaran == "Transfer") {

                    paket[i].status = "Diproses";

                    // RESI BARU DI GENERATE DI SINI
                    paket[i].resi = generateResi();

                    cout << "\nVALIDASI BERHASIL → DIPROSES\n";
                    cout << "Resi baru: " << paket[i].resi << endl;
                }
                else {
                    cout << "\nCOD tidak perlu validasi admin.\n";
                }
            }

            // ===============================
            // 2. DIPROSES → DIKIRIM
            // ===============================
            else if (isDiproses(paket[i].status)) {

                paket[i].status = "Dikirim";

                cout << "\nStatus berubah: DIPROSES → DIKIRIM\n";
            }

            // ===============================
            // 3. DIKIRIM → SELESAI
            // ===============================
            else if (paket[i].status == "Dikirim") {

                paket[i].status = "Selesai";

                cout << "\nStatus berubah: DIKIRIM → SELESAI\n";
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