#include <iostream>
#include <string>
#include "data.h"

using namespace std;

extern int JumlahUser;
extern User user[100];

void RegisterUser() {
    string inputUsername, inputPassword;
    bool usernameAda;

    do {
        system("cls");

        usernameAda = false;

        cout << "\n=== REGISTRASI USER ===\n";

        cout << "Username : ";
        getline(cin, inputUsername);

        cout << "Password : ";
        getline(cin, inputPassword);

        for (int i = 0; i < JumlahUser; i++) {
            if (user[i].username == inputUsername) {
                usernameAda = true;
                break;
            }
        }

        if (usernameAda) {
            cout << "\nUsername sudah digunakan!\n";
            cout << "Silakan gunakan username lain.\n";

            tekanEnter();
        }

    } while (usernameAda);

    user[JumlahUser].username = inputUsername;
    user[JumlahUser].password = inputPassword;
    JumlahUser++;

    cout << "\nRegistrasi berhasil!\n";
    cout << "Silakan login.\n";

    tekanEnter();
}