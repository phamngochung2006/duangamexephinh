#include <iostream>
#include <stdexcept>
#include "trochoi.h"

int main() {
    try {
        TroChoi tro_choi;
        tro_choi.chay();
    }
    catch (const std::exception& e) {
        std::cerr << "[LOI NGHIEM TRONG] " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "[LOI NGHIEM TRONG] Ngoai le khong xac dinh\n";
        return 2;
    }
    return 0;
}