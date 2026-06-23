//
// Created by user on 17.06.2026.
//

#ifndef OOP_CARACTERE_VARIATE_HPP
#define OOP_CARACTERE_VARIATE_HPP

#include "caracter.hpp"
#include <string>
#include <array>

class CaracterCuAbilitate : public Caracter {
protected:
    int cooldown_abilitate = 0;
    [[nodiscard]] bool abilitate_disponibila() const;
    void declanseaza_cooldown(int runde);
    void la_incheiere_tura() override;
    virtual void la_sfarsit_tura() = 0;
public:
    explicit CaracterCuAbilitate(float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    explicit CaracterCuAbilitate(const Arc& arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    void ActiuneAditionala() override = 0;
};

class Asasin : public CaracterCuAbilitate {
    float close_hit_dmg = 5.0f;

    void OnCollision(Entitate& other) override;
    void ActiuneAditionala() override;
    void la_sfarsit_tura() override;


public:
    explicit Asasin(float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    explicit Asasin(const Arc& arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    void close_hit();
    [[nodiscard]] std::string nume_clasa() const override;
};

class Tank : public Caracter {
    [[nodiscard]] float multiplicator_damage_primit() const override;
public:
    explicit Tank(float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    explicit Tank(const Arc& arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    [[nodiscard]] std::string nume_clasa() const override;
};

class Mage : public CaracterCuAbilitate {
    float mana = 50.0f;
    float mana_max = 100.0f;
    float regen_mana = 10.0f;

    void ChangeArrow();
    [[nodiscard]] float vulnerabilitate(const Sageata& s) const override;
    void ActiuneAditionala() override;
    void la_sfarsit_tura() override;

public:
    explicit Mage(float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    explicit Mage(const Arc& arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    [[nodiscard]] std::string nume_clasa() const override;
    [[nodiscard]] std::vector<LinieHud> info_hud() const override;
};


struct InfoCaracter {
    std::string nume;
    std::string descriere;
};

class Reinforcer : public CaracterCuAbilitate {
    void PutArmor();
    void ActiuneAditionala() override;
    void la_sfarsit_tura() override;
public:
    explicit Reinforcer(float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    explicit Reinforcer(const Arc& arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    [[nodiscard]] std::string nume_clasa() const override;
    [[nodiscard]] int durabilitate_zid() const override;
};

class Caracter_factory {
public:
    static std::unique_ptr<Caracter> creeaza(tipCaracter tip, const Arc& arc, float scale,
                                             float posX, float posY, float rotation);
    static const std::array<InfoCaracter, tipCaracter::NrCaractere>& catalog();
};

#endif //OOP_CARACTERE_VARIATE_HPP
