//
// Created by user on 22.05.2026.
//

#ifndef OOP_POWERUP_HPP
#define OOP_POWERUP_HPP
#include "basic_includes.hpp"
#include "entitate.hpp"

class PowerUp : public Entitate {
private:
    bool activ = false;
    float min_x, max_x, min_y, max_y;
    float floatTimer = 0.0f;
    tipSageti efect;
public:
    PowerUp(float min_x, float max_x, float min_y, float max_y);
    PowerUp(float min_x, float max_x, float min_y, float max_y, tipSageti efect);

    [[nodiscard]] bool este_activ() const;

    void Update(float dt) override;
    void Consuma();
    void OnCollision(Entitate& other) override;
    bool GetCollision(Sageata& s) override;
    void _draw(raylib::Vector2 centru) override;
    void TrySpawn();
};


#endif //OOP_POWERUP_HPP