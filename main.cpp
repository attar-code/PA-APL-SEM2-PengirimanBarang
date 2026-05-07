#include<iostream>
#include "user.h"
#include "admin.h"
#include "data.h"

using namespace std;

int JumlahUser = 0;
User user[100];

Paket paket[MAX_PAKET];
int jumlahPaket = 0;

string userAktif = "";

int inputAngka(string pesan){
    int x;
    while(true){
        try{
            cout << pesan;
            cin >> x;

            if(cin.fail()){
                throw "input harus angka!";
            }

            cin.ignore(1000, '\n');
            return x;
        }
        catch(const char* msg){ 
            cout << "ERROR: " << msg << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

void tekanEnter() {
    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();
}

int main(){

    int utama;

    do{
        system("cls");

        cout << "=== SISTEM PENGIRIMAN BARANG ===\n";
        cout << "1. User\n";
        cout << "2. Admin\n";
        cout << "3. Lacak Paket\n";
        cout << "4. Keluar\n";

        utama = inputAngka("Pilih menu : ");

        switch (utama){

            case 1:{
                MenuUser();
                break;
            }

            case 2:{
                MenuAdmin();
                break;
            }

            case 3:{
                cout << "\n=== LACAK PAKET ===\n";
                cout << "Fitur lacak paket belum tersedia.\n";

                tekanEnter();
                break;
            }

            case 4:{
                cout << "\nTerima kasih telah menggunakan program.\n";
                break;
            }

            default:{
                cout << "\nMenu tidak valid!\n";
                tekanEnter();   
            }
        }

    } while(utama != 4);

    return 0;
}