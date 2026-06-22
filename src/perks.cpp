//
// Created by user on 14.06.2026.
//

#include "perks.hpp"
#include "factory.hpp"
#include "exceptii.hpp"

std::unique_ptr<Perk> Perk_factory::creeaza(Perks tip) {
    static const Factory<Perks, Perk, NrPerks> fabrica = [] {
        Factory<Perks, Perk, NrPerks> f;
        f.inregistreaza(Heal, [] {
            return std::make_unique<Perk>(500, "Heal", "Recupereaza 25 HP.",
                [](Caracter* c){ c->Heal(25.0f); });
        });
        f.inregistreaza(GetMultiShot, [] {
            return std::make_unique<Perk>(1000, "Multi-Shot", "Tragi 3 sageti tura urmatoare.",
                [](Caracter* c){ c->PrimesteMultiShot(3); });
        });
        f.inregistreaza(GetTmpDamageMultiplier, [] {
            return std::make_unique<Perk>(1000, "Damage temporar", "x0.5 in plus la damage multiplier timp de 3 ture.",
                [](Caracter* c){ c->SetDmgMultiplier(0.5f, false, 3); });
        });
        f.inregistreaza(GetDamageMultiplier, [] {
            return std::make_unique<Perk>(2000, "Damage permanent", "x0.25 in plus la damage multiplier pentru tot meciul.",
                [](Caracter* c){ c->SetDmgMultiplier(0.25f, true); });
        });
        f.inregistreaza(GetTmpArmor, [] {
            return std::make_unique<Perk>(1000, "Armura temporara", "-50% damage direct timp de 3 ture.",
                [](Caracter* c){ c->SetArmorMultiplier(0.5f, false, 3); });
        });
        f.inregistreaza(GetArmor, [] {
            return std::make_unique<Perk>(2500, "Armura permanenta", "-50% damage direct pentru tot meciul.",
                [](Caracter* c){ c->SetArmorMultiplier(0.75f, true); });
        });
        return f;
    }();

    auto p = fabrica.creeaza(tip);
    if (!p) throw eroare_joc("Perk invalid: " + std::to_string(static_cast<int>(tip)));
    return p;
}

std::array<std::unique_ptr<Perk>, Perks::NrPerks> Perk_factory::predefinite() {
    std::array<std::unique_ptr<Perk>, Perks::NrPerks> catalog;
    for (int i = 0; i < Perks::NrPerks; ++i)
        catalog[static_cast<std::size_t>(i)] = creeaza(static_cast<Perks>(i));
    return catalog;
}
