#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
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

//VARIABEL GLOBAL
extern int JumlahUser;
extern User user[100];

//materi: fungsi
//FUNGSI VALIDASI
bool validHurufAngka(string teks) {

    for (char c : teks) {

        //mengecek apakah karakter bukan huruf/angka
        if (!isalnum(c)) {
            return false;
        }
    }

    return true;
}

//materi: prosedur, pass by value
//PROSEDUR SIMPAN USER KE JSON
void saveUserToJson(User u) {

    ifstream inputFile("database/Users.json");

    json data;

    //jika file kosong
    if (inputFile.peek() == ifstream::traits_type::eof()) {
        data = json::array();
    }

    //jika ada data
    else {
        inputFile >> data;
    }

    inputFile.close();

    //STRUCT
    json userBaru = {
        {"username", u.username},
        {"password", u.password}
    };

    //menambahkan user baru
    data.push_back(userBaru);

    //menyimpan kembali ke file json
    ofstream outputFile("database/Users.json");

    outputFile << data.dump(4);

    outputFile.close();
}

//materi: prosedur
//PROSEDUR REGISTER USER
void RegisterUser() {

    //AKTIFKAN ANSI COLOR WINDOWS
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    //VARIABEL LOKAL
    string inputUsername, inputPassword;

    bool usernameAda;

    while (true) {

        usernameAda = false;

        system("cls");

        cout << CYAN;
        cout << "====================================================\n";
        cout << RESET;

        cout << BOLD << PUTIH;
        cout << "                 REGISTER USER\n";
        cout << RESET;

        cout << CYAN;
        cout << "====================================================\n\n";
        cout << RESET;

        //INPUT USERNAME
        cout << KUNING << "Username : " << RESET;
        getline(cin, inputUsername);

        //jika username kosong
        if (inputUsername.empty()) {

            cout << MERAH;
            cout << "\nUsername tidak boleh kosong!\n";
            cout << RESET;

            cout << "\nTekan ENTER untuk kembali...";
            cin.get();

            return;
        }

        //jika username = admin
        if (inputUsername == "admin") {

            cout << MERAH;
            cout << "\nERROR: Username admin tidak boleh digunakan!\n";
            cout << RESET;

            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            continue;
        }

        //validasi username
        if (!validHurufAngka(inputUsername)) {

            cout << MERAH;
            cout << "\nERROR: Username hanya boleh huruf dan angka!\n";
            cout << RESET;

            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            continue;
        }

        //INPUT PASSWORD
        cout << KUNING << "Password : " << RESET;
        getline(cin, inputPassword);

        //jika password kosong
        if (inputPassword.empty()) {

            cout << MERAH;
            cout << "\nERROR: Password tidak boleh kosong!\n";
            cout << RESET;

            cout << "\nTekan ENTER untuk kembali...";
            cin.get();

            return;
        }

        //validasi password
        if (!validHurufAngka(inputPassword)) {

            cout << MERAH;
            cout << "\nERROR: Password hanya boleh huruf dan angka!\n";
            cout << RESET;

            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            continue;
        }

        //FILE HANDLING
        ifstream inputFile("database/Users.json");

        json data;

        //jika file kosong
        if (inputFile.peek() == ifstream::traits_type::eof()) {
            data = json::array();
        }

        //jika ada data
        else {
            inputFile >> data;
        }

        inputFile.close();

        //materi: linear search
        //cek username apakah sudah digunakan
        for (auto akun : data) {

            if (akun["username"] == inputUsername) {

                usernameAda = true;

                break;
            }
        }

        //jika username sudah ada
        if (usernameAda) {

            cout << MERAH;
            cout << "\nERROR: Username sudah digunakan!\n";
            cout << "Silakan gunakan username lain.\n";
            cout << RESET;

            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            continue;
        }

        //jika semua valid
        break;
    }

    //ARRAY + STRUCT
    user[JumlahUser].username = inputUsername;
    user[JumlahUser].password = inputPassword;

    JumlahUser++;

    //simpan ke database
    saveUserToJson(user[JumlahUser - 1]);

    cout << HIJAU;
    cout << "\nRegistrasi berhasil!\n";
    cout << RESET;

    cout << "Silakan login.\n";

    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();
}