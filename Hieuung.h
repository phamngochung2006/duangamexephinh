#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class HieuUngDiem {
private:
    float       x;
    float       y;
    float       tocDoDoc;
    float       doMo;
    float       thoiGianCon;
    std::string noiDung;

public:
    HieuUngDiem(float px, float py, int diem);

    bool        capNhat(float dt);

    float       getX() const;
    float       getY() const;
    float       getDoMo() const;
    std::string getNoiDung() const;
    void        setNoiDung(const std::string& nd);
};