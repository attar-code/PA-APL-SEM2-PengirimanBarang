#include <iostream>
#include <string>
#include <fstream>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

//VARIABEL GLOBAL
extern int JumlahUser;
extern User user[100];
extern string userAktif;

//materi: fungsi
bool LoginUser() {

    //FILE HANDLING
    ifstream inputFile("database/Users.json");
    json data;

    //ERROR HANDLING
    if (inputFile.peek() == ifstream::traits_type::eof()) {
        data = json::array();
    }

    else {
        inputFile >> data;
    }

    inputFile.close();

    //ERROR HANDLING
    if (data.empty()) {
        cout << "\nBelum ada user yang terdaftar!\n";

        tekanEnter();
        return false;
    }

    //VARIABEL LOKAL
    string inputUsername, inputPassword;
    int sisa = 3;

    while (sisa > 0) {
        system("cls");

        cout << "\n=== LOGIN USER ===\n";
        cout << "Username : ";
        getline(cin, inputUsername);

        if (inputUsername.empty()) {
            cout << "\nERROR: Username tidak boleh kosong!\n";

            tekanEnter();
            continue;
        }

        cout << "Password : ";
        getline(cin, inputPassword);

        if (inputPassword.empty()) {
            cout << "\nERROR: Password tidak boleh kosong!\n";

            tekanEnter();
            continue;
        }

        bool loginBerhasil = false;

        //SEARCHING & LINEAR SEARCH
        for (auto akun : data) {

            if (
                akun["username"] == inputUsername &&
                akun["password"] == inputPassword
            ) {

                userAktif = inputUsername;
                loginBerhasil = true;
                break;
            }
        }

        if (loginBerhasil) {
            cout << "\nLogin berhasil!\n";

            tekanEnter();
            return true;
        }

        else {
            sisa--;

            cout << "\nUsername atau password salah!\n";
            cout << "Sisa percobaan : " << sisa << endl;

            tekanEnter();
        }
    }

    cout << "\nAnda gagal login 3 kali.\n";
    tekanEnter();

    return false;
}