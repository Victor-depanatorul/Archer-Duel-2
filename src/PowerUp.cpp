//
// Created by user on 22.05.2026.
//
#include "PowerUp.hpp"
#include "sageata.hpp"
#include "caracter.hpp"

PowerUp::PowerUp(float min_x, float max_x, float min_y, float max_y) :
Entitate(50, 100, -1, -1), min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y){
    tip = ARROW;
    efect = static_cast<tipSageti>(MyRand<int>(tipSageti::Normala, tipSageti::NrTipuri-1));
}

PowerUp::PowerUp(float min_x, float max_x, float min_y, float max_y, tipSageti efect) :
Entitate(50, 100, -1, -1), min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y),
tip(ARROW), efect(efect){}


bool PowerUp::este_activ() const {
    return activ;
}

void PowerUp::_draw(raylib::Vector2) {
    if (!activ) return;
    raylib::Color culoare;
    switch (tip) {
        case MULTI_SHOT:  culoare = GOLD;    break;
        case DOUBLE_MOVE: culoare = SKYBLUE; break;
        case ARROW:
        default:
            culoare = (efect == Invalid) ? WHITE : raylib::Color(culori.at(efect));
            break;
    }
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
        int prob = MyRand<int>(0, max_prob);
        if (prob == 0) {
            activ = true;
            tip = static_cast<tipPowerUp>(MyRand<int>(1, tipPowerUp::NrPowerUps - 1));
            SetPosition(MyRand<float>(min_x, max_x), MyRand<float>(min_y, max_y));
        }
        else if (prob == 1) {
            tip = tipPowerUp::ARROW;
            efect = static_cast<tipSageti>(MyRand<int>(tipSageti::Normala + 1, tipSageti::NrTipuri - 1));
        }
    }
}

void PowerUp::OnCollision(Entitate&) {
    Consuma();
}

void PowerUp::OnCollision(Sageata &s) {
    Caracter* tragator = s.get_tragator();
    if (tragator != nullptr) {
        tragator->stats_powerup();
        switch (tip) {
            case MULTI_SHOT:  tragator->PrimesteMultiShot(MyRand<int>(2, 4)); break;
            case DOUBLE_MOVE: tragator->PrimesteDoubleMove();                 break;
            case ARROW:
            default:          tragator->PushSageata(efect);                   break;
        }
    }
    Consuma();
}

void PowerUp::SetProbability(int probability) {max_prob = probability;}
