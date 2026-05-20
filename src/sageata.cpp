//
// Created by user on 13.05.2026.
//
#include "sageata.hpp"

Sageata::Sageata(tipSageti tip, float posX, float posY) : tip(tip), pos(posX, posY) {}

tipSageti Sageata::get_tip() const {return tip;}

raylib::Vector2 Sageata::get_pos() const {return pos;}

float Sageata::get_damage(tipSageti tip) {
    {
        if (tip!=tipSageti::Invalid)
            return damage.at(tip);
        return 0;
    }
}

Color Sageata::get_color(tipSageti tip) {
    {
        if (tip!=tipSageti::Invalid)
            return culori.at(tip);
        return WHITE;
    }
}

raylib::Vector2 Sageata::get_velocity() const {return vel;}

void Sageata::CresteVarsta() {varsta+=0.1f;}

bool Sageata::este_veche() const {return varsta>=2.0f;}

void Sageata::UpdateVelocity(float dx, float dy) {vel.x+=dx, vel.y+=dy;}

void Sageata::MutaSageata(float dx, float dy) {pos.x=dx, pos.y=dy;}

void Sageata::MiscaSageata(float dx, float dy) {pos.x+=dx, pos.y+=dy;}

std::ostream& operator<< (std::ostream& os, const Sageata& s) {
    os << "Pozitia: (" << s.pos.x << ", " << s.pos.y << ")" << '\n';
    os << "Tipul sagetii: ";
    switch (s.tip) {
        case Normala:
            os << "Normala";
            break;
        case Otravitoare:
            os << "Otravitoare";
            break;
        case Aimbot:
            os << "Aimbot";
            break;
        case Healing:
            os << "Healing";
            break;
        case Giganta:
            os << "Giganta";
            break;
        case LifeSteal:
            os << "LifeSteal";
            break;
        default:
            os << "Tip necunoscut!";
            break;
    }
    os << std::endl;
    return os;
}

