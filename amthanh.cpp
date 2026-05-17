#include "amthanh.h"
#include <cmath>
#include <algorithm>

static constexpr float PI = 3.14159265358979f;
static constexpr unsigned int MAU_PER_GIAY = 44100;

// Tên file nhạc nền
static constexpr const char* NHAC_NEN_FILE = "darren_hirst-gamer-music-140-bpm-355954.wav";

std::vector<sf::Int16> AmThanh::taoSinWave(float tanSo, float thoiGian, unsigned int mauPerGiay, float doGiam) {
    int soMau = static_cast<int>(mauPerGiay * thoiGian);
    std::vector<sf::Int16> mau(soMau);
    for (int i = 0; i < soMau; i++) {
        float t = static_cast<float>(i) / mauPerGiay;
        float envelope = 1.0f;
        if (doGiam > 0.0f) {
            envelope = std::max(0.0f, 1.0f - (t / thoiGian) * doGiam);
        }
        mau[i] = static_cast<sf::Int16>(32767 * 0.5f * std::sin(2.0f * PI * tanSo * t) * envelope);
    }
    return mau;
}

std::vector<sf::Int16> AmThanh::taoNoise(float thoiGian, unsigned int mauPerGiay, float doGiam) {
    int soMau = static_cast<int>(mauPerGiay * thoiGian);
    std::vector<sf::Int16> mau(soMau);
    for (int i = 0; i < soMau; i++) {
        float t = static_cast<float>(i) / mauPerGiay;
        float envelope = std::max(0.0f, 1.0f - (t / thoiGian));
        float n = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
        mau[i] = static_cast<sf::Int16>(32767 * doGiam * n * envelope);
    }
    return mau;
}

std::vector<sf::Int16> AmThanh::taoChord(float tan1, float tan2, float thoiGian, unsigned int mauPerGiay) {
    int soMau = static_cast<int>(mauPerGiay * thoiGian);
    std::vector<sf::Int16> mau(soMau);
    for (int i = 0; i < soMau; i++) {
        float t = static_cast<float>(i) / mauPerGiay;
        float envelope = std::max(0.0f, 1.0f - (t / thoiGian));
        float s = 0.35f * std::sin(2.0f * PI * tan1 * t)
            + 0.35f * std::sin(2.0f * PI * tan2 * t);
        mau[i] = static_cast<sf::Int16>(32767 * s * envelope);
    }
    return mau;
}

bool AmThanh::khoiTaoBuffer(sf::SoundBuffer& buffer, const std::vector<sf::Int16>& mau, unsigned int mauPerGiay) {
    return buffer.loadFromSamples(mau.data(), mau.size(), 1, mauPerGiay);
}

AmThanh::AmThanh() : dacTat(false) {
    // --- Hiệu ứng: Xóa hàng ---
    {
        int soMau = static_cast<int>(MAU_PER_GIAY * 0.25f);
        std::vector<sf::Int16> mau(soMau);
        float tanSoCo = 523.25f;
        for (int i = 0; i < soMau; i++) {
            float t = static_cast<float>(i) / MAU_PER_GIAY;
            float phanDoan = t / 0.25f;
            float tanSo = tanSoCo + phanDoan * 200.0f;
            float envelope = std::max(0.0f, 1.0f - phanDoan * 0.3f);
            mau[i] = static_cast<sf::Int16>(32767 * 0.5f * std::sin(2.0f * PI * tanSo * t) * envelope);
        }
        khoiTaoBuffer(bufferXoaHang, mau, MAU_PER_GIAY);
        soundXoaHang.setBuffer(bufferXoaHang);
        soundXoaHang.setVolume(70.f);
    }

    // --- Hiệu ứng: Tetris (xóa 4 hàng) ---
    {
        int soMau = static_cast<int>(MAU_PER_GIAY * 0.8f);
        std::vector<sf::Int16> mau(soMau);
        float chuoi[] = { 523.25f, 659.25f, 783.99f, 1046.50f };
        int buoc = soMau / 4;
        for (int seg = 0; seg < 4; seg++) {
            float tanSo = chuoi[seg];
            for (int i = 0; i < buoc && (seg * buoc + i) < soMau; i++) {
                float t = static_cast<float>(i) / MAU_PER_GIAY;
                float envelope = std::max(0.0f, 1.0f - static_cast<float>(i) / buoc * 0.4f);
                mau[seg * buoc + i] = static_cast<sf::Int16>(
                    32767 * 0.55f * std::sin(2.0f * PI * tanSo * t) * envelope);
            }
        }
        khoiTaoBuffer(bufferTetris, mau, MAU_PER_GIAY);
        soundTetris.setBuffer(bufferTetris);
        soundTetris.setVolume(85.f);
    }

    // --- Hiệu ứng: Rơi (drop) ---
    {
        auto mau = taoNoise(0.08f, MAU_PER_GIAY, 0.45f);
        khoiTaoBuffer(bufferRe, mau, MAU_PER_GIAY);
        soundRe.setBuffer(bufferRe);
        soundRe.setVolume(50.f);
    }

    // --- Hiệu ứng: Xoay ---
    {
        auto mau = taoSinWave(440.0f, 0.06f, MAU_PER_GIAY, 2.0f);
        khoiTaoBuffer(bufferXoay, mau, MAU_PER_GIAY);
        soundXoay.setBuffer(bufferXoay);
        soundXoay.setVolume(40.f);
    }

    // --- Hiệu ứng: Di chuyển ---
    {
        auto mau = taoSinWave(220.0f, 0.04f, MAU_PER_GIAY, 3.0f);
        khoiTaoBuffer(bufferDiChuyen, mau, MAU_PER_GIAY);
        soundDiChuyen.setBuffer(bufferDiChuyen);
        soundDiChuyen.setVolume(30.f);
    }

    // --- Hiệu ứng: Game Over ---
    {
        int soMau = static_cast<int>(MAU_PER_GIAY * 1.0f);
        std::vector<sf::Int16> mau(soMau);
        for (int i = 0; i < soMau; i++) {
            float t = static_cast<float>(i) / MAU_PER_GIAY;
            float tanSo = 200.0f * std::max(0.1f, 1.0f - t * 0.8f);
            float envelope = std::max(0.0f, 1.0f - t / 1.0f);
            float n = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
            mau[i] = static_cast<sf::Int16>(32767 * 0.4f *
                (0.5f * std::sin(2.0f * PI * tanSo * t) + 0.5f * n) * envelope);
        }
        khoiTaoBuffer(bufferGameOver, mau, MAU_PER_GIAY);
        soundGameOver.setBuffer(bufferGameOver);
        soundGameOver.setVolume(80.f);
    }

    // --- Hiệu ứng: Rác (garbage) ---
    {
        auto mau = taoChord(110.0f, 146.83f, 0.15f, MAU_PER_GIAY);
        khoiTaoBuffer(bufferRac, mau, MAU_PER_GIAY);
        soundRac.setBuffer(bufferRac);
        soundRac.setVolume(60.f);
    }

    // --- Nhạc nền: load từ file WAV ---
    if (nhacNen.openFromFile(NHAC_NEN_FILE)) {
        nhacNen.setLoop(true);
        nhacNen.setVolume(35.f);
    }
}

void AmThanh::batNhacNen() {
    if (!dacTat && nhacNen.getStatus() != sf::Music::Playing) {
        nhacNen.play();
    }
}

void AmThanh::dungNhacNen() {
    nhacNen.pause();
}

void AmThanh::phatXoaHang() {
    if (!dacTat) {
        soundXoaHang.stop();
        soundXoaHang.play();
    }
}

void AmThanh::phatTetris() {
    if (!dacTat) {
        soundTetris.stop();
        soundTetris.play();
    }
}

void AmThanh::phatRe() {
    if (!dacTat) {
        soundRe.stop();
        soundRe.play();
    }
}

void AmThanh::phatXoay() {
    if (!dacTat) {
        soundXoay.stop();
        soundXoay.play();
    }
}

void AmThanh::phatDiChuyen() {
    if (!dacTat) {
        soundDiChuyen.stop();
        soundDiChuyen.play();
    }
}

void AmThanh::phatGameOver() {
    if (!dacTat) {
        nhacNen.pause();
        soundGameOver.stop();
        soundGameOver.play();
    }
}

void AmThanh::phatRac() {
    if (!dacTat) {
        soundRac.stop();
        soundRac.play();
    }
}

void AmThanh::tatAmThanh() {
    dacTat = true;
    nhacNen.pause();
}

void AmThanh::batAmThanh() {
    dacTat = false;
    batNhacNen();
}

bool AmThanh::isDacTat() const {
    return dacTat;
}

void AmThanh::setAmLuong(float mucAm) {
    nhacNen.setVolume(mucAm * 35.f);
    soundXoaHang.setVolume(mucAm * 70.f);
    soundTetris.setVolume(mucAm * 85.f);
    soundRe.setVolume(mucAm * 50.f);
    soundXoay.setVolume(mucAm * 40.f);
    soundDiChuyen.setVolume(mucAm * 30.f);
    soundGameOver.setVolume(mucAm * 80.f);
    soundRac.setVolume(mucAm * 60.f);
}