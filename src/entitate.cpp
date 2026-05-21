//
// Created by user on 15.05.2026.
//

#include "entitate.hpp"
std::vector<Entitate*> Entitate::entitati;

void Entitate::_inregistreaza_entitate(Entitate* e) {
    auto it = std::ranges::find(entitati.begin(), entitati.end(), e);
    if (it == entitati.end()) {
        entitati.emplace_back(e);
    }
}


Entitate::Entitate(float width, float height, float posX, float posY, float scale, float rotation) : scale(scale), rotation(rotation) {
    hitbox.SetPosition(posX, posY);
    hitbox.SetSize(width*scale, height*scale);
    _inregistreaza_entitate(this);
}

Entitate::Entitate(raylib::Rectangle hitbox, float scale, float rotation) : scale(scale), rotation(rotation) {
    this->hitbox.SetPosition(hitbox.x, hitbox.y);
    this->hitbox.SetSize(hitbox.width*scale, hitbox.height*scale);
    _inregistreaza_entitate(this);
}

Entitate::Entitate(const Entitate &other) :
hitbox(other.hitbox), scale(other.scale), rotation(other.rotation){
    _inregistreaza_entitate(this);
}

Entitate &Entitate::operator=(const Entitate &other) {
    if (&other != this) {
        hitbox = other.hitbox;
        scale = other.scale;
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

void Entitate::SetPosition(float x, float y) {
    hitbox.SetPosition(x, y);
}

raylib::Rectangle Entitate::GetHitbox() const { return hitbox; }

float Entitate::GetRotation() const{ return rotation; }

void Entitate::Draw() {
    raylib::Vector2 centru = {hitbox.width/2, hitbox.height/2};
    _draw(centru);
}

void Entitate::SetRotation(float r) {rotation = r;}
