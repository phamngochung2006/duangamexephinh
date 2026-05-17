#pragma once

#include <SFML/Graphics.hpp>

struct CauHinh {
    static constexpr int COT = 10;
    static constexpr int HANG = 20;
    static constexpr int O_SIZE = 28;
    static constexpr int SO_KIEU = 7;
    static constexpr int MAU_RAC = 8;

    static constexpr float TOC_DE = 0.60f;
    static constexpr float TOC_TRUNG = 0.38f;
    static constexpr float TOC_KHO = 0.18f;

    static constexpr const char* FILE_LUU_1P = "xephim_save.dat";
    static constexpr const char* FILE_LUU_2P = "xephim_save_2p.dat";
    static constexpr const char* FILE_KY_LUC = "highscore.txt";
};

extern const float   BANG_TOC_DO[3];
extern const char* NHAN_TOC_DO[3];
extern int           dinhNghiaKhoi[CauHinh::SO_KIEU][4];
extern sf::Color     bangMauKhoi[CauHinh::SO_KIEU];