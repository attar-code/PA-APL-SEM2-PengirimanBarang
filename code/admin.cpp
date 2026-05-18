#include<iostream> 
#include "../include/admin.h"
#include "../include/data.h"

using namespace std;

int menuNavigasi(string menu[], int jumlahMenu, string judul);

string username, password;
bool LoginAdmin(string namaadmin, string passwordadmin){
    int login = 0;

    while(login < 3){
        cout << "===============================" << endl;
        cout << " LOGIN ADMIN (MAKSIMAL 3 KALI) " << endl;
        cout << "===============================" << endl;
        cout << "Sisa kesempatan : " << 3 - login << endl;

        cout << "Username : ";
        getline(cin, username);

        if(username.empty()){
            cout << "Username tidak boleh kosong!\n";
            tekanEnter();
            system("cls");
            continue;
        }
        
        cout << "Password : ";
        getline(cin, password);
        
        if(password.empty()){
            cout << "Password tidak boleh kosong!\n";
            tekanEnter();
            system("cls");
            continue;
        }

        if(username == namaadmin && password == passwordadmin){
            cout << endl << "==============================" << endl;
            cout << " BERHASIL LOGIN SEBAGAI ADMIN " << endl;
            cout << "==============================" << endl;
            tekanEnter();
            return true;
        }

        login++;

        cout << "USERNAME ATAU PASSWORD SALAH" << endl;
        tekanEnter();
        system("cls");
    }

    cout << "\nANDA TELAH MELEBIHI BATAS LOGIN!\n";
    tekanEnter();

    return false;
}

void MenuAdmin(){
    system("cls");
    int admin;
    if(LoginAdmin("admin","123")){
        system("cls");
        do{
            string menu[] = {
                "Tambah Paket",
                "Lihat Riwayat Paket",
                "Lihat Antrian dan Status Paket",
                "Hapus Riwayat Paket",
                "Membatalkan Pengiriman",
                "keluar"
            };
            admin = menuNavigasi(menu,6,"Menu Admin");

            switch (admin){
                case 0: {
                    TambahPaketAdmin();
                    break;
                    system("cls");
                }

                case 1: {
                    tampilRiwayatPaket();
                    tekanEnter();
                    system("cls");
                    break;
                }

                case 2: {
                    AntriandanUpdateStatus();
                    system("cls");
                    break;
                }
                
                case 3: {
                    HapusRiwayatPaket_Admin();
                    system("cls");
                    break;
                }
                
                case 4: {
                    BatalkanPaket_Admin();
                    system("cls");
                    break;
                }
                
                case 5: {
                    system("cls");
                    cout << "LOGOUT DARI ADMIN" << endl;
                    tekanEnter();
                    system("cls");
                    return;
                }
            }
        }while(true);
    }
}
