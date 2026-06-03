//
// Created by user on 02.06.2026.
//

#include "statistici.hpp"
#include <string>

float Statistici::acuratete() const {
    if (sageti_trase == 0) return 0.0f;
    return static_cast<float>(sageti_nimerite) / static_cast<float>(sageti_trase);
}

void Statistici::Deseneaza(int x, int y, Color culoare, std::string&& aditional) const {
    constexpr int line_h = 25;
    DrawText(aditional.c_str(), x, y, 20, culoare);
    DrawText(("Sageti trase: " + std::to_string(sageti_trase)).c_str(),
             x, y + line_h, 20, culoare);
    DrawText(("Sageti nimerite: " + std::to_string(sageti_nimerite)).c_str(),
             x, y + 2*line_h, 20, culoare);
    DrawText(("Damage dat: " + std::to_string(static_cast<int>(damage_dat))).c_str(),
             x, y + 3*line_h, 20, culoare);
    DrawText(("Power-ups luate: " + std::to_string(powerups_luate)).c_str(),
             x, y + 4*line_h, 20, culoare);
    std::string acur = (sageti_trase > 0)
        ? "Acuratete: " + std::to_string(static_cast<int>(acuratete() * 100.0f)) + "%"
        : std::string("Acuratete: -");
    DrawText(acur.c_str(), x, y + 5*line_h, 20, culoare);
}

void Statistici::reset() {
    sageti_trase = 0;
    damage_dat = 0.0f;
    powerups_luate = 0;
    sageti_nimerite = 0;
}

std::ostream& operator<<(std::ostream &os, const Statistici &obj){
    return os
           << "sageti_trase: " << obj.sageti_trase
           << " damage_dat: " << obj.damage_dat
           << " powerups_luate: " << obj.powerups_luate
           << " acuratete: " << obj.acuratete();
}


