//
// Created by user on 13.06.2026.
//

#ifndef OOP_PERKS_HPP
#define OOP_PERKS_HPP

#include "caracter.hpp"
#include <string>
#include <array>
#include <memory>

enum Perks{
    Heal,
    GetMultiShot,
    GetTmpDamageMultiplier,
    GetDamageMultiplier,
    GetTmpArmor,
    GetArmor,
    NrPerks
};

class Perk {
protected:
    int pret;
    std::string nume;
    std::string descriere;
public:
    explicit Perk(int pret, std::string nume = "Nedefinit", std::string descriere = "Necunoscuta")
    : pret(pret), nume(std::move(nume)), descriere(std::move(descriere)) {}
    virtual void AplicaEfect(Caracter* c) const = 0;
    [[nodiscard]] int Pret() const {return pret;}
    [[nodiscard]] const std::string& Nume() const {return nume;}
    [[nodiscard]] const std::string& Descriere() const {return descriere;}
    virtual ~Perk() = default;
};

class PerkHeal : public Perk {
public:
    PerkHeal() : Perk(500, "Heal", "Recupereaza 25 HP.") {}
    void AplicaEfect(Caracter* c) const override {c->Heal(25.0f);}
};

class PerkMultiShot : public Perk {
public:
    PerkMultiShot() : Perk(1000, "Multi-Shot", "Tragi 3 sageti tura urmatoare.")  {}
    void AplicaEfect(Caracter* c) const override {c->PrimesteMultiShot(3);}
};

class PerkTmpDamageMultiplier : public Perk {
public:
    PerkTmpDamageMultiplier() : Perk(1000, "Damage temporar", "x0.5 in plus la damage multiplier timp de 3 ture.") {}
    void AplicaEfect(Caracter* c) const override {c->SetDmgMultiplier(0.5f, false, 3);}
};

class PerkDamageMultiplier : public Perk {
public:
    PerkDamageMultiplier() : Perk(2000, "Damage permanent", "x0.25 in plus la damage multiplier pentru tot meciul.") {}
    void AplicaEfect(Caracter* c) const override {c->SetDmgMultiplier(0.25f, true);}
};

class PerkTmpArmor : public Perk {
public:
    PerkTmpArmor() : Perk(1000, "Armura temporara", "-50% damage direct timp de 3 ture.") {}
    void AplicaEfect(Caracter* c) const override {c->SetArmorMultiplier(0.5f, false, 3);}
};

class PerkArmor : public Perk {
public:
    PerkArmor() : Perk(2500, "Armura permanenta", "-50% damage direct pentru tot meciul.") {}
    void AplicaEfect(Caracter* c) const override {c->SetArmorMultiplier(0.75f, true);}
};

class Perk_factory {
public:
    static std::unique_ptr<Perk> creeaza(Perks tip);
    static std::array<std::unique_ptr<Perk>, Perks::NrPerks> predefinite();
};


#endif //OOP_PERKS_HPP