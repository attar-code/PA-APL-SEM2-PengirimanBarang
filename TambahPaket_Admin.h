#ifndef TambahPaketAdmin_h
#define TambahPaketAdmin_h
#include "data.h"

void TambahPaketAdmin();
void tampilRiwayatPaket();
void AntriandanUpdateStatus();
void HapusRiwayatPaket_Admin();
void BatalkanPaket_Admin();
bool cekResiDiJson(const string& Cari);
string generateResi();

#endif