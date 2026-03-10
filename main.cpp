#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <vector>
#include <raylib-cpp.hpp>
namespace miscare {
    constexpr char MoveKeys[5] = "WASD";
    raylib::Vector2 ChangePos[4] = {{0, -10}, {-10, 0}, {0, 10}, {10, 0}};
}
namespace fizica {
    constexpr float g = 9.8;
    constexpr float MaxForta = 10;
    constexpr float MaxDrawLength = 5*1.41;
    void PozitieUrmatoare(float &x0, float &y0, float &x, float &y) {
        float deltaX = x0-x, deltaY= y0-y;
        float unghi=atan2(deltaY,deltaX);
        float d = sqrt(deltaX*deltaX+deltaY*deltaY);
        float F = MaxForta/MaxDrawLength * d;
        float prevX=x, prevY=y;
        x++;
        y=y0+deltaX*tan(unghi)+(g*deltaX*deltaX)*(10*F*d*cos(unghi)*cos(unghi));
        x0=prevX, y0=prevY;
    }


} //unghiul trebuie dat in radiani
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
    static constexpr std::array<float, tipSageti::NrTipuri> damage = {5, 2.5, 5, 0, 10};
    raylib::Rectangle rect;
public:
    Sageata() :tip(Normala), rect(0, 0, 5, 1) {}
    explicit Sageata(tipSageti tip) : tip(tip), rect(0, 0, 5, 1) {}
    Sageata(tipSageti tip, raylib::Vector2 pos) : tip(tip), rect(pos.x, pos.y, 5, 1) {}
    Sageata(const Sageata& AltaSageata) = default;
    Sageata& operator=(const Sageata& AltaSageata) {
        if (this != &AltaSageata) {
            tip = AltaSageata.tip;
            rect = AltaSageata.rect;
        }
        return *this;
    }
};
class Arc {
    int CapacitateArc = 20;
    std::vector<Sageata> Sageti;
    public:
    Arc() : Sageti(10) {}
    explicit Arc(int capacitate) :  CapacitateArc(capacitate), Sageti(capacitate>>2) {}
    Arc(int capacitate, std::vector<Sageata> upgrade)  : CapacitateArc(capacitate),
                                                         Sageti(std::move(upgrade)) {}
    Arc(const Arc& Arc) : CapacitateArc(Arc.CapacitateArc), Sageti(Arc.Sageti) {}
    Arc& operator=(const Arc& Arc) {
        if (this != &Arc) {
            Sageti = Arc.Sageti;
            CapacitateArc = Arc.CapacitateArc;
        }
        return *this;
    }
};

class Caracter {
    int hp=100;
    float forta=0;
public:
    Caracter() {}
    explicit Caracter(int hp) : hp(hp) {}
    Caracter(const Caracter &other) = default;
    Caracter& operator=(const Caracter &other) {
        if (this != &other) {
            hp=other.hp;
            forta=other.forta;
        }
    }
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