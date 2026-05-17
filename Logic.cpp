#include "logic.h"
#include <algorithm>
#include <cstdlib>

void ghiSoNguyen(std::ofstream& f, int gia_tri) {
    f.write(reinterpret_cast<const char*>(&gia_tri), sizeof(gia_tri));
    if (!f) throw std::runtime_error("Loi ghi file");
}

int docSoNguyen(std::ifstream& f) {
    int gia_tri = 0;
    f.read(reinterpret_cast<char*>(&gia_tri), sizeof(gia_tri));
    if (!f) throw std::runtime_error("Loi doc file");
    return gia_tri;
}

BanChoiDon::BanChoiDon()
    : kieuHienTai(0), kieuTiepTheo(0), diem(0),
    daGameOver(false), dangXoaHang(false), demHieuUng(0), hangRacCho(0) {
    for (int i = 0; i < CauHinh::HANG; i++)
        for (int j = 0; j < CauHinh::COT; j++)
            luoi[i][j] = luoiMau[i][j] = 0;
    for (int i = 0; i < CauHinh::HANG; i++) hangDay[i] = false;
    for (int i = 0; i < 4; i++) khoiHienTai[i] = khoiDuPhong[i] = { 0, 0 };
}

void BanChoiDon::khoiDong(sf::Color mau) {
    mauNguoiChoi = mau;
    kieuTiepTheo = rand() % CauHinh::SO_KIEU;
    diem = 0;
    daGameOver = false;
    hangRacCho = 0;
    danhSachHieuUng.clear();
    dangXoaHang = false;
    demHieuUng = 0;
    for (int i = 0; i < CauHinh::HANG; i++)
        for (int j = 0; j < CauHinh::COT; j++)
            luoi[i][j] = luoiMau[i][j] = 0;
    for (int i = 0; i < CauHinh::HANG; i++) hangDay[i] = false;
    taoBienMoi();
}

void BanChoiDon::taoBienMoi() {
    kieuHienTai = kieuTiepTheo;
    kieuTiepTheo = rand() % CauHinh::SO_KIEU;
    for (int i = 0; i < 4; i++) {
        khoiHienTai[i].x = dinhNghiaKhoi[kieuHienTai][i] % 2 + CauHinh::COT / 2 - 1;
        khoiHienTai[i].y = dinhNghiaKhoi[kieuHienTai][i] / 2;
    }
    if (!kiemTraHopLe()) daGameOver = true;
}

bool BanChoiDon::kiemTraHopLe() {
    for (int i = 0; i < 4; i++) {
        int cx = khoiHienTai[i].x, cy = khoiHienTai[i].y;
        if (cx < 0 || cx >= CauHinh::COT || cy >= CauHinh::HANG) return false;
        if (cy >= 0 && luoi[cy][cx]) return false;
    }
    return true;
}

void BanChoiDon::khoaKhoiXuongLuoi() {
    for (int i = 0; i < 4; i++) {
        luoi[khoiDuPhong[i].y][khoiDuPhong[i].x] = 1;
        luoiMau[khoiDuPhong[i].y][khoiDuPhong[i].x] = kieuHienTai + 1;
    }
}

int BanChoiDon::demHangDay() {
    int soHang = 0;
    for (int i = 0; i < CauHinh::HANG; i++) {
        int soO = 0;
        for (int j = 0; j < CauHinh::COT; j++) if (luoi[i][j]) soO++;
        hangDay[i] = (soO == CauHinh::COT);
        if (hangDay[i]) soHang++;
    }
    return soHang;
}

void BanChoiDon::xoaHangDayVaDon() {
    int k = CauHinh::HANG - 1;
    for (int i = CauHinh::HANG - 1; i >= 0; i--) {
        if (!hangDay[i]) {
            for (int j = 0; j < CauHinh::COT; j++) {
                luoi[k][j] = luoi[i][j];
                luoiMau[k][j] = luoiMau[i][j];
            }
            k--;
        }
    }
    for (int i = 0; i <= k; i++)
        for (int j = 0; j < CauHinh::COT; j++)
            luoi[i][j] = luoiMau[i][j] = 0;
}

int BanChoiDon::tinhDiemTheoSoHang(int soHang) {
    switch (soHang) {
    case 1:  return 100;
    case 2:  return 300;
    case 3:  return 600;
    case 4:  return 1000;
    default: return 0;
    }
}

int BanChoiDon::buocRoi(float viTriNgang) {
    if (dangXoaHang) {
        if (--demHieuUng <= 0) {
            xoaHangDayVaDon();
            dangXoaHang = false;
        }
        return 0;
    }

    for (int i = 0; i < 4; i++) {
        khoiDuPhong[i] = khoiHienTai[i];
        khoiHienTai[i].y++;
    }

    if (!kiemTraHopLe()) {
        for (int i = 0; i < 4; i++) khoiHienTai[i] = khoiDuPhong[i];
        khoaKhoiXuongLuoi();

        int soHangXoa = demHangDay();
        if (soHangXoa > 0) {
            dangXoaHang = true;
            demHieuUng = 2;
            int diemKiemDuoc = tinhDiemTheoSoHang(soHangXoa);
            diem += diemKiemDuoc;

            float ex = viTriNgang + (CauHinh::COT * CauHinh::O_SIZE) / 2.f;
            float ey = CauHinh::HANG * CauHinh::O_SIZE / 2.f;
            danhSachHieuUng.push_back(HieuUngDiem(ex, ey, diemKiemDuoc));
            if (soHangXoa == 4) {
                danhSachHieuUng.push_back(HieuUngDiem(ex - 20, ey + 30, 0));
                danhSachHieuUng.back().setNoiDung("TETRIS!");
            }
        }
        taoBienMoi();
        return soHangXoa;
    }
    return 0;
}

void BanChoiDon::diChuyen(int dx) {
    for (int i = 0; i < 4; i++) {
        khoiDuPhong[i] = khoiHienTai[i];
        khoiHienTai[i].x += dx;
    }
    if (!kiemTraHopLe())
        for (int i = 0; i < 4; i++) khoiHienTai[i] = khoiDuPhong[i];
}

void BanChoiDon::xoay() {
    ToaDo tamXoay = khoiHienTai[1];
    for (int i = 0; i < 4; i++) {
        int dx = khoiHienTai[i].y - tamXoay.y;
        int dy = khoiHienTai[i].x - tamXoay.x;
        khoiDuPhong[i] = khoiHienTai[i];
        khoiHienTai[i].x = tamXoay.x - dx;
        khoiHienTai[i].y = tamXoay.y + dy;
    }
    if (!kiemTraHopLe())
        for (int i = 0; i < 4; i++) khoiHienTai[i] = khoiDuPhong[i];
}

void BanChoiDon::reNhanh() {
    while (true) {
        for (int i = 0; i < 4; i++) {
            khoiDuPhong[i] = khoiHienTai[i];
            khoiHienTai[i].y++;
        }
        if (!kiemTraHopLe()) {
            for (int i = 0; i < 4; i++) khoiHienTai[i] = khoiDuPhong[i];
            break;
        }
    }
}

void BanChoiDon::tinhViTriBong(ToaDo bong[4]) {
    for (int i = 0; i < 4; i++) bong[i] = khoiHienTai[i];
    ToaDo tam[4];
    while (true) {
        for (int i = 0; i < 4; i++) {
            tam[i] = bong[i];
            bong[i].y++;
        }
        bool hopLe = true;
        for (int i = 0; i < 4; i++) {
            if (bong[i].x < 0 || bong[i].x >= CauHinh::COT || bong[i].y >= CauHinh::HANG)
            {
                hopLe = false; break;
            }
            if (bong[i].y >= 0 && luoi[bong[i].y][bong[i].x])
            {
                hopLe = false; break;
            }
        }
        if (!hopLe) {
            for (int i = 0; i < 4; i++) bong[i] = tam[i];
            break;
        }
    }
}

void BanChoiDon::nhanHangRac(int soHang) {
    if (soHang <= 0) return;
    for (int i = 0; i < CauHinh::HANG - soHang; i++)
        for (int j = 0; j < CauHinh::COT; j++) {
            luoi[i][j] = luoi[i + soHang][j];
            luoiMau[i][j] = luoiMau[i + soHang][j];
        }
    int viTriLo = rand() % CauHinh::COT;
    for (int i = CauHinh::HANG - soHang; i < CauHinh::HANG; i++)
        for (int j = 0; j < CauHinh::COT; j++) {
            luoi[i][j] = (j == viTriLo) ? 0 : 1;
            luoiMau[i][j] = (j == viTriLo) ? 0 : CauHinh::MAU_RAC;
        }
    for (int k = 0; k < soHang; k++) {
        bool biChong = false;
        for (int i = 0; i < 4; i++)
            if (khoiHienTai[i].y >= 0 && khoiHienTai[i].y < CauHinh::HANG
                && luoi[khoiHienTai[i].y][khoiHienTai[i].x])
            {
                biChong = true; break;
            }
        if (biChong)
            for (int i = 0; i < 4; i++) khoiHienTai[i].y--;
    }
}

void BanChoiDon::capNhatHieuUng(float dt) {
    for (int i = (int)danhSachHieuUng.size() - 1; i >= 0; i--)
        if (!danhSachHieuUng[i].capNhat(dt))
            danhSachHieuUng.erase(danhSachHieuUng.begin() + i);
}

void BanChoiDon::luuTrangThai(std::ofstream& f) {
    for (int i = 0; i < CauHinh::HANG; i++)
        for (int j = 0; j < CauHinh::COT; j++) {
            ghiSoNguyen(f, luoi[i][j]);
            ghiSoNguyen(f, luoiMau[i][j]);
        }
    for (int i = 0; i < 4; i++) {
        ghiSoNguyen(f, khoiHienTai[i].x);
        ghiSoNguyen(f, khoiHienTai[i].y);
    }
    ghiSoNguyen(f, kieuHienTai);
    ghiSoNguyen(f, kieuTiepTheo);
    ghiSoNguyen(f, diem);
}

void BanChoiDon::napTrangThai(std::ifstream& f) {
    for (int i = 0; i < CauHinh::HANG; i++)
        for (int j = 0; j < CauHinh::COT; j++) {
            luoi[i][j] = docSoNguyen(f);
            luoiMau[i][j] = docSoNguyen(f);
        }
    for (int i = 0; i < 4; i++) {
        khoiHienTai[i].x = docSoNguyen(f);
        khoiHienTai[i].y = docSoNguyen(f);
    }
    kieuHienTai = docSoNguyen(f);
    kieuTiepTheo = docSoNguyen(f);
    diem = docSoNguyen(f);
    daGameOver = false;
    dangXoaHang = false;
    hangRacCho = 0;
    danhSachHieuUng.clear();
}

void BanChoiDon::veBanChoi(sf::RenderWindow& cua_so, int viTriNgang, sf::Font& phong_chu) {
    sf::RectangleShape nen(sf::Vector2f(CauHinh::COT * CauHinh::O_SIZE, CauHinh::HANG * CauHinh::O_SIZE));
    nen.setPosition(viTriNgang, 0);
    nen.setFillColor(sf::Color(15, 15, 30));
    cua_so.draw(nen);

    sf::RectangleShape duongDoc(sf::Vector2f(1, CauHinh::HANG * CauHinh::O_SIZE));
    duongDoc.setFillColor(sf::Color(40, 40, 60));
    for (int j = 1; j < CauHinh::COT; j++) {
        duongDoc.setPosition(viTriNgang + j * CauHinh::O_SIZE, 0);
        cua_so.draw(duongDoc);
    }
    sf::RectangleShape duongNgang(sf::Vector2f(CauHinh::COT * CauHinh::O_SIZE, 1));
    duongNgang.setFillColor(sf::Color(40, 40, 60));
    for (int i = 1; i < CauHinh::HANG; i++) {
        duongNgang.setPosition(viTriNgang, i * CauHinh::O_SIZE);
        cua_so.draw(duongNgang);
    }

    sf::RectangleShape o(sf::Vector2f(CauHinh::O_SIZE - 2.f, CauHinh::O_SIZE - 2.f));
    for (int i = 0; i < CauHinh::HANG; i++) {
        for (int j = 0; j < CauHinh::COT; j++) {
            if (!luoi[i][j]) continue;
            int ci = luoiMau[i][j] - 1;
            sf::Color mauO;
            if (ci == CauHinh::MAU_RAC - 1)
                mauO = sf::Color(100, 100, 100);
            else
                mauO = (ci >= 0 && ci < CauHinh::SO_KIEU) ? bangMauKhoi[ci] : mauNguoiChoi;
            if (dangXoaHang && hangDay[i])
                mauO = (demHieuUng % 3 == 0) ? sf::Color::White
                : sf::Color(mauO.r / 2, mauO.g / 2, mauO.b / 2);
            o.setFillColor(mauO);
            o.setPosition(viTriNgang + j * CauHinh::O_SIZE + 1, i * CauHinh::O_SIZE + 1);
            cua_so.draw(o);
            sf::RectangleShape sang(sf::Vector2f(CauHinh::O_SIZE - 2.f, 3.f));
            sang.setFillColor(sf::Color(255, 255, 255, 60));
            sang.setPosition(viTriNgang + j * CauHinh::O_SIZE + 1, i * CauHinh::O_SIZE + 1);
            cua_so.draw(sang);
        }
    }

    ToaDo bong[4];
    tinhViTriBong(bong);
    sf::Color mauBong = bangMauKhoi[kieuHienTai];
    mauBong.a = 60;
    o.setFillColor(mauBong);
    for (int i = 0; i < 4; i++) {
        if (bong[i].y >= 0) {
            o.setPosition(viTriNgang + bong[i].x * CauHinh::O_SIZE + 1,
                bong[i].y * CauHinh::O_SIZE + 1);
            cua_so.draw(o);
        }
    }

    sf::Color mauKhoi = bangMauKhoi[kieuHienTai];
    mauKhoi.a = 255;
    o.setFillColor(mauKhoi);
    for (int i = 0; i < 4; i++) {
        if (khoiHienTai[i].y >= 0) {
            o.setPosition(viTriNgang + khoiHienTai[i].x * CauHinh::O_SIZE + 1,
                khoiHienTai[i].y * CauHinh::O_SIZE + 1);
            cua_so.draw(o);
            sf::RectangleShape sang(sf::Vector2f(CauHinh::O_SIZE - 2.f, 3.f));
            sang.setFillColor(sf::Color(255, 255, 255, 80));
            sang.setPosition(viTriNgang + khoiHienTai[i].x * CauHinh::O_SIZE + 1,
                khoiHienTai[i].y * CauHinh::O_SIZE + 1);
            cua_so.draw(sang);
        }
    }

    sf::RectangleShape khung(sf::Vector2f(CauHinh::COT * CauHinh::O_SIZE, CauHinh::HANG * CauHinh::O_SIZE));
    khung.setPosition(viTriNgang, 0);
    khung.setFillColor(sf::Color::Transparent);
    khung.setOutlineThickness(2);
    khung.setOutlineColor(sf::Color(100, 100, 160));
    cua_so.draw(khung);
}

void BanChoiDon::vePanel(sf::RenderWindow& cua_so, int panelX, int panelY, sf::Font& phong_chu, const std::string& nhan) {
    sf::RectangleShape nen(sf::Vector2f(130, 310));
    nen.setPosition(panelX, panelY);
    nen.setFillColor(sf::Color(20, 20, 40, 200));
    nen.setOutlineThickness(1);
    nen.setOutlineColor(sf::Color(80, 80, 120));
    cua_so.draw(nen);

    auto veChu = [&](const std::string& s, int x, int y, int cz, sf::Color c) {
        sf::Text t(s, phong_chu, cz);
        t.setPosition((float)x, (float)y);
        t.setFillColor(c);
        cua_so.draw(t);
        };

    veChu(nhan, panelX + 10, panelY + 8, 16, mauNguoiChoi);
    veChu("DIEM", panelX + 10, panelY + 35, 12, sf::Color(160, 160, 200));
    veChu(std::to_string(diem), panelX + 10, panelY + 52, 22, sf::Color::White);
    veChu("TIEP THEO", panelX + 10, panelY + 90, 12, sf::Color(160, 160, 200));

    int coChuKhoi = 22;
    ToaDo xemTruoc[4];
    for (int i = 0; i < 4; i++) {
        xemTruoc[i].x = dinhNghiaKhoi[kieuTiepTheo][i] % 2;
        xemTruoc[i].y = dinhNghiaKhoi[kieuTiepTheo][i] / 2;
    }
    sf::RectangleShape oXem(sf::Vector2f(coChuKhoi - 2.f, coChuKhoi - 2.f));
    oXem.setFillColor(bangMauKhoi[kieuTiepTheo]);
    int gocX = panelX + 30, gocY = panelY + 115;
    for (int i = 0; i < 4; i++) {
        oXem.setPosition(gocX + xemTruoc[i].x * coChuKhoi, gocY + xemTruoc[i].y * coChuKhoi);
        cua_so.draw(oXem);
    }

    if (hangRacCho > 0)
        veChu("RAC: " + std::to_string(hangRacCho), panelX + 10, panelY + 190, 13, sf::Color(255, 80, 80));
}

void BanChoiDon::veHieuUngDiem(sf::RenderWindow& cua_so, sf::Font& phong_chu) {
    for (auto& hu : danhSachHieuUng) {
        bool laTetris = (hu.getNoiDung() == "TETRIS!");
        sf::Text t(hu.getNoiDung(), phong_chu, laTetris ? 26 : 22);
        sf::Uint8 doMo = (sf::Uint8)std::max(0.f, hu.getDoMo());
        t.setFillColor(laTetris ? sf::Color(255, 220, 0, doMo) : sf::Color(100, 255, 100, doMo));
        t.setOutlineColor(sf::Color(0, 0, 0, doMo));
        t.setOutlineThickness(2);
        t.setPosition(hu.getX() - t.getLocalBounds().width / 2.f, hu.getY());
        cua_so.draw(t);
    }
}

int  BanChoiDon::getDiem() const { return diem; }
bool BanChoiDon::isDaGameOver() const { return daGameOver; }
bool BanChoiDon::isDangXoaHang() const { return dangXoaHang; }
int  BanChoiDon::getHangRacCho() const { return hangRacCho; }
int  BanChoiDon::getKieuHienTai() const { return kieuHienTai; }
int  BanChoiDon::getKieuTiepTheo() const { return kieuTiepTheo; }

std::vector<HieuUngDiem>& BanChoiDon::getDanhSachHieuUng() {
    return danhSachHieuUng;
}