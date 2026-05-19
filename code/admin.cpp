#include <iostream> 
#include <string>
#include "../include/admin.h"
#include "../include/data.h"

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
        bersihkanLayar();
        
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

            string menu[] = {
                "Tambah Paket Baru",
                "Lihat Semua Riwayat Paket",
                "Lihat Antrean & Update Status",
                "Hapus Riwayat Paket",
                "Batalkan Pengiriman Paket",
                "Logout dari Sistem Admin"
            };
            
            admin = menuNavigasi(menu, 6, "SISTEM MANAJEMEN GUDANG EKSPEDISI (ADMIN)");

            switch (admin){
                case 0: {
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
                    AntriandanUpdateStatus();
                    break;
                }
                
                case 3: {
                    HapusRiwayatPaket_Admin();
                    break;
                }
                
                case 4: {
                    BatalkanPaket_Admin();
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
                    return;
                }
            }
        } while(true);
    }
}