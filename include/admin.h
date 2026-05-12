#ifndef admin_h
#define admin_h
#include "data.h"

void MenuAdmin();
void TambahPaketAdmin();
void tampilRiwayatPaket();
void AntriandanUpdateStatus();
void HapusRiwayatPaket_Admin();
void BatalkanPaket_Admin();
bool cekResiDiJson(const string& Cari);
string generateResi();

#endif