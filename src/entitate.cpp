//
// Created by user on 15.05.2026.
//

#include "entitate.hpp"
std::vector<Entitate*> Entitate::entitati;
float Entitate::factor_scalare = 1.0f;

void Entitate::_inregistreaza_entitate(Entitate* e) {
    auto it = std::ranges::find(entitati.begin(), entitati.end(), e);
    if (it == entitati.end()) {
        entitati.emplace_back(e);
    }
}

void Entitate::aplica_scalare() {
    hitbox.SetSize(latime_baza * scale * factor_scalare, inaltime_baza * scale * factor_scalare);
}

void Entitate::set_dimensiuni_baza(float w, float h) {
    latime_baza = w;
    inaltime_baza = h;
    aplica_scalare();
}

void Entitate::set_factor_scalare(float f) {
    factor_scalare = f;
    for (auto* e : entitati) e->aplica_scalare();
}

Entitate::Entitate(float width, float height, float posX, float posY, float scale, float rotation)
    : scale(scale), latime_baza(width), inaltime_baza(height), rotation(rotation) {
    hitbox.SetPosition(posX, posY);
    aplica_scalare();
    _inregistreaza_entitate(this);
}

Entitate::Entitate(raylib::Rectangle hitbox, float scale, float rotation)
    : scale(scale), latime_baza(hitbox.width), inaltime_baza(hitbox.height), rotation(rotation) {
    this->hitbox.SetPosition(hitbox.x, hitbox.y);
    aplica_scalare();
    _inregistreaza_entitate(this);
}

Entitate::Entitate(const Entitate &other) :
hitbox(other.hitbox), scale(other.scale), latime_baza(other.latime_baza),
inaltime_baza(other.inaltime_baza), rotation(other.rotation){
    _inregistreaza_entitate(this);
}

Entitate &Entitate::operator=(const Entitate &other) {
    if (&other != this) {
        hitbox = other.hitbox;
        scale = other.scale;
        latime_baza = other.latime_baza;
        inaltime_baza = other.inaltime_baza;
        rotation = other.rotation;
        _inregistreaza_entitate(this);
    }
    return *this;
}

Entitate::~Entitate() {
    std::erase(entitati, this);
}


void Entitate::MoveWith(float dx, float dy) {
    hitbox.x += dx;
    hitbox.y += dy;
}
bool Entitate::GetCollision(Entitate& other) {
    if (fizica::VerColiziune(hitbox, rotation, other.GetHitbox(), other.GetRotation())) {
        OnCollision(other);
        return true;
    }
    return false;
}

bool Entitate::GetCollision(Sageata &s) {
    if (fizica::VerColiziune(hitbox, rotation, s.get_hitbox(), s.get_rotation())) {
        OnCollision(s);
        return true;
    }
    return false;
}

void Entitate::SetPosition(float x, float y) {
    hitbox.SetPosition(x, y);
}

raylib::Rectangle Entitate::GetHitbox() const { return hitbox; }

float Entitate::GetRotation() const{ return rotation; }

void Entitate::Draw() {
    raylib::Vector2 centru = {hitbox.width/2, hitbox.height/2};
    _draw(centru);
}

const std::vector<Entitate *>& Entitate::get_entitati() {
    return entitati;
}
