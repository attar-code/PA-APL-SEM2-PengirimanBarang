#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

//WARNA TERMINAL
#define RESET   "\033[0m"
#define MERAH   "\033[31m"
#define HIJAU   "\033[32m"
#define KUNING  "\033[33m"
#define CYAN    "\033[36m"
#define PUTIH   "\033[37m"
#define BOLD    "\033[1m"

extern int JumlahUser;
extern User user[100];
extern string userAktif;

bool LoginUser() {

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

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

        cout << MERAH;
        cout << "\nBelum ada user yang terdaftar!\n";
        cout << RESET;

        cout << "\nTekan ENTER untuk kembali...";
        cin.get();

        return false;
    }

    string inputUsername, inputPassword;

    int sisa = 3;

    while (sisa > 0) {

        system("cls");

        cout << CYAN;
        cout << "====================================================\n";
        cout << RESET;

        cout << BOLD << PUTIH;
        cout << "                   LOGIN USER\n";
        cout << RESET;

        cout << CYAN;
        cout << "====================================================\n\n";
        cout << RESET;

        cout << KUNING;
        cout << "Sisa Kesempatan Login : " << sisa << endl;
        cout << RESET;

        cout << "\nUsername : ";
        getline(cin, inputUsername);

        if (inputUsername.empty()) {

            cout << MERAH;
            cout << "\nERROR: Username tidak boleh kosong!\n";
            cout << RESET;

            cout << "\nTekan ENTER untuk kembali...";
            cin.get();

            return false;
        }

        //INPUT PASSWORD
        cout << "Password : ";
        getline(cin, inputPassword);

        //jika password kosong → kembali ke menu
        if (inputPassword.empty()) {

            cout << MERAH;
            cout << "\nERROR: Password tidak boleh kosong!\n";
            cout << RESET;

            cout << "\nTekan ENTER untuk kembali...";
            cin.get();

            return false;
        }

        bool loginBerhasil = false;

        //SEARCHING + LINEAR SEARCH
        for (auto akun : data) {

            if (
                akun["username"] == inputUsername &&
                akun["password"] == inputPassword
            ) {

                //VARIABEL GLOBAL
                userAktif = inputUsername;

                loginBerhasil = true;

                break;
            }
        }

        //JIKA LOGIN BERHASIL
        if (loginBerhasil) {

            cout << HIJAU;
            cout << "\nLogin berhasil!\n";
            cout << RESET;

            cout << "Selamat datang, "
                 << inputUsername << "!\n";

            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            return true;
        }

        //JIKA LOGIN GAGAL
        else {

            sisa--;

            cout << MERAH;
            cout << "\nUsername atau password salah!\n";
            cout << RESET;

            cout << "Sisa percobaan login : "
                 << sisa << endl;

            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();
        }
    }

    //JIKA GAGAL LOGIN 3 KALI
    cout << MERAH;
    cout << "\nAnda gagal login 3 kali.\n";
    cout << RESET;

    cout << "Kembali ke menu utama...\n";

    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();

    return false;
}