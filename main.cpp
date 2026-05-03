#include<iostream>
#include "user.h"
#include "admin.h"

using namespace std;

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

main(){
    int utama;
    do{

        cout << endl << "Menu Utama" << endl;
        cout << "Menu Utama" << endl;
        utama = inputAngka("Pilih 1/2/3/4 :");

        switch (utama){
            case 1:{
                MenuUser();
            }

            case 2:{
                MenuAdmin();
            }
        }
    }while(utama != 1);
    return 0;
}