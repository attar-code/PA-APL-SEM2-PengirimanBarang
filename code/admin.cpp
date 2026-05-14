#include<iostream> 
#include "../include/admin.h"
#include "../include/data.h"

using namespace std;

string username, password;
bool LoginAdmin(string namaadmin, string passwordadmin){
    int login = 0;

    while(login < 3){
        cout << "===============================" << endl;
        cout << " LOGIN ADMIN (MAKSIMAL 3 KALI) " << endl;
        cout << "===============================" << endl;
        cout << "Sisa kesempatan : " << 3 - login << endl;

        cout << "Username : ";
        cin >> username;

        cout << "Password : ";
        cin >> password;

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
            system("cls");
            cout << "===================================" << endl;
            cout << "            Menu Admin             " << endl;
            cout << "-----------------------------------" << endl;
            cout << " 1. Tambah Paket                   " << endl;
            cout << " 2. Lihat Riwayat Paket            " << endl;
            cout << " 3. Lihat Antrian dan Status Paket " << endl;
            cout << " 4. Hapus Riwayat Paket            " << endl;
            cout << " 5. Membatalkan Pengiriman         " << endl;
            cout << " 6. Keluar                         " << endl;
            cout << "===================================" << endl;
            cout << "INPUT ANGKA 1-6 : ";
            cin >> admin;

            switch (admin){
                case 1: {
                    TambahPaketAdmin();
                    break;
                    system("cls");
                }

                case 2: {
                    tampilRiwayatPaket();
                    tekanEnter();
                    system("cls");
                    break;
                }

                case 3: {
                    AntriandanUpdateStatus();
                    system("cls");
                    break;
                }
                
                case 4: {
                    HapusRiwayatPaket_Admin();
                    system("cls");
                    break;
                }
                
                case 5: {
                    BatalkanPaket_Admin();
                    system("cls");
                    break;
                }
                
                case 6: {
                    system("cls");
                    cout << "LOGOUT DARI ADMIN" << endl;
                    tekanEnter();
                    system("cls");
                    break;
                } 

            }
        }while(admin != 6);
    }else{
        cout << "salah";
    }
}
