#include <iostream>
#include <string>
#include <fstream>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern int JumlahUser;
extern User user[100];
extern string userAktif;

bool LoginUser() {

    ifstream inputFile("database/Users.json");
    json data;

    if (inputFile.peek() == ifstream::traits_type::eof()) {
            data = json::array();
        }

        else {
            inputFile >> data;
        }

        inputFile.close();

    if (data.empty()) {
        cout << "\nBelum ada user yang terdaftar!\n";
        cout << "Silakan register terlebih dahulu.\n";

        tekanEnter();
        return false;
    }

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
            cout << "Selamat datang, " << inputUsername << "!\n";

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
    cout << "Kembali ke menu utama...\n";

    tekanEnter();

    return false;
}