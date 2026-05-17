#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "cauhinh.h"
#include "hieuung.h"

struct ToaDo {
    int x;
    int y;
};

void ghiSoNguyen(std::ofstream& f, int gia_tri);
int  docSoNguyen(std::ifstream& f);

class BanChoiDon {
private:
    int   luoi[CauHinh::HANG][CauHinh::COT];
    int   luoiMau[CauHinh::HANG][CauHinh::COT];
    ToaDo khoiHienTai[4];
    ToaDo khoiDuPhong[4];
    int   kieuHienTai;
    int   kieuTiepTheo;
    int   diem;
    bool  daGameOver;
    sf::Color mauNguoiChoi;
    bool  dangXoaHang;
    int   demHieuUng;
    bool  hangDay[CauHinh::HANG];
    std::vector<HieuUngDiem> danhSachHieuUng;
    int   hangRacCho;

public:
    BanChoiDon();

    bool kiemTraHopLe();
    void khoaKhoiXuongLuoi();
    int  demHangDay();
    void xoaHangDayVaDon();
    int  tinhDiemTheoSoHang(int soHang);

    void khoiDong(sf::Color mau);
    void taoBienMoi();
    int  buocRoi(float viTriNgang);
    void diChuyen(int dx);
    void xoay();
    void reNhanh();
    void tinhViTriBong(ToaDo bong[4]);
    void nhanHangRac(int soHang);
    void capNhatHieuUng(float dt);

    void luuTrangThai(std::ofstream& f);
    void napTrangThai(std::ifstream& f);

    void veBanChoi(sf::RenderWindow& cua_so, int viTriNgang, sf::Font& phong_chu);
    void vePanel(sf::RenderWindow& cua_so, int panelX, int panelY, sf::Font& phong_chu, const std::string& nhan);
    void veHieuUngDiem(sf::RenderWindow& cua_so, sf::Font& phong_chu);

    int  getDiem() const;
    bool isDaGameOver() const;
    bool isDangXoaHang() const;
    int  getHangRacCho() const;
    int  getKieuHienTai() const;
    int  getKieuTiepTheo() const;
    std::vector<HieuUngDiem>& getDanhSachHieuUng();
};