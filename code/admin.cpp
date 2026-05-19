// #include<iostream> 
// #include "../include/admin.h"
// #include "../include/data.h"

// using namespace std;

// int menuNavigasi(string menu[], int jumlahMenu, string judul);

// string username, password;
// bool LoginAdmin(string namaadmin, string passwordadmin){
//     int login = 0;

//     while(login < 3){
//         cout << "===============================" << endl;
//         cout << " LOGIN ADMIN (MAKSIMAL 3 KALI) " << endl;
//         cout << "===============================" << endl;
//         cout << "Sisa kesempatan : " << 3 - login << endl;

//         cout << "Username : ";
//         getline(cin, username);

//         if(username.empty()){
//             cout << "Username tidak boleh kosong!\n";
//             tekanEnter();
//             system("cls");
//             continue;
//         }
        
//         cout << "Password : ";
//         getline(cin, password);
        
//         if(password.empty()){
//             cout << "Password tidak boleh kosong!\n";
//             tekanEnter();
//             system("cls");
//             continue;
//         }

//         if(username == namaadmin && password == passwordadmin){
//             cout << endl << "==============================" << endl;
//             cout << " BERHASIL LOGIN SEBAGAI ADMIN " << endl;
//             cout << "==============================" << endl;
//             tekanEnter();
//             return true;
//         }

//         login++;

//         cout << "USERNAME ATAU PASSWORD SALAH" << endl;
//         tekanEnter();
//         system("cls");
//     }

//     cout << "\nANDA TELAH MELEBIHI BATAS LOGIN!\n";
//     tekanEnter();

//     return false;
// }

// void MenuAdmin(){
//     system("cls");
//     int admin;
//     if(LoginAdmin("admin","123")){
//         system("cls");
//         do{
//             string menu[] = {
//                 "Tambah Paket",
//                 "Lihat Riwayat Paket",
//                 "Lihat Antrian dan Status Paket",
//                 "Hapus Riwayat Paket",
//                 "Membatalkan Pengiriman",
//                 "keluar"
//             };
//             admin = menuNavigasi(menu,6,"Menu Admin");

//             switch (admin){
//                 case 0: {
//                     TambahPaketAdmin();
//                     break;
//                     system("cls");
//                 }

//                 case 1: {
//                     tampilRiwayatPaket();
//                     tekanEnter();
//                     system("cls");
//                     break;
//                 }

//                 case 2: {
//                     AntriandanUpdateStatus();
//                     system("cls");
//                     break;
//                 }
                
//                 case 3: {
//                     HapusRiwayatPaket_Admin();
//                     system("cls");
//                     break;
//                 }
                
//                 case 4: {
//                     BatalkanPaket_Admin();
//                     system("cls");
//                     break;
//                 }
                
//                 case 5: {
//                     system("cls");
//                     cout << "LOGOUT DARI ADMIN" << endl;
//                     tekanEnter();
//                     system("cls");
//                     return;
//                 }
//             }
//         }while(true);
//     }
// }
#include <iostream> 
#include <string>
#include "../include/admin.h"
#include "../include/data.h"

// 🌟 Standarisasi Kode Warna ANSI agar Tampilan Mewah
#define RESET   "\033[0m"
#define MERAH   "\033[31m"
#define HIJAU   "\033[32m"
#define KUNING  "\033[33m"
#define BIRU    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

using namespace std;

int menuNavigasi(string menu[], int jumlahMenu, string judul);

string username, password;

bool LoginAdmin(string namaadmin, string passwordadmin){
    int login = 0;

    while(login < 3){
        bersihkanLayar(); // 👍 Ganti system("cls") biar ga kedip/bertelur di Mac
        
        // 🌟 Tampilan Box Login yang Lebih Menarik
        cout << CYAN << BOLD << "==================================================" << RESET << endl;
        cout << BOLD << "                  PORTAL LOGIN ADMIN              " << RESET << endl;
        cout << CYAN << BOLD << "==================================================" << RESET << endl;
        cout << KUNING << " Sisa Kesempatan Login : " << BOLD << 3 - login << RESET << "\n\n";

        cout << " Username : ";
        getline(cin, username);

        if(username.empty()){
            cout << MERAH << BOLD << "\n [ERROR] Username tidak boleh kosong!" << RESET << endl;
            tekanEnter();
            continue;
        }
        
        cout << " Password : ";
        getline(cin, password);
        
        if(password.empty()){
            cout << MERAH << BOLD << "\n [ERROR] Password tidak boleh kosong!" << RESET << endl;
            tekanEnter();
            continue;
        }

        if(username == namaadmin && password == passwordadmin){
            bersihkanLayar();
            cout << HIJAU << BOLD << "==================================================" << RESET << endl;
            cout << HIJAU << BOLD << "            BERHASIL LOGIN SEBAGAI ADMIN          " << RESET << endl;
            cout << HIJAU << BOLD << "==================================================" << RESET << endl;
            cout << "Selamat bertugas, admin!\n\n";
            tekanEnter();
            return true;
        }

        login++;
        cout << MERAH << BOLD << "\n [GAGAL] USERNAME ATAU PASSWORD SALAH!" << RESET << endl;
        tekanEnter();
    }

    bersihkanLayar();
    cout << MERAH << BOLD << "==================================================" << RESET << endl;
    cout << MERAH << BOLD << "          ANDA TELAH MELEBIHI BATAS LOGIN!        " << RESET << endl;
    cout << MERAH << BOLD << "==================================================" << RESET << endl;
    cout << "Akses ditolak sementara waktu.\n" << endl;
    tekanEnter();

    return false;
}

void MenuAdmin(){
    bersihkanLayar();
    int admin;
    
    if(LoginAdmin("admin","123")){
        do{
            bersihkanLayar();
            
            // Daftar menu utama admin
            string menu[] = {
                "Tambah Paket Baru",
                "Lihat Semua Riwayat Paket",
                "Lihat Antrean & Update Status",
                "Hapus Riwayat Paket",
                "Batalkan Pengiriman Paket",
                "Logout dari Sistem Admin"
            };
            
            // 🌟 Memanggil menuNavigasi bawaanmu dengan judul bercorak Kuning Bold
            admin = menuNavigasi(menu, 6, "SISTEM MANAJEMEN GUDANG EKSPEDISI (ADMIN)");

            switch (admin){
                case 0: { // 👍 Menyesuaikan indeks return menuNavigasi biasanya dimulai dari 1 (atau ganti ke 0 jika sistemmu mulai dari 0)
                    TambahPaketAdmin();
                    break;
                }

                case 1: {
                    bersihkanLayar();
                    tampilRiwayatPaket();
                    tekanEnter();
                    break;
                }

                case 2: {
                    AntriandanUpdateStatus(); // Modul update status yang baru kita buat
                    break;
                }
                
                case 3: {
                    HapusRiwayatPaket_Admin(); // Modul hapus riwayat yang baru kita perbaiki
                    break;
                }
                
                case 4: {
                    BatalkanPaket_Admin(); // Modul pembatalan paket yang super simpel kemarin
                    break;
                }
                
                case 5: {
                    bersihkanLayar();
                    cout << KUNING << BOLD << "==================================================" << RESET << endl;
                    cout << KUNING << BOLD << "                LOGOUT BERHASIL                   " << RESET << endl;
                    cout << KUNING << BOLD << "==================================================" << RESET << endl;
                    cout << "Anda telah keluar dari sesi admin secara aman.\n" << endl;
                    tekanEnter();
                    bersihkanLayar();
                    return; // Keluar dari fungsi MenuAdmin
                }
            }
        } while(true);
    }
}