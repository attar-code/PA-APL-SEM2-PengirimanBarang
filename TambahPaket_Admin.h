#ifndef TambahPaketAdmin_h
#define TambahPaketAdmin_h
#include "data.h"

void TambahPaketAdmin();
void tampilRiwayatPaket();
void AntriandanUpdateStatus(Paket DaftarPaket[], int jumlahPaket);
void HapusRiwayatPaket_Admin();
void BatalkanPaket(Paket DaftarPaket[], int &jumlahPaket);

#endif