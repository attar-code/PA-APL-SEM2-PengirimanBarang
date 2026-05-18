#include<iostream>
#include "../include/user.h"
#include "../include/admin.h"
#include "../include/data.h"
#include "../include/LacakPaket.h"
#include "../include/cekResi.h"

#ifdef _WIN32
    #include <conio.h>
    #define CLEAR "cls"
#else
    #include <termios.h>
    #include <unistd.h>
    #define CLEAR "clear"

    int _getch() {
        struct termios oldt, newt;
        int ch;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;

        newt.c_lflag &= ~(ICANON | ECHO);

        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return ch;
    }
#endif


using namespace std;
int JumlahUser = 0;
User user[100];
Paket paket[100];
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
            
            return -1;
        }
    }
}

int menuNavigasi(string menu[], int jumlahMenu, string judul){

    int posisi = 0;
    int lebar = 35;

    while(true){

        system(CLEAR);

        cout << string(lebar, '=') << endl;
        int spasi = (lebar - judul.length()) / 2;
        cout << string(spasi, ' ') << judul << endl;
        cout << string(lebar, '=') << endl;
        
        for(int i=0;i<jumlahMenu;i++){
            
            if(i == posisi){
                cout << "> " << menu[i] << endl;
            }
            else{
                cout << "  " << menu[i] << endl;
            }
        }
        
        char tombol = _getch();
        
        if(tombol == 72){ // panah atas
            posisi--;
            
            if(posisi < 0)
            posisi = jumlahMenu - 1;
        }
        
        else if(tombol == 80){ // panah bawah
            posisi++;

            if(posisi >= jumlahMenu)
            posisi = 0;
        }
        
        else if(tombol == 13){ // enter
            return posisi;
        }
    }
}

int main(){
    string menu[] = {
        "User",
        "Admin",
        "Cek Resi",
        "keluar"
    };
    
    do{
        int utama = menuNavigasi(menu,4,"Menu Utama");
        
        switch(utama){
            case 0:{
                MenuUser();
                break;
            }
            
            case 1:{
                MenuAdmin();
                break;
            }
            
            case 2:{
                cekResi();
                break;
            }
            
            case 3:{
                system(CLEAR);
                cout << "=========================================" << endl;
                cout << " Terima Kasih Telah Menggunakan Program. " << endl;
                cout << "=========================================" << endl;
                return 0;
            }
        }
    }while(true);
}

// int main(){

//     int utama;

//     do{
//         system("cls");

//         cout << "=== SISTEM PENGIRIMAN BARANG ===\n";
//         cout << "1. User\n";
//         cout << "2. Admin\n";
//         cout << "3. Cek Resi\n";
//         cout << "4. Keluar\n";

//         utama = inputAngka("Pilih menu : ");

//         switch (utama){

//             case 1:{
//                 MenuUser();
//                 break;
//             }

//             case 2:{
//                 MenuAdmin();
//                 break;
//             }

//             case 3:{
//                 cekResi();
//                 break;
//             }

//             case 4:{
//                 cout << "\nTerima kasih telah menggunakan program.\n";
//                 break;
//             }

//             default:{
//                 cout << "\nMenu tidak valid!\n";
//                 tekanEnter();   
//             }   
//         }

//     } while(utama == -1 || utama != 4);

//     return 0;
// }