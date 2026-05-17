#include "cauhinh.h"

const float BANG_TOC_DO[3] = {
    CauHinh::TOC_DE,
    CauHinh::TOC_TRUNG,
    CauHinh::TOC_KHO
};

const char* NHAN_TOC_DO[3] = {
    "DE",
    "TRUNG BINH",
    "KHO"
};

int dinhNghiaKhoi[CauHinh::SO_KIEU][4] = {
    {1, 3, 5, 7},
    {2, 4, 5, 7},
    {3, 5, 4, 6},
    {3, 5, 4, 7},
    {2, 3, 5, 7},
    {3, 5, 7, 6},
    {2, 3, 4, 5}
};

sf::Color bangMauKhoi[CauHinh::SO_KIEU] = {
    sf::Color(0,   220, 220),
    sf::Color(0,   200, 50),
    sf::Color(220, 50,  50),
    sf::Color(160, 0,   220),
    sf::Color(220, 140, 0),
    sf::Color(0,   80,  220),
    sf::Color(220, 220, 0)
};