#include "bloc.hpp"
#include "sageata.hpp"
#include "caracter.hpp"

Bloc::Bloc(float Width, float Height) : Entitate(Width, Height, 0, 0) {}

Bloc::Bloc(float posX, float posY, float Width, float Height)
    : Entitate(Width, Height, posX, posY) {}

Bloc::Bloc(float posX, float posY, float Width, float Height, const Caracter& owner)
    : Entitate(Width, Height, posX, posY), owner(&owner) {
    lifespan = owner.durabilitate_zid();
    dx_owner = (posX - owner.GetHitbox().x) / Entitate::get_factor_scalare();
}

void Bloc::Recalibreaza() {
    if (owner == nullptr) return;
    const float factor = Entitate::get_factor_scalare();
    const float x = owner->GetHitbox().x + dx_owner * factor;
    const float y = owner->GetHitbox().y + owner->GetHitbox().height - hitbox.height;
    SetPosition(x, y);
}

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

void Bloc::OnCollision(Entitate&) {
    lifespan = 0;
}

void Bloc::OnCollision(Sageata& s) {
    lifespan -= s.damage_perete();
}

bool Bloc::TrebuieSters() const {
    return lifespan <= 0;
}

std::ostream& operator<<(std::ostream& os, const Bloc& b) {
    return os << "Pozitie: " << '(' << b.hitbox.x << ", " << b.hitbox.y << ")\n" <<
                 "Marimi: " << '(' << b.hitbox.width << ", " << b.hitbox.height << ")" << std::endl;
}