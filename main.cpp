#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <vector>
#include <raylib-cpp.hpp>
namespace miscare {
    const char MoveKeys[5] = "WASD";
    raylib::Vector2 ChangePos[4] = {{0, -10}, {-10, 0}, {0, 10}, {10, 0}};
}
namespace fizica {
    const float g = 9.8;
    float radiani_grade(float unghi) {return unghi * PI / 180;}
    float grade_radinai(float unghi) {return unghi * 180 / PI;}
    void PozitieUrmatoare(float &x, float &y, float F, float &unghi, float d) {
        y=x*tan(unghi)-(g*x*x)/(20*F*d*cos(unghi)*cos(unghi));
        x+=0.5;
        unghi=atan2(y,x);
    } //unghiul trebuie dat in radiani
}
enum tipSageti : unsigned char {
    Normala,
    Otravitoare,
    Aimbot,
    Healing,
    Giganta,
    NrTipuri
};
class Sageata {
    tipSageti tip;
    float viteza;
    static constexpr std::array<float, tipSageti::NrTipuri> damage = {5, 2.5, 5, 0, 10};
    raylib::Rectangle rect;
public:
    Sageata() :tip(Normala), viteza(0), rect(0, 0, 5, 1) {}
    Sageata(tipSageti tip) : tip(tip), viteza(0), rect(0, 0, 5, 1) {}
    Sageata(tipSageti tip, raylib::Vector2 pos) : tip(tip), viteza(0), rect(pos.x, pos.y, 5, 1) {}
    Sageata(const Sageata& AltaSageata) : tip(AltaSageata.tip), viteza(0), rect(AltaSageata.rect) {}
    Sageata& operator=(const Sageata& AltaSageata) {
        if (this != &AltaSageata) {
            tip = AltaSageata.tip;
            viteza = 0;
            rect = AltaSageata.rect;
        }
        return *this;
    }
    void PozitieUrmatoare(float F, float& unghi, float d) {
        fizica::PozitieUrmatoare(rect.x, rect.y, F, unghi, d);
    }
};
class Arc {
    int CapacitateArc = 20;
    std::vector<Sageata> Sageti;
};

class Caracter {
    int hp=100;

};
class Bloc {

};

int main() {
    raylib::Window window(800, 600, "test");
    raylib::Texture2D textura("textures/PrimavaraFrumoasaV2.png");
    window.SetTargetFPS(60);
    while (!window.ShouldClose()) {
        window.BeginDrawing();
        ClearBackground(RAYWHITE);
        window.EndDrawing();
    }

    return 0;
}