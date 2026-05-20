//
// Created by user on 13.05.2026.
//

#ifndef OOP_SAGEATA_HPP
#define OOP_SAGEATA_HPP

#include "basic_includes.hpp"

class Sageata {
    tipSageti tip;
    raylib::Vector2 pos;
    raylib::Vector2 vel{0.0f, 0.0f};
    float varsta=0.0f;
    static constexpr std::array<float, tipSageti::NrTipuri> damage{5, 3.5, 5, -10, 10, 5};
    static constexpr std::array<Color, tipSageti::NrTipuri> culori{BLUE, VIOLET, BLACK, GREEN, BLUE, RED};

public:
    explicit Sageata(tipSageti tip=Normala, float posX=-1, float posY=-1);
    [[nodiscard]] tipSageti get_tip() const;

    [[nodiscard]] raylib::Vector2 get_pos() const;

    [[nodiscard]] static float get_damage(tipSageti tip);

    [[nodiscard]] static Color get_color(tipSageti tip);

    [[nodiscard]] raylib::Vector2 get_velocity() const;
    void CresteVarsta();

    [[nodiscard]] bool este_veche() const;

    void UpdateVelocity(float dx, float dy);

    void MutaSageata(float dx, float dy);

    void MiscaSageata(float dx, float dy);

    friend std::ostream& operator<< (std::ostream& os, const Sageata& s);

};


#endif //OOP_SAGEATA_HPP