#include<iostream> 
#include "admin.h"
#include "TambahPaket_Admin.h"


using namespace std;

string username, password;
bool LoginAdmin(string namaadmin, string passwordadmin){
    int login = 0;

    cout << "LOGIN ADMIN (MAKSIMAL 3 KALI)" << endl;

    while(login < 3){
        cout << "Username : ";
        cin >> username;

        cout << "Password : ";
        cin >> password;

        if(username == namaadmin && password == passwordadmin){
            cout << "BERHASIL MASUK" << endl;
            return true;
        }

        cout << "USERNAME ATAU PASSWORD SALAH" << endl;
        login++;
    }

    return false;
}

void MenuAdmin(){
    int admin;
    if(LoginAdmin("admin","123")){
        do{
            cout << "===========================" << endl;
            cout << "        Menu Admin         " << endl;
            cout << "---------------------------" << endl;
            cout << " 1. Tambah Paket           " << endl;
            cout << " 2. Lihat Riwayat Paket    " << endl;
            cout << " 3. Lihat Antrian Paket    " << endl;
            cout << " 4. Update Status          " << endl;
            cout << " 5. Hapus Riwayat Paket    " << endl;
            cout << " 6. Membatalkan Pengiriman " << endl;
            cout << " 7. Keluar                 x`" << endl;
            cout << "===========================" << endl;
            cin >> admin;


            switch (admin){
                case 1: {
                    TambahPaketAdmin();
                    break;
                }

                case 2: {
                    // LihatRiwayatAdmin();
                }
            }
        }while(admin != 3);
    }else{
        cout << "salah";
    }
}
