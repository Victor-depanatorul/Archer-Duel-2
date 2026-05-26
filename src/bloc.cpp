#include "bloc.hpp"
#include "sageata.hpp"

Bloc::Bloc(float Width, float Height) : Entitate(Width, Height, 0, 0) {}

Bloc::Bloc(float posX, float posY, float Width, float Height)
    : Entitate(Width, Height, posX, posY) {}

void Bloc::Update(float dt) {
    if (progres_constructie < 1.0f) {
        progres_constructie += viteza_constructie * dt;
        if (progres_constructie > 1.0f) progres_constructie = 1.0f;
    }
}

void Bloc::_draw(raylib::Vector2) {
    float inaltime_curenta = hitbox.height * progres_constructie;
    float y_pornire_desen = hitbox.y + (hitbox.height - inaltime_curenta);
    raylib::Rectangle dreptunghi_animat(hitbox.x, y_pornire_desen, hitbox.width, inaltime_curenta);
    dreptunghi_animat.Draw(raylib::Color(120, 120, 120, 200));
    dreptunghi_animat.DrawLines(DARKGRAY);
}

void Bloc::OnCollision(Entitate& other) {
    float w = other.GetHitbox().width, h = other.GetHitbox().height;
    raylib::Vector2 origin = {w/2.0f, h/2.0f};
    other.GetHitbox().Draw(origin, rotation, RED);
}

bool Bloc::GetCollision(Sageata &s) {
    if (fizica::VerColiziune(s.get_hitbox(), s.get_rotation(), hitbox, rotation)) {
        lifespan=0;
        return true;
    }
    return false;
}

bool Bloc::TrebuieSters() const {
    return lifespan <= 0;
}

std::ostream& operator<<(std::ostream& os, const Bloc& b) {
    return os << "Pozitie: " << '(' << b.hitbox.x << ", " << b.hitbox.y << ")\n" <<
                 "Marimi: " << '(' << b.hitbox.width << ", " << b.hitbox.height << ")" << std::endl;
}