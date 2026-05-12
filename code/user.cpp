#include <iostream>
#include "../include/data.h"
#include "../include/user.h"

using namespace std;

void RegisterUser();
bool LoginUser();
void MenuUtamaUser();

int inputAngka(string pesan);

void MenuUser() {
 
    int pilih;

    do {
        system("cls");

        cout << "=== MENU USER ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Keluar\n";

        pilih = inputAngka("Pilih menu : ");

        switch (pilih) {

            case 1: {
                RegisterUser();
                break;
            }

            case 2: {
                bool berhasil = LoginUser();

                if (berhasil) {
                    MenuUtamaUser();
                }

                break;
            }

            case 3: {
                cout << "\nKembali ke menu utama...\n";
                tekanEnter();
                break;
            }

            default: {
                cout << "\nMenu tidak valid!\n";
                tekanEnter();
            }
        }

    } while (pilih != 3);
}

void MenuUtamaUser() {

    int pilih;

    do {
        system("cls");

        cout << "=== HALAMAN UTAMA USER ===\n";
        cout << "1. Tambah Paket\n";
        cout << "2. Lihat Riwayat Paket\n";
        cout << "3. Update Paket\n";
        cout << "4. Hapus Riwayat Paket\n";
        cout << "5. Batalkan Paket\n";
        cout << "6. Keluar\n";

        pilih = inputAngka("Pilih menu : ");

        switch (pilih) {

            case 1: {
                TambahPaket_User();
                break;
            }

            case 2: {
                LihatRiwayatPaket_User();
                break;
            }

            case 3: {
                UpdatePaket_User();
                break;
            }

            case 4: {
                HapusRiwayatPaket_User();
                break;
            }

            case 5: {
                BatalkanPaket_User();
                break;
            }

            case 6: {
                cout << "\nLogout berhasil...\n";
                tekanEnter();
                break;
            }

            default: {
                cout << "\nMenu tidak valid!\n";
                tekanEnter();
            }
        }

    } while (pilih != 6);
}