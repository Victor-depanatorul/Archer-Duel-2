//
// Created by user on 17.06.2026.
//

#include "caractere_variate.hpp"
#include "factory.hpp"
#include "exceptii.hpp"
#include <typeinfo>

Asasin::Asasin(float scale, float posX, float posY, float rotation) : CaracterCuAbilitate(scale, posX, posY, rotation)
{
    hp = 75.0f;
    dmg_multiplier = 1.25f;
}

Asasin::Asasin(const Arc& arc, float scale, float posX, float posY, float rotation) : CaracterCuAbilitate(arc, scale, posX, posY, rotation) {
    hp = 75.0f;
    dmg_multiplier = 1.25f;
}

void Asasin::close_hit() {
    if (!abilitate_disponibila()) return;
    close_hit_dmg = 10.0f;
    declanseaza_cooldown(2);
}

void Asasin::OnCollision(Entitate& other) {
    auto* c = dynamic_cast<Caracter*>(&other);
    if (c != nullptr && (in_miscare() || c->in_miscare())) {
        c->IaDamage(close_hit_dmg, 1.0f);
        if (typeid(*c) == typeid(*this)) IaDamage(close_hit_dmg, 1.0f);
    }
    Caracter::OnCollision(other);
}

void Asasin::ActiuneAditionala() {
    if (IsKeyPressed(KEY_E)) close_hit();
}

void Asasin::la_sfarsit_tura() {
    close_hit_dmg = 5.0f;
}

Tank::Tank(float scale, float posX, float posY, float rotation) : Caracter(scale, posX, posY, rotation) {
    this->scale = base_scale * 1.5f;
    aplica_scalare();
    hp = 200.0f;
    viteza_miscare = 2.5f;
    max_forta_tragere = 1000.0f;
}

Tank::Tank(const Arc& arc, float scale, float posX, float posY, float rotation) : Caracter(arc, scale, posX, posY, rotation) {
    this->scale = base_scale * 1.5f;
    aplica_scalare();
    hp = 200.0f;
    viteza_miscare = 2.5f;
    max_forta_tragere = 1000.0f;
}

float Tank::multiplicator_damage_primit() const {
    return MyRand<int>(1, 100) <= 5 ? 0.0f : 1.0f;
}

Mage::Mage(float scale, float posX, float posY, float rotation) : CaracterCuAbilitate(scale, posX, posY, rotation) {}

Mage::Mage(const Arc& arc, float scale, float posX, float posY, float rotation) : CaracterCuAbilitate(arc, scale, posX, posY, rotation) {}


void Mage::ChangeArrow() {
    if (!AreSageti()) return;
    DiscardSageata();
    PushSageata(tipSageti::Otravitoare);
}

float Mage::vulnerabilitate(const Sageata& s) const {
    return s.e_fizica() ? 1.25f : 1.0f;
}

void Mage::ActiuneAditionala() {
    if (IsKeyPressed(KEY_E) && mana >= 30.0f) { Heal(25.0f); mana -= 30.0f; }
    if (IsKeyPressed(KEY_R) && mana >= 40.0f) { ChangeArrow(); mana -= 40.0f; }
    if (IsKeyPressed(KEY_T) && mana > 0.0f) {
        SetArmorMultiplier(1.0f - mana / mana_max, false, 2);
        mana = 0.0f;
    }
}

void Mage::la_sfarsit_tura() {
    mana = std::min(mana_max, mana + regen_mana);
}

std::unique_ptr<Caracter> Caracter_factory::creeaza(tipCaracter tip, const Arc& arc, float scale,
                                                    float posX, float posY, float rotation) {
    static const Factory<tipCaracter, Caracter, NrCaractere, Arc, float, float, float, float> fab = [] {
        Factory<tipCaracter, Caracter, NrCaractere, Arc, float, float, float, float> f;
        f.inregistreaza(CaracterNormal, [](const Arc& a, float s, float x, float y, float r){ return std::make_unique<Caracter>(a, s, x, y, r); });
        f.inregistreaza(CaracterAsasin, [](const Arc& a, float s, float x, float y, float r){ return std::make_unique<Asasin>(a, s, x, y, r); });
        f.inregistreaza(CaracterTank,   [](const Arc& a, float s, float x, float y, float r){ return std::make_unique<Tank>(a, s, x, y, r); });
        f.inregistreaza(CaracterMage,   [](const Arc& a, float s, float x, float y, float r){ return std::make_unique<Mage>(a, s, x, y, r); });
        return f;
    }();
    auto c = fab.creeaza(tip, arc, scale, posX, posY, rotation);
    if (!c) throw eroare_joc("Tip de caracter invalid: " + std::to_string(static_cast<int>(tip)));
    return c;
}

const std::array<InfoCaracter, NrCaractere>& Caracter_factory::catalog() {
    static const std::array<InfoCaracter, NrCaractere> info = {{
        {"None",   "Caracter clasic, fara abilitati speciale."},
        {"Asasin", "Mai putin HP, damage si viteza mai mari. Lovitura de aproape; abilitate close-hit (E) cu cooldown."},
        {"Tank",   "HP dublu, mai mare si mai lent, forta de tragere mai mica. 5% sansa sa ia 0 damage."},
        {"Mage",   "Mana: heal (E), transforma sageata (R), rezistenta (T). Slab la sageti Normale/Gigante."}
    }};
    return info;
}
