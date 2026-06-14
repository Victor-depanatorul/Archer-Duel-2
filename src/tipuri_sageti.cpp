#include "tipuri_sageti.hpp"
#include "caracter.hpp"
#include "exceptii.hpp"
#include "factory.hpp"
#include <cmath>

SageataNormala::SageataNormala(float posX, float posY)
    : Sageata(posX, posY, 40.0f, 20.0f, 7.5f) {}

std::unique_ptr<Sageata> SageataNormala::clone() const {
    return std::make_unique<SageataNormala>(*this);
}
void SageataNormala::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(dmg, tragator->get_dmg_multiplier());
}
Color SageataNormala::get_color() const { return BLUE; }
std::string SageataNormala::nume() const { return "Normala"; }

SageataOtravitoare::SageataOtravitoare(float posX, float posY)
    : Sageata(posX, posY, 40.0f, 20.0f, 4.0f) {}

std::unique_ptr<Sageata> SageataOtravitoare::clone() const {
    return std::make_unique<SageataOtravitoare>(*this);
}
void SageataOtravitoare::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(dmg, tragator->get_dmg_multiplier());
    tinta.AplicaOtrava(3);
}
Color SageataOtravitoare::get_color() const { return VIOLET; }
std::string SageataOtravitoare::nume() const { return "Otravitoare"; }

SageataHealing::SageataHealing(float posX, float posY)
    : Sageata(posX, posY, 40.0f, 20.0f, -7.5f) {}

std::unique_ptr<Sageata> SageataHealing::clone() const {
    return std::make_unique<SageataHealing>(*this);
}
void SageataHealing::aplica_efect(Caracter& tinta) const {
    tinta.Heal(-dmg * tragator->get_dmg_multiplier());
}
Color SageataHealing::get_color() const { return GREEN; }
std::string SageataHealing::nume() const { return "Healing"; }

SageataLifeSteal::SageataLifeSteal(float posX, float posY)
    : Sageata(posX, posY, 40.0f, 20.0f, 7.5f) {}

std::unique_ptr<Sageata> SageataLifeSteal::clone() const {
    return std::make_unique<SageataLifeSteal>(*this);
}
void SageataLifeSteal::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(dmg, tragator->get_dmg_multiplier());
    tragator->Heal(dmg * tragator->get_dmg_multiplier());
}
Color SageataLifeSteal::get_color() const { return RED; }
std::string SageataLifeSteal::nume() const { return "LifeSteal"; }

SageataGiganta::SageataGiganta(float posX, float posY)
    : Sageata(posX, posY, 80.0f, 40.0f, 15.0f) {}

std::unique_ptr<Sageata> SageataGiganta::clone() const {
    return std::make_unique<SageataGiganta>(*this);
}
void SageataGiganta::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(dmg, tragator->get_dmg_multiplier());
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

SageataAimbot::SageataAimbot(float posX, float posY)
    : Sageata(posX, posY, 40.0f, 20.0f, 5.0f) {}

std::unique_ptr<Sageata> SageataAimbot::clone() const {
    return std::make_unique<SageataAimbot>(*this);
}
void SageataAimbot::aplica_efect(Caracter& tinta) const {
    tinta.IaDamage(dmg, tragator->get_dmg_multiplier());
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

SageataGlassCannon::SageataGlassCannon(float posX, float posY) : Sageata(posX, posY,  100.0f, 50.0f, 30.0f) {}

void SageataGlassCannon::aplica_efect(Caracter &tinta) const {
    tinta.IaDamage(dmg, tragator->get_dmg_multiplier());
}

void SageataGlassCannon::la_distrugere_nenimerit() const {
    tragator->IaDamage(20.0f, 1.0f);
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

SageataRandom::SageataRandom(float posX, float posY) : Sageata(posX, posY) {}

void SageataRandom::aplica_efect(Caracter &tinta) const {
    auto valoare = MyRand<float>(min_dmg, max_dmg);
    dmg = valoare;
    if (valoare < 0.0f) tinta.Heal(-valoare * tragator->get_dmg_multiplier());
    else tinta.IaDamage(valoare, tragator->get_dmg_multiplier());;
    switch (MyRand<int>(0, 10)) {
        case 0: tinta.AplicaOtrava(MyRand<int>(1, 4)); break;
        case 1: tinta.AplicaBurn(MyRand<int>(1, 4)); break;
        case 2: tinta.DiscardSageata(); break;
        default: tragator->PushSageata(static_cast<tipSageti>(MyRand<int>(tipSageti::Normala, tipSageti::NrTipuri - 1)));
    }
}

std::unique_ptr<Sageata> SageataRandom::clone() const {
    return std::make_unique<SageataRandom>(*this);
}

std::string SageataRandom::nume() const {
    return "Random";
}

Color SageataRandom::get_color() const {
    const double acum = GetTime();
    if (ultima_schimbare < 0.0 || acum - ultima_schimbare >= interval_culoare) {
        culoare_curenta = Color{
            static_cast<unsigned char>(MyRand<int>(0, 255)),
            static_cast<unsigned char>(MyRand<int>(0, 255)),
            static_cast<unsigned char>(MyRand<int>(0, 255)),
            255
        };
        ultima_schimbare = acum;
    }
    return culoare_curenta;
}

std::unique_ptr<Sageata> Sageata_factory::creeaza(tipSageti tip, float x, float y) {
    static const Factory<tipSageti, Sageata, NrTipuri, float, float> fabrica = [] {
        Factory<tipSageti, Sageata, NrTipuri, float, float> f;
        f.inregistreaza(Normala,     [](float x, float y){ return std::make_unique<SageataNormala>(x, y); });
        f.inregistreaza(Otravitoare, [](float x, float y){ return std::make_unique<SageataOtravitoare>(x, y); });
        f.inregistreaza(Aimbot,      [](float x, float y){ return std::make_unique<SageataAimbot>(x, y); });
        f.inregistreaza(Healing,     [](float x, float y){ return std::make_unique<SageataHealing>(x, y); });
        f.inregistreaza(Giganta,     [](float x, float y){ return std::make_unique<SageataGiganta>(x, y); });
        f.inregistreaza(LifeSteal,   [](float x, float y){ return std::make_unique<SageataLifeSteal>(x, y); });
        f.inregistreaza(Burn,        [](float x, float y){ return std::make_unique<SageataBurn>(x, y); });
        f.inregistreaza(GlassCannon, [](float x, float y){ return std::make_unique<SageataGlassCannon>(x, y); });
        f.inregistreaza(Random,      [](float x, float y){ return std::make_unique<SageataRandom>(x, y); });
        return f;
    }();

    auto s = fabrica.creeaza(tip, x, y);
    if (!s) throw eroare_sageti(static_cast<int>(tip));
    return s;
}
