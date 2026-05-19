#ifndef MODELS_H
#define MODELS_H
#include <iostream>
#include <limits>
#include <string>

using namespace std;

#ifdef _WIN32
    #define CLEAR "cls"
    #include <conio.h>
#else
    #define CLEAR "clear"
    
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


struct Paket {
    string resi;
    string namaPengirim;
    string namaPenerima;
    string alamat;
    string lokasi;
    long long berat;
    string tipe;
    string status;
    string pemilik;
    long long ongkir;
    string pembayaran;
};

struct User {
    string username;
    string password;
};

#define MAX_PAKET 100


inline void tekanEnter() {
    cout << "\nTekan ENTER untuk melanjutkan...";
    string buang;
    getline(cin, buang);
};

inline void bersihkanLayar() {
    system(CLEAR);
}

extern Paket paket[100];
extern int jumlahPaket;

int inputAngka(string pesan);

#endif