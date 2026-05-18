#ifndef admin_h
#define admin_h
#include "data.h"
#include<vector>


void MenuAdmin();
void TambahPaketAdmin();
void tampilRiwayatPaket();
void AntriandanUpdateStatus();
void HapusRiwayatPaket_Admin();
void BatalkanPaket_Admin();
bool cekResiDiJson(const string& Cari);
void loadPaket();
void savePaket();
int MenuScroll(string judul, vector<string> pilihan);
string generateResi();


#endif