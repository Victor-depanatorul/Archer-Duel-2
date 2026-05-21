//
// Created by user on 13.05.2026.
//
#include "sageata.hpp"

Sageata::Sageata(tipSageti tip, float posX, float posY) :
Entitate(raylib::Rectangle{posX, posY, 40, 20}), tip(tip) {
    float w = tip != Giganta ? hitbox.width : 2*hitbox.height;
    float h = tip != Giganta ? hitbox.height : 2*hitbox.height;
    hitbox.width = w;
    hitbox.height = h;
}

tipSageti Sageata::get_tip() const {return tip;}

raylib::Vector2 Sageata::get_pos() const {return hitbox.GetPosition();}

float Sageata::get_damage() const{
    {
        if (tip!=tipSageti::Invalid)
            return damage.at(tip);
        return 0;
    }
}

Color Sageata::get_color() const {
    {
        if (tip!=tipSageti::Invalid)
            return culori.at(tip);
        return WHITE;
    }
}

// raylib::Vector2 Sageata::get_velocity() const {return viteza;}


bool Sageata::este_veche() const {return veche;}

void Sageata::SetPosition(float x, float y) {
    hitbox.SetPosition(x, y);
    if (init_pos.x == -1.0f && init_pos.y == -1.0f) init_pos = raylib::Vector2{x, y};
}
void Sageata::UpdateVelocity(float vitezaScala) {
    float rad = rotation * (PI / 180.0f);
    viteza.x = cosf(rad) * vitezaScala;
    viteza.y = sinf(rad) * vitezaScala;
}

void Sageata::UpdateImunitate(const Entitate& e) {
    if (este_veche()) return;
    if (!fizica::VerColiziune(e.GetHitbox(), e.GetRotation(),
        this->GetHitbox(), this->GetRotation())) veche = true;
}

void Sageata::Update(float deltaTime, const Entitate& e) {
    viteza.y += fizica::gravitate * deltaTime;
    float rad = atan2f(viteza.y, viteza.x);
    rotation = rad * (180.0f / PI);
    MoveWith(viteza.x * deltaTime, viteza.y * deltaTime);
    UpdateImunitate(e);
}

void Sageata::_draw(raylib::Vector2 centru) {
    hitbox.Draw(centru, rotation, this->get_color());
}


void Sageata::OnCollision(Entitate&) {veche = true;}

std::ostream& operator<< (std::ostream& os, const Sageata& s) {
    os << "Pozitia: (" << s.hitbox.x << ", " << s.hitbox.y << ")" << '\n';
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

