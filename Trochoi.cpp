#include "trochoi.h"
#include <fstream>
#include <algorithm>
#include <iostream>

TroChoi::TroChoi()
    : cuaSo(sf::VideoMode(820, CauHinh::HANG* CauHinh::O_SIZE + 60), "Xep Hinh"),
    trangThai(TrangThaiGame::Menu),
    thoiGianTich(0.f),
    dt(0.f),
    diemKyLuc(0),
    mucTocDo(1),
    cho2Nguoi(false),
    choDoiKhang(false),
    nguoiChoi1DaThua(false),
    nguoiChoi2DaThua(false),
    gameOverDaPhat1(false),
    gameOverDaPhat2(false) {

    srand((unsigned int)time(0));

    bool fontLoaded = phongChu.loadFromFile("C:/Windows/Fonts/arial.ttf");
    if (!fontLoaded)
        fontLoaded = phongChu.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf");
    if (!fontLoaded)
        std::cerr << "[CANH BAO] Khong tai duoc font chu.\n";

    docKyLuc();
    amThanh.batNhacNen();
}

void TroChoi::docKyLuc() {
    try {
        std::ifstream f(CauHinh::FILE_KY_LUC);
        if (f) f >> diemKyLuc;
    }
    catch (...) {
        diemKyLuc = 0;
    }
}

void TroChoi::luuKyLuc(int diem) {
    try {
        if (diem > diemKyLuc) {
            diemKyLuc = diem;
            std::ofstream f(CauHinh::FILE_KY_LUC);
            if (f) f << diemKyLuc;
        }
    }
    catch (...) {}
}

void TroChoi::luuGame1Nguoi() {
    try {
        std::ofstream f(CauHinh::FILE_LUU_1P, std::ios::binary);
        if (!f) return;
        nguoiChoi1.luuTrangThai(f);
        ghiSoNguyen(f, mucTocDo);
    }
    catch (...) {}
}

bool TroChoi::napGame1Nguoi() {
    try {
        std::ifstream f(CauHinh::FILE_LUU_1P, std::ios::binary);
        if (!f) return false;
        nguoiChoi1.khoiDong(sf::Color(50, 220, 80));
        nguoiChoi1.napTrangThai(f);
        mucTocDo = docSoNguyen(f);
        if (mucTocDo < 0 || mucTocDo > 2) mucTocDo = 1;
        return true;
    }
    catch (...) {
        return false;
    }
}

void TroChoi::luuGame2Nguoi() {
    try {
        std::ofstream f(CauHinh::FILE_LUU_2P, std::ios::binary);
        if (!f) return;
        nguoiChoi1.luuTrangThai(f);
        nguoiChoi2.luuTrangThai(f);
        ghiSoNguyen(f, mucTocDo);
    }
    catch (...) {}
}

bool TroChoi::napGame2Nguoi() {
    try {
        std::ifstream f(CauHinh::FILE_LUU_2P, std::ios::binary);
        if (!f) return false;
        nguoiChoi1.khoiDong(sf::Color(50, 220, 80));
        nguoiChoi2.khoiDong(sf::Color(220, 80, 80));
        nguoiChoi1.napTrangThai(f);
        nguoiChoi2.napTrangThai(f);
        mucTocDo = docSoNguyen(f);
        if (mucTocDo < 0 || mucTocDo > 2) mucTocDo = 1;
        return true;
    }
    catch (...) {
        return false;
    }
}

void TroChoi::xoaFileLuu(const char* duong_dan) {
    remove(duong_dan);
}

bool TroChoi::coFileLuu1P() {
    std::ifstream f(CauHinh::FILE_LUU_1P, std::ios::binary);
    return f.good();
}

bool TroChoi::coFileLuu2P() {
    std::ifstream f(CauHinh::FILE_LUU_2P, std::ios::binary);
    return f.good();
}

void TroChoi::veChuLen(const std::string& noi_dung, int x, int y, int co_chu, sf::Color mau) {
    sf::Text t(noi_dung, phongChu, co_chu);
    t.setPosition((float)x, (float)y);
    t.setFillColor(mau);
    cuaSo.draw(t);
}

void TroChoi::veOverlayDaThua(int viTriNgang, const std::string& dongChu2) {
    sf::RectangleShape man(sf::Vector2f(CauHinh::COT * CauHinh::O_SIZE, CauHinh::HANG * CauHinh::O_SIZE));
    man.setPosition(viTriNgang, 0);
    man.setFillColor(sf::Color(0, 0, 0, 180));
    cuaSo.draw(man);

    sf::Text chuThua("THUA", phongChu, 32);
    chuThua.setFillColor(sf::Color(220, 60, 60));
    chuThua.setPosition(
        viTriNgang + CauHinh::COT * CauHinh::O_SIZE / 2.f - chuThua.getLocalBounds().width / 2.f,
        CauHinh::HANG * CauHinh::O_SIZE / 2.f - 20);
    cuaSo.draw(chuThua);

    sf::Text chuCho(dongChu2, phongChu, 14);
    chuCho.setFillColor(sf::Color(200, 200, 200));
    chuCho.setPosition(
        viTriNgang + CauHinh::COT * CauHinh::O_SIZE / 2.f - chuCho.getLocalBounds().width / 2.f,
        CauHinh::HANG * CauHinh::O_SIZE / 2.f + 22);
    cuaSo.draw(chuCho);
}

void TroChoi::veMuiTenDoiKhangGiua() {
    int cx = 415, cy = CauHinh::HANG * CauHinh::O_SIZE / 2;

    sf::RectangleShape thanh(sf::Vector2f(30, 4));
    thanh.setFillColor(sf::Color(200, 200, 60));
    thanh.setPosition(cx - 15, cy - 2);
    cuaSo.draw(thanh);

    sf::ConvexShape dauTrai;
    dauTrai.setPointCount(3);
    dauTrai.setPoint(0, { 0,  0 });
    dauTrai.setPoint(1, { 10, -7 });
    dauTrai.setPoint(2, { 10,  7 });
    dauTrai.setFillColor(sf::Color(200, 200, 60));
    dauTrai.setPosition(cx - 15, cy);
    cuaSo.draw(dauTrai);

    sf::ConvexShape dauPhai;
    dauPhai.setPointCount(3);
    dauPhai.setPoint(0, { 10, 0 });
    dauPhai.setPoint(1, { 0, -7 });
    dauPhai.setPoint(2, { 0,  7 });
    dauPhai.setFillColor(sf::Color(200, 200, 60));
    dauPhai.setPosition(cx + 5, cy);
    cuaSo.draw(dauPhai);
}

void TroChoi::batDauGame() {
    nguoiChoi1DaThua = false;
    nguoiChoi2DaThua = false;
    gameOverDaPhat1 = false;
    gameOverDaPhat2 = false;

    if (choDoiKhang) {
        nguoiChoi1.khoiDong(sf::Color(50, 220, 80));
        nguoiChoi2.khoiDong(sf::Color(220, 80, 80));
        trangThai = TrangThaiGame::DoiKhang;
    }
    else if (!cho2Nguoi) {
        nguoiChoi1.khoiDong(sf::Color(50, 220, 80));
        trangThai = TrangThaiGame::Choi1Nguoi;
    }
    else {
        nguoiChoi1.khoiDong(sf::Color(50, 220, 80));
        nguoiChoi2.khoiDong(sf::Color(220, 80, 80));
        trangThai = TrangThaiGame::Choi2Nguoi;
    }
    thoiGianTich = 0;
    amThanh.batNhacNen();
}

void TroChoi::xuLyPhimNhan(sf::Event& su_kien) {
    using KB = sf::Keyboard;
    using TS = TrangThaiGame;

    if (trangThai == TS::Menu) {
        if (su_kien.key.code == KB::Num1) {
            cho2Nguoi = false; choDoiKhang = false; trangThai = TS::ChonTocDo;
        }
        if (su_kien.key.code == KB::Num2) {
            cho2Nguoi = true;  choDoiKhang = false; trangThai = TS::ChonTocDo;
        }
        if (su_kien.key.code == KB::Num3) {
            cho2Nguoi = false; choDoiKhang = true;  trangThai = TS::ChonTocDo;
        }
        if (su_kien.key.code == KB::Num4) trangThai = TS::HuongDan;
        if (su_kien.key.code == KB::Num5) trangThai = TS::KyLuc;
        if (su_kien.key.code == KB::Num6) cuaSo.close();
        if (su_kien.key.code == KB::Num7 && coFileLuu1P()) {
            if (napGame1Nguoi()) {
                trangThai = TS::Choi1Nguoi;
                thoiGianTich = 0;
                gameOverDaPhat1 = false;
                amThanh.batNhacNen();
            }
        }
        if (su_kien.key.code == KB::Num8 && coFileLuu2P()) {
            if (napGame2Nguoi()) {
                trangThai = TS::Choi2Nguoi;
                thoiGianTich = 0;
                gameOverDaPhat1 = gameOverDaPhat2 = false;
                amThanh.batNhacNen();
            }
        }
        if (su_kien.key.code == KB::M) {
            if (amThanh.isDacTat()) amThanh.batAmThanh();
            else                    amThanh.tatAmThanh();
        }
        return;
    }

    if (trangThai == TS::ChonTocDo) {
        if (su_kien.key.code == KB::Num1) { mucTocDo = 0; batDauGame(); }
        if (su_kien.key.code == KB::Num2) { mucTocDo = 1; batDauGame(); }
        if (su_kien.key.code == KB::Num3) { mucTocDo = 2; batDauGame(); }
        if (su_kien.key.code == KB::Escape) trangThai = TS::Menu;
        return;
    }

    if (su_kien.key.code == KB::Escape) {
        if (trangThai == TS::Choi1Nguoi) luuGame1Nguoi();
        if (trangThai == TS::Choi2Nguoi) luuGame2Nguoi();
        amThanh.dungNhacNen();
        trangThai = TS::Menu;
        return;
    }

    if (su_kien.key.code == KB::M) {
        if (amThanh.isDacTat()) amThanh.batAmThanh();
        else                    amThanh.tatAmThanh();
        return;
    }

    bool nc1CoTheDiChuyen = (trangThai == TS::Choi1Nguoi ||
        trangThai == TS::Choi2Nguoi ||
        trangThai == TS::DoiKhang) && !nguoiChoi1DaThua;

    if (nc1CoTheDiChuyen) {
        if (su_kien.key.code == KB::Left) {
            nguoiChoi1.diChuyen(-1);
            amThanh.phatDiChuyen();
        }
        if (su_kien.key.code == KB::Right) {
            nguoiChoi1.diChuyen(1);
            amThanh.phatDiChuyen();
        }
        if (su_kien.key.code == KB::Up) {
            nguoiChoi1.xoay();
            amThanh.phatXoay();
        }
        if (su_kien.key.code == KB::Down) {
            nguoiChoi1.reNhanh();
            amThanh.phatRe();
        }
    }

    bool nc2CoTheDiChuyen = (trangThai == TS::Choi2Nguoi ||
        trangThai == TS::DoiKhang) && !nguoiChoi2DaThua;

    if (nc2CoTheDiChuyen) {
        if (su_kien.key.code == KB::A) {
            nguoiChoi2.diChuyen(-1);
            amThanh.phatDiChuyen();
        }
        if (su_kien.key.code == KB::D) {
            nguoiChoi2.diChuyen(1);
            amThanh.phatDiChuyen();
        }
        if (su_kien.key.code == KB::W) {
            nguoiChoi2.xoay();
            amThanh.phatXoay();
        }
        if (su_kien.key.code == KB::S) {
            nguoiChoi2.reNhanh();
            amThanh.phatRe();
        }
    }
}

void TroChoi::veManHinhMenu() {
    for (int i = 0; i < 3; i++) {
        sf::CircleShape anhSang(120 + i * 30);
        anhSang.setFillColor(sf::Color(30 - i * 8, 30 - i * 8, 80 - i * 20, 40));
        anhSang.setPosition(300, 180);
        cuaSo.draw(anhSang);
    }

    veChuLen("XEP HINH", 240, 100, 68, sf::Color(80, 160, 255));
    sf::Color mauBT(200, 200, 220);
    veChuLen("[1]  1 Nguoi choi", 220, 225, 20, mauBT);
    veChuLen("[2]  2 Nguoi choi", 220, 258, 20, mauBT);
    veChuLen("[3]  Doi Khang", 220, 291, 20, sf::Color(255, 120, 80));
    veChuLen("[4]  Huong dan", 220, 324, 20, mauBT);
    veChuLen("[5]  Ky luc", 220, 357, 20, mauBT);
    veChuLen("[6]  Thoat", 220, 390, 20, mauBT);
    if (coFileLuu1P()) veChuLen("[7]  Tiep tuc 1P da luu", 220, 423, 20, sf::Color(100, 255, 150));
    if (coFileLuu2P()) veChuLen("[8]  Tiep tuc 2P da luu", 220, 453, 20, sf::Color(255, 160, 100));
    veChuLen("KY LUC: " + std::to_string(diemKyLuc), 270, 490, 18, sf::Color(255, 200, 0));

    std::string trangThaiAm = amThanh.isDacTat() ? "[M]  Am thanh: TAT" : "[M]  Am thanh: BAT";
    sf::Color   mauAm = amThanh.isDacTat() ? sf::Color(180, 80, 80) : sf::Color(80, 200, 120);
    veChuLen(trangThaiAm, 220, 520, 16, mauAm);
}

void TroChoi::veManHinhChonTocDo() {
    veChuLen("CHON MUC DO", 250, 140, 36, sf::Color(80, 160, 255));
    if (choDoiKhang)
        veChuLen("(Che do Doi Khang)", 260, 188, 16, sf::Color(255, 120, 80));
    veChuLen("[1]  DE", 220, 240, 22, sf::Color(100, 220, 100));
    veChuLen("[2]  TRUNG BINH", 220, 285, 22, sf::Color(220, 220, 50));
    veChuLen("[3]  KHO", 220, 330, 22, sf::Color(220, 100, 100));
    veChuLen("ESC  Quay lai menu", 240, 420, 16, sf::Color(150, 150, 180));
}

void TroChoi::veManHinhHuongDan() {
    veChuLen("HUONG DAN", 280, 60, 40, sf::Color(80, 160, 255));
    veChuLen("--- 1 NGUOI CHOI ---", 220, 130, 18, sf::Color(100, 220, 100));
    veChuLen("Arrow Left/Right : Di chuyen", 180, 162, 15, sf::Color::White);
    veChuLen("Arrow Up         : Xoay khoi", 180, 182, 15, sf::Color::White);
    veChuLen("Arrow Down       : Re nhanh", 180, 202, 15, sf::Color::White);
    veChuLen("--- 2 NGUOI CHOI ---", 220, 240, 18, sf::Color(220, 100, 100));
    veChuLen("Player 2: A D W S", 180, 268, 15, sf::Color::White);
    veChuLen("1 nguoi thua -> nguoi kia choi tiep!", 180, 290, 13, sf::Color(200, 200, 100));
    veChuLen("Ca 2 thua -> so diem -> xac dinh thang", 180, 308, 13, sf::Color(200, 200, 100));
    veChuLen("--- DOI KHANG (CHE DO 3) ---", 220, 345, 18, sf::Color(255, 120, 80));
    veChuLen("Xoa 1 hang -> gui 1 hang rac", 180, 372, 14, sf::Color::White);
    veChuLen("Xoa 2 hang -> gui 2 hang rac", 180, 390, 14, sf::Color::White);
    veChuLen("Xoa 3 hang -> gui 3 hang rac", 180, 408, 14, sf::Color::White);
    veChuLen("Xoa 4 hang -> gui 4 hang rac", 180, 426, 14, sf::Color::White);
    veChuLen("Hang rac co 1 lo ngau nhien!", 180, 444, 14, sf::Color(200, 180, 80));
    veChuLen("Nguoi thua truoc -> nguoi kia choi tiep", 180, 462, 13, sf::Color(200, 200, 100));
    veChuLen("--- DIEM SO ---", 240, 490, 18, sf::Color(255, 200, 0));
    veChuLen("1 hang=100 | 2 hang=300 | 3 hang=600 | TETRIS=1000", 140, 515, 13, sf::Color::White);
    veChuLen("[M] Tat/Bat am thanh", 240, 540, 14, sf::Color(150, 200, 255));
    veChuLen("ESC de quay lai", 285, 560, 16, sf::Color(150, 150, 180));
}

void TroChoi::veManHinhKyLuc() {
    veChuLen("KY LUC", 310, 140, 44, sf::Color(255, 200, 0));

    sf::RectangleShape khung(sf::Vector2f(300, 80));
    khung.setPosition(255, 230);
    khung.setFillColor(sf::Color(20, 20, 50, 220));
    khung.setOutlineThickness(2);
    khung.setOutlineColor(sf::Color(255, 200, 0));
    cuaSo.draw(khung);

    veChuLen("DIEM CAO NHAT", 278, 242, 16, sf::Color(200, 200, 220));
    veChuLen(std::to_string(diemKyLuc), 340, 268, 30, sf::Color(255, 220, 50));
    veChuLen("ESC de quay lai", 298, 370, 16, sf::Color(150, 150, 180));
}

void TroChoi::choi1Nguoi() {
    nguoiChoi1.capNhatHieuUng(dt);

    if (nguoiChoi1.isDaGameOver()) {
        if (!gameOverDaPhat1) {
            luuKyLuc(nguoiChoi1.getDiem());
            xoaFileLuu(CauHinh::FILE_LUU_1P);
            amThanh.phatGameOver();
            gameOverDaPhat1 = true;
        }
        sf::RectangleShape man(sf::Vector2f(820, CauHinh::HANG * CauHinh::O_SIZE + 60));
        man.setFillColor(sf::Color(0, 0, 0, 160));
        cuaSo.draw(man);
        veChuLen("GAME OVER", 250, 220, 44, sf::Color(220, 60, 60));
        veChuLen("Diem: " + std::to_string(nguoiChoi1.getDiem()), 290, 280, 24, sf::Color::White);
        veChuLen("ESC de quay lai", 275, 330, 18, sf::Color(150, 150, 180));
        return;
    }

    if (thoiGianTich > BANG_TOC_DO[mucTocDo]) {
        int soHang = nguoiChoi1.buocRoi(BAN_1P_X);
        if (soHang == 4)      amThanh.phatTetris();
        else if (soHang > 0)  amThanh.phatXoaHang();
        thoiGianTich = 0;
    }

    nguoiChoi1.veBanChoi(cuaSo, BAN_1P_X, phongChu);
    nguoiChoi1.vePanel(cuaSo, PANEL_1P_X, 20, phongChu, "NGUOI CHOI 1");
    veChuLen("Arrow: Di chuyen", PANEL_1P_X + 8, 230, 10, sf::Color(120, 120, 160));
    veChuLen("Up: Xoay", PANEL_1P_X + 8, 244, 10, sf::Color(120, 120, 160));
    veChuLen("Down: Re", PANEL_1P_X + 8, 258, 10, sf::Color(120, 120, 160));
    veChuLen("KY LUC", PANEL_1P_X + 8, 340, 12, sf::Color(160, 160, 200));
    veChuLen(std::to_string(diemKyLuc), PANEL_1P_X + 8, 358, 18, sf::Color(255, 200, 0));
    veChuLen("TOC DO: " + std::string(NHAN_TOC_DO[mucTocDo]), PANEL_1P_X + 8, 390, 11, sf::Color(100, 200, 255));

    std::string trangThaiAm = amThanh.isDacTat() ? "[M] Am: TAT" : "[M] Am: BAT";
    veChuLen(trangThaiAm, PANEL_1P_X + 8, 410, 10, sf::Color(150, 200, 150));

    nguoiChoi1.veHieuUngDiem(cuaSo, phongChu);
}

void TroChoi::choi2Nguoi() {
    nguoiChoi1.capNhatHieuUng(dt);
    nguoiChoi2.capNhatHieuUng(dt);

    if (nguoiChoi1.isDaGameOver() && !nguoiChoi1DaThua) {
        nguoiChoi1DaThua = true;
        if (!gameOverDaPhat1) {
            amThanh.phatGameOver();
            gameOverDaPhat1 = true;
        }
    }
    if (nguoiChoi2.isDaGameOver() && !nguoiChoi2DaThua) {
        nguoiChoi2DaThua = true;
        if (!gameOverDaPhat2) {
            amThanh.phatGameOver();
            gameOverDaPhat2 = true;
        }
    }

    if (nguoiChoi1DaThua && nguoiChoi2DaThua) {
        luuKyLuc(std::max(nguoiChoi1.getDiem(), nguoiChoi2.getDiem()));
        xoaFileLuu(CauHinh::FILE_LUU_2P);

        sf::RectangleShape man(sf::Vector2f(820, CauHinh::HANG * CauHinh::O_SIZE + 60));
        man.setFillColor(sf::Color(0, 0, 0, 160));
        cuaSo.draw(man);

        std::string tenNguoiThang;
        if (nguoiChoi1.getDiem() > nguoiChoi2.getDiem()) tenNguoiThang = "NGUOI CHOI 1 THANG!";
        else if (nguoiChoi2.getDiem() > nguoiChoi1.getDiem()) tenNguoiThang = "NGUOI CHOI 2 THANG!";
        else                                                   tenNguoiThang = "HOA!";

        veChuLen("GAME OVER", 280, 180, 40, sf::Color(220, 60, 60));
        veChuLen(tenNguoiThang, 230, 240, 28, sf::Color(255, 200, 0));
        veChuLen("P1: " + std::to_string(nguoiChoi1.getDiem()) +
            "  P2: " + std::to_string(nguoiChoi2.getDiem()), 230, 295, 20, sf::Color::White);
        veChuLen("ESC de quay lai", 295, 345, 16, sf::Color(150, 150, 180));
        return;
    }

    if (thoiGianTich > BANG_TOC_DO[mucTocDo]) {
        int soHang1 = 0, soHang2 = 0;
        if (!nguoiChoi1DaThua) soHang1 = nguoiChoi1.buocRoi(BAN_2P_1_X);
        if (!nguoiChoi2DaThua) soHang2 = nguoiChoi2.buocRoi(BAN_2P_2_X);
        if (soHang1 == 4)     amThanh.phatTetris();
        else if (soHang1 > 0) amThanh.phatXoaHang();
        if (soHang2 == 4)     amThanh.phatTetris();
        else if (soHang2 > 0) amThanh.phatXoaHang();
        thoiGianTich = 0;
    }

    nguoiChoi1.veBanChoi(cuaSo, BAN_2P_1_X, phongChu);
    nguoiChoi2.veBanChoi(cuaSo, BAN_2P_2_X, phongChu);
    nguoiChoi1.vePanel(cuaSo, PANEL_2P_1_X, 10, phongChu, "NGUOI CHOI 1");
    veChuLen("<- -> Di chuyen", PANEL_2P_1_X + 5, 218, 10, sf::Color(120, 120, 160));
    veChuLen("Up Xoay / Down Re", PANEL_2P_1_X + 5, 232, 10, sf::Color(120, 120, 160));
    nguoiChoi2.vePanel(cuaSo, PANEL_2P_2_X, 10, phongChu, "NGUOI CHOI 2");
    veChuLen("A D Di chuyen", PANEL_2P_2_X + 5, 218, 10, sf::Color(120, 120, 160));
    veChuLen("W Xoay / S Re", PANEL_2P_2_X + 5, 232, 10, sf::Color(120, 120, 160));
    veChuLen("TOC DO: " + std::string(NHAN_TOC_DO[mucTocDo]),
        330, CauHinh::HANG * CauHinh::O_SIZE + 10, 13, sf::Color(100, 200, 255));

    if (nguoiChoi1DaThua && !nguoiChoi2DaThua) veOverlayDaThua(BAN_2P_1_X, "Dang cho...");
    if (nguoiChoi2DaThua && !nguoiChoi1DaThua) veOverlayDaThua(BAN_2P_2_X, "Dang cho...");

    nguoiChoi1.veHieuUngDiem(cuaSo, phongChu);
    nguoiChoi2.veHieuUngDiem(cuaSo, phongChu);
}

void TroChoi::choiDoiKhang() {
    nguoiChoi1.capNhatHieuUng(dt);
    nguoiChoi2.capNhatHieuUng(dt);

    if (nguoiChoi1.isDaGameOver() && !nguoiChoi1DaThua) {
        nguoiChoi1DaThua = true;
        if (!gameOverDaPhat1) {
            amThanh.phatGameOver();
            gameOverDaPhat1 = true;
        }
    }
    if (nguoiChoi2.isDaGameOver() && !nguoiChoi2DaThua) {
        nguoiChoi2DaThua = true;
        if (!gameOverDaPhat2) {
            amThanh.phatGameOver();
            gameOverDaPhat2 = true;
        }
    }

    if (nguoiChoi1DaThua && nguoiChoi2DaThua) {
        luuKyLuc(std::max(nguoiChoi1.getDiem(), nguoiChoi2.getDiem()));

        sf::RectangleShape man(sf::Vector2f(820, CauHinh::HANG * CauHinh::O_SIZE + 60));
        man.setFillColor(sf::Color(0, 0, 0, 170));
        cuaSo.draw(man);

        std::string tenNguoiThang;
        if (nguoiChoi1.getDiem() > nguoiChoi2.getDiem()) tenNguoiThang = "NGUOI CHOI 1 THANG!";
        else if (nguoiChoi2.getDiem() > nguoiChoi1.getDiem()) tenNguoiThang = "NGUOI CHOI 2 THANG!";
        else                                                   tenNguoiThang = "HOA!";

        veChuLen("DOI KHANG KET THUC", 180, 180, 36, sf::Color(220, 60, 60));
        veChuLen(tenNguoiThang, 200, 240, 30, sf::Color(255, 200, 0));
        veChuLen("P1: " + std::to_string(nguoiChoi1.getDiem()) +
            "  P2: " + std::to_string(nguoiChoi2.getDiem()), 230, 295, 22, sf::Color::White);
        veChuLen("ESC de quay lai", 295, 360, 16, sf::Color(150, 150, 180));
        return;
    }

    if (thoiGianTich > BANG_TOC_DO[mucTocDo]) {
        int soHangNC1 = 0, soHangNC2 = 0;
        if (!nguoiChoi1DaThua) soHangNC1 = nguoiChoi1.buocRoi(BAN_DK_1_X);
        if (!nguoiChoi2DaThua) soHangNC2 = nguoiChoi2.buocRoi(BAN_DK_2_X);

        if (soHangNC1 == 4)     amThanh.phatTetris();
        else if (soHangNC1 > 0) amThanh.phatXoaHang();
        if (soHangNC2 == 4)     amThanh.phatTetris();
        else if (soHangNC2 > 0) amThanh.phatXoaHang();

        if (soHangNC1 > 0 && !nguoiChoi2DaThua) {
            nguoiChoi2.nhanHangRac(soHangNC1);
            amThanh.phatRac();
            float gx = BAN_DK_2_X + CauHinh::COT * CauHinh::O_SIZE / 2.f;
            HieuUngDiem hu(gx, 80, soHangNC1 * 100);
            hu.setNoiDung("+" + std::to_string(soHangNC1) + " RAC!");
            nguoiChoi2.getDanhSachHieuUng().push_back(hu);
        }
        if (soHangNC2 > 0 && !nguoiChoi1DaThua) {
            nguoiChoi1.nhanHangRac(soHangNC2);
            amThanh.phatRac();
            float gx = BAN_DK_1_X + CauHinh::COT * CauHinh::O_SIZE / 2.f;
            HieuUngDiem hu(gx, 80, soHangNC2 * 100);
            hu.setNoiDung("+" + std::to_string(soHangNC2) + " RAC!");
            nguoiChoi1.getDanhSachHieuUng().push_back(hu);
        }
        thoiGianTich = 0;
    }

    nguoiChoi1.veBanChoi(cuaSo, BAN_DK_1_X, phongChu);
    nguoiChoi2.veBanChoi(cuaSo, BAN_DK_2_X, phongChu);
    nguoiChoi1.vePanel(cuaSo, PANEL_DK_1_X, 10, phongChu, "P1");
    veChuLen("<- -> Di chuyen", PANEL_DK_1_X + 3, 210, 9, sf::Color(120, 120, 160));
    veChuLen("Up Xoay", PANEL_DK_1_X + 3, 223, 9, sf::Color(120, 120, 160));
    veChuLen("Down Re", PANEL_DK_1_X + 3, 236, 9, sf::Color(120, 120, 160));
    nguoiChoi2.vePanel(cuaSo, PANEL_DK_2_X, 10, phongChu, "P2");
    veChuLen("A D Di chuyen", PANEL_DK_2_X + 3, 210, 9, sf::Color(120, 120, 160));
    veChuLen("W Xoay / S Re", PANEL_DK_2_X + 3, 223, 9, sf::Color(120, 120, 160));
    veChuLen("DOI KHANG",
        330, CauHinh::HANG * CauHinh::O_SIZE + 5, 16, sf::Color(255, 100, 100));
    veChuLen("Xoa hang -> gui rac ngay!",
        255, CauHinh::HANG * CauHinh::O_SIZE + 25, 12, sf::Color(200, 200, 100));
    veChuLen("TOC DO: " + std::string(NHAN_TOC_DO[mucTocDo]),
        310, CauHinh::HANG * CauHinh::O_SIZE + 42, 11, sf::Color(100, 200, 255));

    if (nguoiChoi1DaThua && !nguoiChoi2DaThua) veOverlayDaThua(BAN_DK_1_X, "Dang cho P2...");
    if (nguoiChoi2DaThua && !nguoiChoi1DaThua) veOverlayDaThua(BAN_DK_2_X, "Dang cho P1...");

    veMuiTenDoiKhangGiua();
    nguoiChoi1.veHieuUngDiem(cuaSo, phongChu);
    nguoiChoi2.veHieuUngDiem(cuaSo, phongChu);
}

void TroChoi::chay() {
    while (cuaSo.isOpen()) {
        dt = dongHo.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        thoiGianTich += dt;

        sf::Event su_kien;
        while (cuaSo.pollEvent(su_kien)) {
            if (su_kien.type == sf::Event::Closed) {
                if (trangThai == TrangThaiGame::Choi1Nguoi) luuGame1Nguoi();
                if (trangThai == TrangThaiGame::Choi2Nguoi) luuGame2Nguoi();
                cuaSo.close();
            }
            if (su_kien.type == sf::Event::KeyPressed) {
                xuLyPhimNhan(su_kien);
            }
        }

        cuaSo.clear(sf::Color(10, 10, 22));

        switch (trangThai) {
        case TrangThaiGame::Menu:       veManHinhMenu();      break;
        case TrangThaiGame::ChonTocDo:  veManHinhChonTocDo(); break;
        case TrangThaiGame::Choi1Nguoi: choi1Nguoi();         break;
        case TrangThaiGame::Choi2Nguoi: choi2Nguoi();         break;
        case TrangThaiGame::DoiKhang:   choiDoiKhang();       break;
        case TrangThaiGame::HuongDan:   veManHinhHuongDan();  break;
        case TrangThaiGame::KyLuc:      veManHinhKyLuc();     break;
        }

        cuaSo.display();
    }
}