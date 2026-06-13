//
// Created by user on 13.06.2026.
//

#ifndef OOP_PERKS_HPP
#define OOP_PERKS_HPP

#include <functional>
#include "caracter.hpp"
#include "constante.hpp"
#include <string>
#include <array>

class Perk {
    int pret;
    std::string nume;
    std::string descriere;
    std::function<void(Caracter*)> aplica_efect;
public:
    explicit Perk(int pret, std::string nume = "Nedefinit", std::string descriere = "Necunoscuta")
    : pret(pret), nume(std::move(nume)), descriere(std::move(descriere)) {}
    void Efect(const std::function<void(Caracter*)>& f) {aplica_efect = f;}
    void AplicaEfect(Caracter* c) const {if (aplica_efect) aplica_efect(c);}
    [[nodiscard]] int Pret() const {return pret;}
    [[nodiscard]] const std::string& Nume() const {return nume;}
    [[nodiscard]] const std::string& Descriere() const {return descriere;}
};

inline std::array<Perk, Perks::NrPerks> creeaza_perks() {
    std::array<Perk, Perks::NrPerks> perks{{
        Perk{300, "Heal",             "Recupereaza 25 HP."},
        Perk{500, "Multi-Shot",       "Tragi 3 sageti tura urmatoare."},
        Perk{500, "Damage temporar",  "x0.5 in plus la damage multiplier timp de 3 ture."},
        Perk{1000, "Damage permanent", "x0.25 in plus la damage multiplier pentru tot meciul."},
        Perk{500, "Armura temporara", "-50% damage direct timp de 3 ture."},
        Perk{1250, "Armura permanenta","-50% damage direct pentru tot meciul."}
    }};

    perks[Heal].Efect([](Caracter* c){ c->Heal(25.0f); });
    perks[GetMultiShot].Efect([](Caracter* c){ c->PrimesteMultiShot(3); });
    perks[GetTmpDamageMultiplier].Efect([](Caracter* c){ c->SetDmgMultiplier(0.5f, false, 3); });
    perks[GetDamageMultiplier].Efect([](Caracter* c){ c->SetDmgMultiplier(0.25f, true); });
    perks[GetTmpArmor].Efect([](Caracter* c){ c->SetArmorMultiplier(0.5f, false, 3); });
    perks[GetArmor].Efect([](Caracter* c){ c->SetArmorMultiplier(0.75f, true); });

    return perks;
}


#endif //OOP_PERKS_HPP