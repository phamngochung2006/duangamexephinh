#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "logic.h"
#include "amthanh.h"
#include "cauhinh.h"

class TroChoi {
private:
    sf::RenderWindow cuaSo;
    sf::Font         phongChu;
    AmThanh          amThanh;

    enum class TrangThaiGame {
        Menu, ChonTocDo,
        Choi1Nguoi, Choi2Nguoi, DoiKhang,
        HuongDan, KyLuc
    };

    TrangThaiGame trangThai;

    BanChoiDon nguoiChoi1;
    BanChoiDon nguoiChoi2;

    sf::Clock dongHo;
    float     thoiGianTich;
    float     dt;
    int       diemKyLuc;
    int       mucTocDo;
    bool      cho2Nguoi;
    bool      choDoiKhang;
    bool      nguoiChoi1DaThua;
    bool      nguoiChoi2DaThua;
    bool      gameOverDaPhat1;
    bool      gameOverDaPhat2;

    static constexpr int BAN_1P_X = 200;
    static constexpr int PANEL_1P_X = 210 + CauHinh::COT * CauHinh::O_SIZE + 10;

    static constexpr int BAN_2P_1_X = 20;
    static constexpr int PANEL_2P_1_X = 20 + CauHinh::COT * CauHinh::O_SIZE + 5;
    static constexpr int BAN_2P_2_X = 450;
    static constexpr int PANEL_2P_2_X = 450 + CauHinh::COT * CauHinh::O_SIZE + 5;

    static constexpr int BAN_DK_1_X = 10;
    static constexpr int PANEL_DK_1_X = 10 + CauHinh::COT * CauHinh::O_SIZE + 5;
    static constexpr int BAN_DK_2_X = 440;
    static constexpr int PANEL_DK_2_X = 440 + CauHinh::COT * CauHinh::O_SIZE + 5;

public:
    TroChoi();
    void chay();

    void docKyLuc();
    void luuKyLuc(int diem);
    void luuGame1Nguoi();
    bool napGame1Nguoi();
    void luuGame2Nguoi();
    bool napGame2Nguoi();
    void xoaFileLuu(const char* duong_dan);
    bool coFileLuu1P();
    bool coFileLuu2P();

    void veChuLen(const std::string& noi_dung, int x, int y, int co_chu, sf::Color mau);
    void veOverlayDaThua(int viTriNgang, const std::string& dongChu2);
    void veMuiTenDoiKhangGiua();

    void batDauGame();
    void xuLyPhimNhan(sf::Event& su_kien);

    void veManHinhMenu();
    void veManHinhChonTocDo();
    void veManHinhHuongDan();
    void veManHinhKyLuc();

    void choi1Nguoi();
    void choi2Nguoi();
    void choiDoiKhang();
};