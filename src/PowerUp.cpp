//
// Created by user on 22.05.2026.
//
#include "PowerUp.hpp"
#include "sageata.hpp"
#include "caracter.hpp"

PowerUp::PowerUp(float min_x, float max_x, float min_y, float max_y) :
Entitate(50, 100, -1, -1), min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y){
    efect = static_cast<tipSageti>(MyRand<int>(tipSageti::Normala, tipSageti::NrTipuri-1));
}

PowerUp::PowerUp(float min_x, float max_x, float min_y, float max_y, tipSageti efect) :
Entitate(50, 100, -1, -1), min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y),
efect(efect){}


bool PowerUp::este_activ() const {
    return activ;
}

void PowerUp::_draw(raylib::Vector2) {
    if (!activ) return;
    raylib::Color culoare;
    if (efect == Invalid) culoare = WHITE;
    else culoare = culori.at(efect);
    hitbox.Draw(culoare);
}

void PowerUp::Update(float dt) {
    if (!activ) return;

    floatTimer += dt * 1.5f;
    hitbox.x += std::sin(floatTimer) * 2.0f;
}
void PowerUp::Consuma() {
    activ = false;
}

void PowerUp::TrySpawn() {
    if (!activ) {
        int prob = MyRand<int>(1, 10);
        if (prob == 1) {
            activ = true;
            efect = static_cast<tipSageti>(MyRand<int>(tipSageti::Normala + 1, tipSageti::NrTipuri-1));
            SetPosition(MyRand<float>(min_x, max_x), MyRand<float>(min_y, max_y));
        }
    }
}

void PowerUp::OnCollision(Entitate&) {
    Consuma();
}

bool PowerUp::GetCollision(Sageata &s) {
    if (fizica::VerColiziune(s.get_hitbox(), s.get_rotation(), hitbox, rotation)) {
        if (s.get_tragator())s.get_tragator()->PushSageata(efect);
        Consuma();
        return true;

    }
    return false;
}
