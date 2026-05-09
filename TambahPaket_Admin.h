#ifndef TambahPaketAdmin_h
#define TambahPaketAdmin_h
#include "data.h"

void TambahPaketAdmin(Paket DaftarPaket[], int &jumlahPaket);
void LihatRiwayat(Paket DaftarPaket[], int jumlahPaket);
void AntriandanUpdateStatus(Paket DaftarPaket[], int jumlahPaket);
void HapusRiwayatPaket(Paket DaftarPaket[], int &jumlahPaket);
void BatalkanPaket(Paket DaftarPaket[], int &jumlahPaket);

#endif