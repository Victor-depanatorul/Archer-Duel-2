//
// Created by user on 13.06.2026.
//

#ifndef OOP_PERKS_HPP
#define OOP_PERKS_HPP

#include "caracter.hpp"
#include <string>
#include <array>
#include <memory>
#include <functional>

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
    int pret;
    std::string nume;
    std::string descriere;
    std::function<void(Caracter*)> efect;
public:
    Perk(int pret, std::string nume, std::string descriere, std::function<void(Caracter*)> efect)
    : pret(pret), nume(std::move(nume)), descriere(std::move(descriere)), efect(std::move(efect)) {}
    void AplicaEfect(Caracter* c) const { if (efect) efect(c); }
    [[nodiscard]] int Pret() const {return pret;}
    [[nodiscard]] const std::string& Nume() const {return nume;}
    [[nodiscard]] const std::string& Descriere() const {return descriere;}
};

class Perk_factory {
public:
    static std::unique_ptr<Perk> creeaza(Perks tip);
    static std::array<std::unique_ptr<Perk>, Perks::NrPerks> predefinite();
};


#endif //OOP_PERKS_HPP
