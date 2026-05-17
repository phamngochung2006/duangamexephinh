#pragma once

#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <cstdint>

class AmThanh {
private:
    sf::SoundBuffer bufferXoaHang;
    sf::SoundBuffer bufferTetris;
    sf::SoundBuffer bufferRe;
    sf::SoundBuffer bufferXoay;
    sf::SoundBuffer bufferDiChuyen;
    sf::SoundBuffer bufferGameOver;
    sf::SoundBuffer bufferRac;

    sf::Sound soundXoaHang;
    sf::Sound soundTetris;
    sf::Sound soundRe;
    sf::Sound soundXoay;
    sf::Sound soundDiChuyen;
    sf::Sound soundGameOver;
    sf::Sound soundRac;

    sf::Music nhacNen;

    bool dacTat;

public:
    AmThanh();

    std::vector<sf::Int16> taoSinWave(float tanSo, float thoiGian, unsigned int mauPerGiay, float doGiam = 1.0f);
    std::vector<sf::Int16> taoNoise(float thoiGian, unsigned int mauPerGiay, float doGiam = 0.5f);
    std::vector<sf::Int16> taoChord(float tan1, float tan2, float thoiGian, unsigned int mauPerGiay);
    bool khoiTaoBuffer(sf::SoundBuffer& buffer, const std::vector<sf::Int16>& mau, unsigned int mauPerGiay);

    void batNhacNen();
    void dungNhacNen();

    void phatXoaHang();
    void phatTetris();
    void phatRe();
    void phatXoay();
    void phatDiChuyen();
    void phatGameOver();
    void phatRac();

    void tatAmThanh();
    void batAmThanh();

    bool isDacTat() const;
    void setAmLuong(float mucAm);
};