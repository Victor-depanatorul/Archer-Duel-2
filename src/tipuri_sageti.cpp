#include "tipuri_sageti.hpp"
#include "caracter.hpp"
#include "exceptii.hpp"
#include <cmath>

std::unique_ptr<Sageata> SageataNormala::clone() const {
    return std::make_unique<SageataNormala>(*this);
}
void SageataNormala::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(7.5f);
}
Color SageataNormala::get_color() const { return BLUE; }
std::string SageataNormala::nume() const { return "Normala"; }

std::unique_ptr<Sageata> SageataOtravitoare::clone() const {
    return std::make_unique<SageataOtravitoare>(*this);
}
void SageataOtravitoare::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(4.0f);
    tinta.AplicaOtrava(3);
}
Color SageataOtravitoare::get_color() const { return VIOLET; }
std::string SageataOtravitoare::nume() const { return "Otravitoare"; }

std::unique_ptr<Sageata> SageataHealing::clone() const {
    return std::make_unique<SageataHealing>(*this);
}
void SageataHealing::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(-7.5f);
}
Color SageataHealing::get_color() const { return GREEN; }
std::string SageataHealing::nume() const { return "Healing"; }

std::unique_ptr<Sageata> SageataLifeSteal::clone() const {
    return std::make_unique<SageataLifeSteal>(*this);
}
void SageataLifeSteal::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(7.5f);
    tragator->IaDamage(-7.5f);
}
Color SageataLifeSteal::get_color() const { return RED; }
std::string SageataLifeSteal::nume() const { return "LifeSteal"; }

SageataGiganta::SageataGiganta(float posX, float posY)
    : Sageata(posX, posY, 80.0f, 40.0f) {}

std::unique_ptr<Sageata> SageataGiganta::clone() const {
    return std::make_unique<SageataGiganta>(*this);
}
void SageataGiganta::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(15.0f);
}
Color SageataGiganta::get_color() const { return DARKBLUE; }
std::string SageataGiganta::nume() const { return "Giganta"; }

void SageataAimbot::SetVelocity(raylib::Vector2 tintaMouse, float forta, const Caracter* inamic) {
    if (inamic == nullptr) {
        Sageata::SetVelocity(tintaMouse, forta, nullptr);
        return;
    }
    raylib::Rectangle t = inamic->GetHitbox();
    raylib::Vector2 tinta{ t.x + t.width / 2.0f, t.y + t.height / 2.0f };
    float dx = tinta.x - hitbox.x;
    float dy = hitbox.y - tinta.y;
    float g = fizica::gravitate;

    // forța minimă pentru ca discriminantul balistic să fie >= 0 (adică să se poată atinge ținta)
    float forta_min = std::sqrt(g * (dy + std::sqrt(dx * dx + dy * dy)));
    if (forta < forta_min) forta = forta_min + 1.0f;   // boost ca să nimerească mereu

    float v2 = forta * forta;
    float radical = v2 * v2 - g * (g * dx * dx + 2.0f * dy * v2);
    if (radical < 0.0f) radical = 0.0f;                // protecție numerică
    float unghiBalistic = std::atan2(v2 + std::sqrt(radical), g * dx);
    float directie = (dx >= 0.0f) ? 1.0f : -1.0f;
    viteza = raylib::Vector2{ std::abs(std::cos(unghiBalistic) * forta) * directie,
                              -std::sin(unghiBalistic) * forta };
    float unghi = std::atan2(viteza.y, viteza.x);
    rotation = unghi * (180.0f / PI);
}

std::unique_ptr<Sageata> SageataAimbot::clone() const {
    return std::make_unique<SageataAimbot>(*this);
}
void SageataAimbot::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(5.0f);
}
Color SageataAimbot::get_color() const { return BLACK; }
std::string SageataAimbot::nume() const { return "Aimbot"; }
SageataBurn::SageataBurn(float posX, float posY) : Sageata(posX, posY) {}

std::unique_ptr<Sageata> SageataBurn::clone() const {
    return std::make_unique<SageataBurn>(*this);
}
void SageataBurn::aplica_efect(Caracter& tinta) const {
    tinta.AplicaBurn(2);
}
Color SageataBurn::get_color() const { return ORANGE; }
std::string SageataBurn::nume() const { return "Burn"; }

SageataGlassCannon::SageataGlassCannon(float posX, float posY) : Sageata(posX, posY,  100.0f, 50.0f) {}

void SageataGlassCannon::aplica_efect(Caracter &tinta) const {
    tinta.IaDamage(30.0f);
}

std::unique_ptr<Sageata> SageataGlassCannon::clone() const {
    return std::make_unique<SageataGlassCannon>(*this);
}

Color SageataGlassCannon::get_color() const {
    return DARKGRAY;
}

std::string SageataGlassCannon::nume() const {
    return "Glass Cannon";
}

std::unique_ptr<Sageata> creeaza_sageata(tipSageti tip, float x, float y) {
    switch (tip) {
        case Normala:     return std::make_unique<SageataNormala>(x, y);
        case Otravitoare: return std::make_unique<SageataOtravitoare>(x, y);
        case Aimbot:      return std::make_unique<SageataAimbot>(x, y);
        case Healing:     return std::make_unique<SageataHealing>(x, y);
        case Giganta:     return std::make_unique<SageataGiganta>(x, y);
        case LifeSteal:   return std::make_unique<SageataLifeSteal>(x, y);
        case Burn:        return std::make_unique<SageataBurn>(x, y);
        case GlassCannon: return std::make_unique<SageataGlassCannon>(x, y);
        default:          throw eroare_sageti(static_cast<int>(tip));
    }
}
