//
// Created by user on 13.05.2026.
//

#ifndef OOP_SAGEATA_HPP
#define OOP_SAGEATA_HPP

#include "basic_includes.hpp"
#include "entitate.hpp"

class Sageata : public Entitate{
    tipSageti tip;
    raylib::Vector2 viteza = {0.0f, 0.0f};
    raylib::Vector2 init_pos = {-1.0f, -1.0f};
    bool veche = false;
    void OnCollision(Entitate&) override;
    void _draw(raylib::Vector2 centru) override;

public:
    explicit Sageata(tipSageti tip=Normala, float posX=-1, float posY=-1);

    ~Sageata() override = default;

    [[nodiscard]] tipSageti get_tip() const;

    [[nodiscard]] raylib::Vector2 get_pos() const;

    [[nodiscard]] float get_damage() const;

    [[nodiscard]] Color get_color() const;

    [[nodiscard]] raylib::Vector2 get_velocity() const;


    [[nodiscard]] bool este_veche() const;

    void SetPosition(float x, float y) override;

    void UpdateVelocity(float vitezaScala);

    void UpdateImunitate(const Entitate& e);

    void Update(float dt, const Entitate& e);

    friend std::ostream& operator<< (std::ostream& os, const Sageata& s);

};


#endif //OOP_SAGEATA_HPP