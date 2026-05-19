#ifndef USER_H
#define USER_H
#include <string>


void MenuUser();
void UpdatePaket_User();
void HapusRiwayatPaket_User();
void BatalkanPaket_User();
void TambahPaket_User();
void LihatRiwayatPaket_User();
void HapusPaket_User();
void RegisterUser();
bool LoginUser();
void MenuUtamaUser();
void tampilPesan(std::string pesan = "Proses selesai");

#endif