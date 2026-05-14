#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

//LIBRARY EXTERNAL
//Menggunakan library json.hpp untuk database JSON

//VARIABEL GLOBAL
extern int JumlahUser;
extern User user[100];

//FUNGSI VALIDASI
//Materi: Fungsi
bool validHurufAngka(string teks) {

    for (char c : teks) {

        //mengecek apakah karakter bukan huruf/angka
        if (!isalnum(c)) {
            return false;
        }
    }

    return true;
}

//PROSEDUR SIMPAN USER KE JSON
//Materi: Prosedur, Pass by Value
void saveUserToJson(User u) {

    ifstream inputFile("database/Users.json");

    json data;

    //jika file kosong
    if (inputFile.peek() == ifstream::traits_type::eof()) {
        data = json::array();
    }

    //jika ada isi
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

    //menyimpan kembali ke json
    ofstream outputFile("database/Users.json");
    outputFile << data.dump(4);
    outputFile.close();
}

//PROSEDUR REGISTER USER
//Materi: Prosedur
void RegisterUser() {

    //VARIABEL LOKAL
    string inputUsername, inputPassword;

    bool usernameAda;
    bool inputValid;

    do {
        usernameAda = false;
        inputValid = true;

        system("cls");

        cout << "\n=== REGISTRASI USER ===\n";
        //INPUT USERNAME
        cout << "Username : ";
        getline(cin, inputUsername);

        //USERNAME KOSONG
        if (inputUsername.empty()) {
            cout << "\nERROR: Username tidak boleh kosong!\n";
            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            inputValid = false;
            continue;
        }

        //USERNAME ADMIN
        if (inputUsername == "admin") {
            cout << "\nERROR: Username admin tidak boleh digunakan!\n";
            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            inputValid = false;
            continue;
        }

        //VALIDASI HURUF & ANGKA
        if (!validHurufAngka(inputUsername)) {
            cout << "\nERROR: Username hanya boleh huruf dan angka!\n";
            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            inputValid = false;
            continue;
        }

        //INPUT PASSWORD
        cout << "Password : ";
        getline(cin, inputPassword);

        //PASSWORD KOSONG
        if (inputPassword.empty()) {
            cout << "\nERROR: Password tidak boleh kosong!\n";
            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            inputValid = false;
            continue;
        }

        //PASSWORD HANYA HURUF & ANGKA
        if (!validHurufAngka(inputPassword)) {
            cout << "\nERROR: Password hanya boleh huruf dan angka!\n";
            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            inputValid = false;
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

        //LINEAR SEARCH
        for (auto akun : data) {

            //cek username apakah sudah ada
            if (akun["username"] == inputUsername) {
                usernameAda = true;
                break;
            }
        }

        //USERNAME SUDAH DIGUNAKAN
        if (usernameAda) {
            cout << "\nERROR: Username sudah digunakan!\n";
            cout << "Silakan gunakan username lain.\n";
            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            continue;
        }

    } while (usernameAda || !inputValid);

    //ARRAY + STRUCT
    user[JumlahUser].username = inputUsername;
    user[JumlahUser].password = inputPassword;

    JumlahUser++;

    //SIMPAN KE DATABASE JSON
    saveUserToJson(user[JumlahUser - 1]);

    //OUTPUT BERHASIL
    cout << "\nRegistrasi berhasil!\n";
    cout << "Silakan login.\n";

    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();
}