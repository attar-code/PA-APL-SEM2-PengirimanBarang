#include <iostream>
#include "../include/data.h"
#include "../include/user.h"

using namespace std;

int menuNavigasi(string menu[], int jumlahMenu, string judul);

void MenuUser() {

    string menu[] = {
        "Registrasi",
        "Login",
        "keluar"
    };

    do{
        int pilihUser1 = menuNavigasi(menu,3,"Menu User");

        switch (pilihUser1) {
            case 0: {
                RegisterUser();
                break;
            }

            case 1: {
                bool berhasil = LoginUser();
                if (berhasil) {
                    MenuUtamaUser();
                }
                break;
            }

            case 2: {
                cout << endl << "Kembali ke menu utama..." << endl;
                tekanEnter();
                return;
            }
        }
    }while(true);
}

void MenuUtamaUser() {
    string menu[] = {
        "Tambah Paket",
        "Lihat Riwayat Paket",
        "Update Paket",
        "Hapus Riwayat Paket",
        "Batalkan Paket",
        "keluar"
    };

    do{
        int pilihUser2 = menuNavigasi(menu,6,"Halaman Utama User");

        switch (pilihUser2) {

            case 0: {
                TambahPaket_User();
                break;
            }

            case 1: {
                LihatRiwayatPaket_User();
                break;
            }

            case 2: {
                UpdatePaket_User();
                break;
            }

            case 3: {
                HapusRiwayatPaket_User();
                break;
            }

            case 4: {
                BatalkanPaket_User();
                break;
            }

            case 5: {
                cout << endl << "Logout berhasil..." << endl;
                tekanEnter();
                return;
            }
        }
    }while(true);
}