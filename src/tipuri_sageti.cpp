#include "tipuri_sageti.hpp"
#include "caracter.hpp"
#include "exceptii.hpp"
#include "factory.hpp"
#include <cmath>

SageataDeBaza::SageataDeBaza(float posX, float posY, float width, float height, float dmg,
                             Color culoare, std::string nume, int dmg_perete)
    : Sageata(posX, posY, width, height, dmg), culoare(culoare), nume_(std::move(nume)),
      dmg_perete(dmg_perete) {}

std::unique_ptr<Sageata> SageataDeBaza::clone() const {
    return std::make_unique<SageataDeBaza>(*this);
}

void SageataDeBaza::aplica_efect(Caracter& tinta) const {
    if (tragator != nullptr && dmg > 0.0f)
        tinta.IaDamage(dmg, tragator->get_dmg_multiplier());
}

Color SageataDeBaza::get_color() const { return culoare; }
std::string SageataDeBaza::nume() const { return nume_; }
bool SageataDeBaza::e_fizica() const { return true; }
int SageataDeBaza::damage_perete() const { return dmg_perete; }

SageataCuEfect::SageataCuEfect(float posX, float posY, float width, float height, float dmg,
                               Color culoare, std::string nume, Efect efect, int dmg_perete)
    : SageataDeBaza(posX, posY, width, height, dmg, culoare, std::move(nume), dmg_perete),
      efect(std::move(efect)) {}

std::unique_ptr<Sageata> SageataCuEfect::clone() const {
    return std::make_unique<SageataCuEfect>(*this);
}

void SageataCuEfect::aplica_efect(Caracter& tinta) const {
    SageataDeBaza::aplica_efect(tinta);
    if (efect && tragator != nullptr) efect(tinta, *tragator, dmg);
}

bool SageataCuEfect::e_fizica() const { return false; }

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

SageataGlassCannon::SageataGlassCannon(float posX, float posY)
    : SageataDeBaza(posX, posY, 100.0f, 50.0f, 30.0f, DARKGRAY, "Glass Cannon", 99) {}

void SageataGlassCannon::update(float dt, const std::vector<Entitate*>& obstacole, float podea) {
    Sageata::update(dt, obstacole, podea);
    if (trebuie_distrusa && !a_nimerit && tragator != nullptr)
        tragator->IaDamage(20.0f, 1.0f);
}

std::unique_ptr<Sageata> SageataGlassCannon::clone() const {
    return std::make_unique<SageataGlassCannon>(*this);
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
    static const Factory<tipSageti, Sageata, tipSageti::NrTipuri, float, float> fabrica = [] {
        Factory<tipSageti, Sageata, tipSageti::NrTipuri, float, float> f;
        f.inregistreaza(tipSageti::Normala, [](float x, float y){
            return std::make_unique<SageataDeBaza>(x, y, 40.0f, 20.0f, 7.5f, BLUE, "Normala");
        });
        f.inregistreaza(tipSageti::Otravitoare, [](float x, float y){
            return std::make_unique<SageataCuEfect>(x, y, 40.0f, 20.0f, 4.0f, VIOLET, "Otravitoare",
                [](Caracter& t, Caracter&, float){ t.AplicaOtrava(3); });
        });
        f.inregistreaza(tipSageti::Aimbot,      [](float x, float y){ return std::make_unique<SageataAimbot>(x, y); });
        f.inregistreaza(tipSageti::Healing, [](float x, float y){
            return std::make_unique<SageataCuEfect>(x, y, 40.0f, 20.0f, -7.5f, GREEN, "Healing",
                [](Caracter& t, const Caracter& tr, float d){ t.Heal(-d * tr.get_dmg_multiplier()); });
        });
        f.inregistreaza(tipSageti::Giganta, [](float x, float y){
            return std::make_unique<SageataDeBaza>(x, y, 80.0f, 40.0f, 15.0f, DARKBLUE, "Giganta", 99);
        });
        f.inregistreaza(tipSageti::LifeSteal, [](float x, float y){
            return std::make_unique<SageataCuEfect>(x, y, 40.0f, 20.0f, 7.5f, RED, "LifeSteal",
                [](Caracter&, Caracter& tr, float d){ tr.Heal(d * tr.get_dmg_multiplier()); });
        });
        f.inregistreaza(tipSageti::Burn, [](float x, float y){
            return std::make_unique<SageataCuEfect>(x, y, 40.0f, 20.0f, 0.0f, ORANGE, "Burn",
                [](Caracter& t, Caracter&, float){ t.AplicaBurn(2); });
        });
        f.inregistreaza(tipSageti::GlassCannon, [](float x, float y){ return std::make_unique<SageataGlassCannon>(x, y); });
        f.inregistreaza(tipSageti::Random,      [](float x, float y){ return std::make_unique<SageataRandom>(x, y); });
        return f;
    }();

    auto s = fabrica.creeaza(tip, x, y);
    if (!s) throw eroare_sageti(static_cast<int>(tip));
    return s;
}
