//
// Created by user on 22.05.2026.
//
#include "PowerUp.hpp"
#include "sageata.hpp"
#include "tipuri_sageti.hpp"
#include "caracter.hpp"

PowerUp::PowerUp(float min_x, float max_x, float min_y, float max_y) :
Entitate(25, 50, -1, -1), min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y){
    tip = ARROW;
    efect = static_cast<tipSageti>(MyRand<int>(tipSageti::Normala, tipSageti::NrTipuri-1));
}

PowerUp::PowerUp(float min_x, float max_x, float min_y, float max_y, tipSageti efect) :
Entitate(25, 50, -1, -1), min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y),
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
            culoare = (efect == Invalid) ? WHITE : raylib::Color(creeaza_sageata(efect)->get_color());
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

void PowerUp::SetSpawnArea(float minx, float maxx, float miny, float maxy) {
    min_x = minx; max_x = maxx; min_y = miny; max_y = maxy;
}

void PowerUp::Reaseaza(float old_w, float old_h, float new_w, float new_h) {
    if (!activ) return;
    const float fx = old_w > 0.0f ? hitbox.x / old_w : 0.0f;
    const float fy = old_h > 0.0f ? hitbox.y / old_h : 0.0f;
    const float x = std::clamp(fx * new_w, 0.0f, std::max(0.0f, new_w - hitbox.width));
    const float y = std::clamp(fy * new_h, 0.0f, std::max(0.0f, new_h - hitbox.height));
    SetPosition(x, y);
}

void PowerUp::TrySpawn() {
    if (activ) return;
    if (MyRand<int>(1, max_prob) != 1) return;

    if (MyRand<int>(0, 1)) {
        tip = static_cast<tipPowerUp>(MyRand<int>(1, tipPowerUp::NrPowerUps - 1));
    } else {
        tip = tipPowerUp::ARROW;
        efect = static_cast<tipSageti>(MyRand<int>(tipSageti::Normala + 1, tipSageti::NrTipuri - 1));
    }
    activ = true;

    const float hi_x = std::max(min_x, max_x - hitbox.width);
    const float hi_y = std::max(min_y, max_y - hitbox.height);
    SetPosition(MyRand<float>(min_x, hi_x), MyRand<float>(min_y, hi_y));
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
