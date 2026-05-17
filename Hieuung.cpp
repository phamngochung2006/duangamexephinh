#include "hieuung.h"
#include <algorithm>

HieuUngDiem::HieuUngDiem(float px, float py, int diem) {
    x = px;
    y = py;
    tocDoDoc = -150.f;
    doMo = 255.f;
    thoiGianCon = 0.5f;
    noiDung = "+" + std::to_string(diem);
}

bool HieuUngDiem::capNhat(float dt) {
    y += tocDoDoc * dt;
    doMo -= 200.f * dt;
    thoiGianCon -= dt;
    return thoiGianCon > 0;
}

float HieuUngDiem::getX() const { return x; }
float HieuUngDiem::getY() const { return y; }
float HieuUngDiem::getDoMo() const { return doMo; }
std::string HieuUngDiem::getNoiDung() const { return noiDung; }
void HieuUngDiem::setNoiDung(const std::string& nd) { noiDung = nd; }