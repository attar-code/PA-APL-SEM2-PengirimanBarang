#include <iostream>
#include <string>
#include <fstream>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

//mengambil data global
extern int JumlahUser;
extern User user[100];

//FUNGSI SIMPAN USER KE JSON
void saveUserToJson(User u) {

    //membuka file Users.json
    ifstream inputFile("database/Users.json");

    json data;

    //jika file kosong → buat array kosong
    if (inputFile.peek() == ifstream::traits_type::eof()) {
        data = json::array();
    }

    //jika ada isi → ambil data lama
    else {
        inputFile >> data;
    }

    inputFile.close();

    //membuat object user baru
    json userBaru = {
        {"username", u.username},
        {"password", u.password}
    };

    //menambahkan user baru ke database
    data.push_back(userBaru);

    //menyimpan kembali ke file json
    ofstream outputFile("database/Users.json");

    outputFile << data.dump(4);

    outputFile.close();
}

//FUNGSI REGISTER USER
void RegisterUser() {

    string inputUsername, inputPassword;

    bool usernameAda;

    do {

        system("cls");

        usernameAda = false;

        cout << "\n=== REGISTRASI USER ===\n";

        //input username
        cout << "Username : ";
        getline(cin, inputUsername);

        //validasi username kosong
        if (inputUsername.empty()) {

            cout << "\nERROR: Username tidak boleh kosong!\n";

            tekanEnter();
            continue;
        }

        //username admin tidak boleh dipakai
        if (inputUsername == "admin") {

            cout << "\nERROR: Username admin tidak boleh digunakan!\n";

            tekanEnter();
            continue;
        }

        //input password
        cout << "Password : ";
        getline(cin, inputPassword);

        //validasi password kosong
        if (inputPassword.empty()) {

            cout << "\nERROR: Password tidak boleh kosong!\n";

            tekanEnter();
            continue;
        }

        //cek username di databse
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

        //mengecek apakah username sudah ada
        for (auto akun : data) {

            if (akun["username"] == inputUsername) {

                usernameAda = true;

                break;
            }
        }

        //jika username sudah dipakai
        if (usernameAda) {

            cout << "\nERROR: Username sudah digunakan!\n";
            cout << "Silakan gunakan username lain.\n";

            tekanEnter();
        }

    } while (usernameAda);

    //simpan ke array
    user[JumlahUser].username = inputUsername;
    user[JumlahUser].password = inputPassword;

    JumlahUser++;

    //simpan ke database
    saveUserToJson(user[JumlahUser - 1]);

    //output berhasil
    cout << "\nRegistrasi berhasil!\n";
    cout << "Silakan login.\n";

    tekanEnter();
}