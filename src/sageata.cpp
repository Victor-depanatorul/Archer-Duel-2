#include "sageata.hpp"
#include "entitate.hpp"
#include "caracter.hpp"

Sageata::Sageata(float posX, float posY, float latime, float inaltime)
    : hitbox(posX, posY, latime, inaltime) {}

void Sageata::afiseaza(std::ostream& os) const {
    os << "Sageata[" << nume() << "] la (" << hitbox.x << ", " << hitbox.y << ")";
}

void Sageata::Draw() const {
    raylib::Rectangle dest{ hitbox.x + hitbox.width / 2.0f, hitbox.y + hitbox.height / 2.0f,
                            hitbox.width, hitbox.height };
    raylib::Vector2 origin{ hitbox.width / 2.0f, hitbox.height / 2.0f };
    dest.Draw(origin, rotation, get_color());
}

void Sageata::SetVelocity(raylib::Vector2 tintaMouse, float forta, const Caracter*) {
    raylib::Vector2 centru{hitbox.x + hitbox.width / 2.0f, hitbox.y + hitbox.height / 2.0f};
    float unghi = std::atan2(tintaMouse.y - centru.y, tintaMouse.x - centru.x);
    viteza = raylib::Vector2( std::cos(unghi) * forta, std::sin(unghi) * forta );
    rotation = unghi * (180.0f / PI);
}

void Sageata::lanseaza(Caracter& cine, raylib::Vector2 tintaMouse, float forta, const Caracter* inamic) {
    tragator = &cine;
    raylib::Rectangle h = cine.GetHitbox();
    hitbox.x = (h.x + h.width  / 2.0f) - hitbox.width  / 2.0f;
    hitbox.y = (h.y + h.height / 2.0f) - hitbox.height / 2.0f;
    SetVelocity(tintaMouse, forta, inamic);
}

void Sageata::update(float dt, const std::vector<Entitate*>& obstacole) {
    if (tragator == nullptr) return;

    if (!armata && !fizica::VerColiziune(tragator->GetHitbox(), 0.0f, hitbox, rotation))
        armata = true;

    viteza.y += fizica::gravitate * dt;
    rotation = std::atan2(viteza.y, viteza.x) * (180.0f / PI);
    hitbox.x += viteza.x * dt;
    hitbox.y += viteza.y * dt;

    for (Entitate* e : obstacole) {
        if (e == static_cast<Entitate*>(tragator) && !armata) continue;
        if (e->GetCollision(*this)) {
            trebuie_distrusa = true;
            a_nimerit = true;
            break;
        }
    }

    float podea = tragator->GetHitbox().y + tragator->GetHitbox().height + 10.0f;
    if (hitbox.y > podea) trebuie_distrusa = true;
    if (trebuie_distrusa && !a_nimerit) la_distrugere_nenimerit();
    else if (a_nimerit) tragator->stats_nimerita();
}
