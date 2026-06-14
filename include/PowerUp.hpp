//
// Created by user on 22.05.2026.
//

#ifndef OOP_POWERUP_HPP
#define OOP_POWERUP_HPP
#include "basic_includes.hpp"
#include "entitate.hpp"

enum tipPowerUp {
    ARROW,
    MULTI_SHOT,
    DOUBLE_MOVE,
    NrPowerUps
};

class PowerUp : public Entitate {
private:
    bool activ = false;
    float min_x, max_x, min_y, max_y;
    float floatTimer = 0.0f;
    float x_baza = 0.0f;
    tipPowerUp tip = ARROW;
    tipSageti efect;
    int max_prob = 5;
public:
    PowerUp(float min_x, float max_x, float min_y, float max_y);
    PowerUp(float min_x, float max_x, float min_y, float max_y, tipSageti efect);

    [[nodiscard]] bool este_activ() const;

    void Update(float dt) override;
    void Consuma();
    void OnCollision(Entitate& other) override;
    void OnCollision(Sageata& s) override;
    void _draw(raylib::Vector2 centru) override;
    void TrySpawn();
    void SetSpawnArea(float min_x, float max_x, float min_y, float max_y);
    void Reaseaza(float old_w, float old_h, float new_w, float new_h);
    void SetProbability(int probability);
};


#endif //OOP_POWERUP_HPP