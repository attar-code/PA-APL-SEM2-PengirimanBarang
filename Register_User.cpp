#include <iostream>
#include <string>
#include <fstream>
#include "data.h"
#include "database/json.hpp"

using json = nlohmann::json;
using namespace std;

extern int JumlahUser;
extern User user[100];

void saveUserToJson(User u);
void saveUserToJson(User u) {

    ifstream inputFile("database/Users.json");

    json data;

    if (inputFile.peek() == ifstream::traits_type::eof()) {
        data = json::array();
    }
    else {
        inputFile >> data;
    }

    inputFile.close();

    json userBaru = {
        {"username", u.username},
        {"password", u.password}
    };

    data.push_back(userBaru);

    ofstream outputFile("database/Users.json");

    outputFile << data.dump(4);

    outputFile.close();
}

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
    saveUserToJson(user[JumlahUser - 1]);

    cout << "\nRegistrasi berhasil!\n";
    cout << "Silakan login.\n";

    tekanEnter();
}