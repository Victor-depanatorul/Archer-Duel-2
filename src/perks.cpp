//
// Created by user on 14.06.2026.
//

#include "perks.hpp"
#include "factory.hpp"
#include "exceptii.hpp"

std::unique_ptr<Perk> Perk_factory::creeaza(Perks tip) {
    static const Factory<Perks, Perk, NrPerks> fabrica = [] {
        Factory<Perks, Perk, NrPerks> f;
        f.inregistreaza(Heal,                   [] { return std::make_unique<PerkHeal>(); });
        f.inregistreaza(GetMultiShot,           [] { return std::make_unique<PerkMultiShot>(); });
        f.inregistreaza(GetTmpDamageMultiplier, [] { return std::make_unique<PerkTmpDamageMultiplier>(); });
        f.inregistreaza(GetDamageMultiplier,    [] { return std::make_unique<PerkDamageMultiplier>(); });
        f.inregistreaza(GetTmpArmor,            [] { return std::make_unique<PerkTmpArmor>(); });
        f.inregistreaza(GetArmor,               [] { return std::make_unique<PerkArmor>(); });
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
