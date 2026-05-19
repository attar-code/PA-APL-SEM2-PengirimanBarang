#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include "../include/data.h"
#include "../database/json.hpp"

using json = nlohmann::json;
using namespace std;

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

    //VARIABEL LOKAL
    string inputUsername, inputPassword;

    bool usernameAda;

    while (true) {
        usernameAda = false;

        system("cls");

        cout << "\n=== REGISTRASI USER ===\n";
        cout << "Username : ";
        
        getline(cin, inputUsername);

        //jika username kosong, loop ke menu
        if (inputUsername.empty()) {
            cout << "\nUsername tidak boleh kosong!\n";

            cout << "\nTekan ENTER untuk kembali...";
            cin.get();

            return;
        }

        //jika username = admin
        if (inputUsername == "admin") {
            cout << "\nERROR: Username admin tidak boleh digunakan!\n";

            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            continue;
        }

        //validasi username
        if (!validHurufAngka(inputUsername)) {
            cout << "\nERROR: Username hanya boleh huruf dan angka!\n";

            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            continue;
        }

        cout << "Password : ";
        getline(cin, inputPassword);

        //jika password kosong, kembali ke menu
        if (inputPassword.empty()) {
            cout << "\nERROR: Password tidak boleh kosong!\n";

            cout << "\nTekan ENTER untuk kembali...";
            cin.get();

            return;
        }

        //validasi password
        if (!validHurufAngka(inputPassword)) {
            cout << "\nERROR: Password hanya boleh huruf dan angka!\n";

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
            cout << "\nERROR: Username sudah digunakan!\n";
            cout << "Silakan gunakan username lain.\n";

            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();

            continue;
        }

        //jika semua valid, keluar dari loop
        break;
    }

    //ARRAY + STRUCT
    user[JumlahUser].username = inputUsername;
    user[JumlahUser].password = inputPassword;

    JumlahUser++;

    //simpan ke database
    saveUserToJson(user[JumlahUser - 1]);

    cout << "\nRegistrasi berhasil!\n";
    cout << "Silakan login.\n";

    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();
}