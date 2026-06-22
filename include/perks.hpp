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
    Perk(int pret, std::string nume, std::string descriere, std::function<void(Caracter*)> efect);
    void AplicaEfect(Caracter* c) const;
    [[nodiscard]] int Pret() const;
    [[nodiscard]] const std::string& Nume() const;
    [[nodiscard]] const std::string& Descriere() const;
};

class Perk_factory {
public:
    static std::unique_ptr<Perk> creeaza(Perks tip);
    static std::array<std::unique_ptr<Perk>, Perks::NrPerks> predefinite();
};


#endif //OOP_PERKS_HPP
