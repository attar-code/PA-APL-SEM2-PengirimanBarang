#include<iostream>
#include "../include/user.h"
#include "../include/admin.h"
#include "../include/data.h"
#include "../include/LacakPaket.h"
#include "../include/cekResi.h"

#define RESET "\033[0m"
#define MERAH "\033[31m"
#define HIJAU "\033[32m"
#define KUNING "\033[33m"
#define BIRU "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"


#ifdef _WIN32
    // Jika dikompilasi di Windows, langsung pakai library bawaan Windows
    #include <conio.h> 
#else
    // Jika di Mac / Linux, gunakan fungsi custom buatanmu
    #include <termios.h>
    #include <unistd.h>

    inline int _getch() {
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

    while(true){ // <-- Kurung kurawal buka yang sempat hilang sudah aman di sini

        bersihkanLayar();

        // Kasih warna KUNING & BOLD di pembatas atas
        cout << KUNING << BOLD << string(lebar, '=') << RESET << endl;
        int spasi = (lebar - judul.length()) / 2;
        if (spasi > 0) cout << string(spasi, ' ');
        cout << BOLD << judul << RESET << endl; // Kasih BOLD di judul
        cout << KUNING << BOLD << string(lebar, '=') << RESET << endl;
        
        for(int i=0;i<jumlahMenu;i++){
            
            if(i == posisi){
                // Sesuaikan gaya penanda dengan menu tambah paket: BIRU, BOLD, dan > [ teks ] <
                cout << BIRU << BOLD << "  > [ " << menu[i] << " ] <" << RESET << endl;
            }
            else{
                cout << "      " << menu[i] << endl; // Sesuaikan spasi agar sejajar
            }
        }

        // Tambahkan info petunjuk di bawah menu
        cout << KUNING << string(lebar, '-') << RESET << endl;
        cout << "Gunakan Panah & Enter" << endl;
        
        int tombol = _getch(); // Ubah ke int agar aman menampung kode tombol Mac
        
        // --- FILTER PANAH TAMBAHAN UNTUK MACBOOK & WINDOWS ---
        if (tombol == 27) { // Sinyal escape key khusus di Mac/Linux
            _getch(); 
            tombol = _getch();
            if (tombol == 65) tombol = 72;      // Petakan Panah Atas Mac ke kode Windows
            else if (tombol == 66) tombol = 80; // Petakan Panah Bawah Mac ke kode Windows
        }
        else if (tombol == 224) { // Sinyal tombol extend khusus di Windows
            tombol = _getch(); 
        }
        // -----------------------------------------------------
        
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
        
        else if(tombol == 13 || tombol == 10){ // enter (13 Windows, 10 Mac)
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
                bersihkanLayar();
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