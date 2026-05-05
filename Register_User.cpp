#include<iostream>
#include "data.h"
extern int JumlahUser;
extern User user[100];

using namespace std;


void RegisterUser(){
    cout << endl << "SILAHKAN DAFTAR" << endl;

    cout << "Nama : ";
    getline(cin, user[JumlahUser].username);

    cout << "Password : ";
    cin >> user[JumlahUser].password;
    cin.ignore(1000, '\n');

    JumlahUser++;
}