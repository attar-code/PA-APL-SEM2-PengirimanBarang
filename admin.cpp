#include<iostream> 
#include "admin.h"

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
            cout << "Menu Admin" << endl;
            cin >> admin;

            switch (admin){
                case 1: {
                    cout << "cobain";

                }
            }
        }while(admin != 3);
    }else{
        cout << "salah";
    }
}
