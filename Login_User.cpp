#include <iostream>
#include <string>
#include "data.h"

using namespace std;

extern int JumlahUser;
extern User user[100];
extern string userAktif;

bool LoginUser() {
    string inputUsername, inputPassword;
    int sisa = 3;

    while (sisa > 0) {
        system("cls");

        cout << "\n=== LOGIN USER ===\n";

        cout << "Username : ";
        getline(cin, inputUsername);

        cout << "Password : ";
        getline(cin, inputPassword);

        bool loginBerhasil = false;

        for (int i = 0; i < JumlahUser; i++) {
            if (user[i].username == inputUsername &&
                user[i].password == inputPassword) {

                userAktif = inputUsername;

                loginBerhasil = true;
                break;
            }
        }

        if (loginBerhasil) {
            cout << "\nLogin berhasil!\n";
            cout << "Selamat datang, " << inputUsername << "!\n";

            tekanEnter();
            return true;

        } else {
            sisa--;

            cout << "\nUsername atau password salah!\n";
            cout << "Sisa percobaan : " << sisa << endl;

            tekanEnter();
        }
    }

    cout << "\nAnda gagal login 3 kali.\n";
    cout << "Kembali ke menu utama...\n";

    tekanEnter();

    return false;
}