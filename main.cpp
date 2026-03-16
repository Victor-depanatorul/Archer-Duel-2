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
    // void PozitieUrmatoare(float &x0, float &y0, float &x, float &y) {
    //     float deltaX = x0-x, deltaY= y0-y;
    //     float unghi=atan2(deltaY,deltaX);
    //     float d = sqrt(deltaX*deltaX+deltaY*deltaY);
    //     float F = MaxForta/MaxDrawLength * d;
    //     float prevX=x, prevY=y;
    //     x++;
    //     y=y0+deltaX*tan(unghi)+(g*deltaX*deltaX)*(10*F*d*cos(unghi)*cos(unghi));
    //     x0=prevX, y0=prevY;
    // }


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
    explicit Sageata(tipSageti tip=Normala, float posX=-1, float posY=-1) :
    tip(tip), rect(posX, posY, 5, 1) {}
    Sageata(const Sageata& AltaSageata) = default;
    Sageata& operator=(const Sageata& AltaSageata) {
        if (this != &AltaSageata) {
            tip = AltaSageata.tip;
            rect = AltaSageata.rect;
        }
        return *this;
    }

    [[nodiscard]] tipSageti get_tip() const {return tip;}

    [[nodiscard]] raylib::Rectangle get_rect() const{return rect;}

    friend std::ostream& operator<< (std::ostream& os, const Sageata& s) {
        os << "Pozitia:(" << s.rect.x << ", " << s.rect.y << ")" << '\n';
        os << "Marimea HitBox-ului:" << s.rect.width << ", " << s.rect.height << '\n';
        os << "Tipul sagetii:" << s.tip << std::endl;
        return os;
    }
};

class Arc {
    unsigned long long CapacitateArc;
    std::vector<Sageata> Sageti;
    public:
    explicit Arc(unsigned long long CapacitateArc=20, tipSageti tip=Normala) :
    CapacitateArc(CapacitateArc), Sageti(CapacitateArc, Sageata(tip)) {}
    explicit Arc(const std::vector<Sageata>& Sageti){
        CapacitateArc=Sageti.size();
        this->Sageti=Sageti;
    }
    Arc(const Arc& Arc) = default;
    Arc& operator=(const Arc& Arc) {
        if (this != &Arc) {
            Sageti = Arc.Sageti;
            CapacitateArc = Arc.CapacitateArc;
        }
        return *this;
    }
    friend std::ostream& operator<< (std::ostream& os, const Arc& a) {
        os << "Capacitatea arcului:" << a.CapacitateArc << '\n';
        bool SagetiGasite[tipSageti::NrTipuri] = {false};
        for (const Sageata& sageata : a.Sageti) {
            SagetiGasite[sageata.get_tip()] = true;
        }
        os << "Arcul contine sageti de tipul:";
        for (int i=0; i<tipSageti::NrTipuri; ++i) {
            if (SagetiGasite[i]){
                switch (i) {
                    case tipSageti::Normala:
                        os<<"Normale";
                        break;
                    case tipSageti::Healing:
                        os << "Healing";
                        break;
                    case tipSageti::Aimbot:
                        os << "Aimbot";
                        break;
                    case tipSageti::Otravitoare:
                        os << "Otravitoare";
                        break;
                    case tipSageti::Giganta:
                        os << "Gigante";
                        break;
                    default:
                        os << "Tip invalid";
                        break;
                }
                if (i<tipSageti::NrTipuri-1) os << ", ";
            }
        }
        os << std::endl;
        return os;
    }
};

class Caracter {
    int hp;
    raylib::Rectangle rect;
    const char* PathTextura;
    raylib::Texture2D textura;
    Arc arc;
public:
    explicit Caracter(const char* PathTextura="../assets/textures/pacman3.png", int hp=100,
        float posX=0, float posY=0) : hp(hp),PathTextura(PathTextura), textura(PathTextura), arc() {
        rect=raylib::Rectangle(posX, posY,
            static_cast<float>(textura.GetWidth()), static_cast<float>(textura.GetHeight()));
    }
    explicit Caracter(const Arc& arc, const char* PathTextura="../assets/textures/pacman3.png", int hp=100,
        float posX=0, float posY=0) : hp(hp),PathTextura(PathTextura), textura(PathTextura), arc(arc) {
        rect=raylib::Rectangle(posX, posY,
            static_cast<float>(textura.GetWidth()), static_cast<float>(textura.GetHeight()));
    }
    Caracter(const Caracter &other) : hp(other.hp), rect(other.rect), PathTextura(other.PathTextura)
    , textura(other.PathTextura){}
    Caracter& operator=(const Caracter &other) {
        if (this != &other) {
            hp=other.hp;
            rect=other.rect;
            PathTextura=other.PathTextura;
            textura.Load(other.textura.GetData());
        }
        return *this;
    }
    [[nodiscard]] Arc get_arc() const {return arc;}
    void MutaCaracter() {
        for (int i=0; i<4; ++i)
            if (raylib::Keyboard::IsKeyDown(miscare::MoveKeys[i]))
                rect.x+=miscare::ChangePos[i].x, rect.y+=miscare::ChangePos[i].y;
    }
    void DeseneazaCaracter(float rotation=0, float scale=0.1f,
        raylib::Color tint={255,255,255,255})
    const {
        textura.Draw({rect.x, rect.y}, rotation, scale, tint);
    }
};
class Bloc {
    raylib::Rectangle rect;
public:
    explicit Bloc(float posX=0, float posY=0, float Width=0, float Height=0) :
    rect(posX, posY, Width, Height) {}
    Bloc(const Bloc& Bloc) = default;
    Bloc& operator=(const Bloc& Bloc) {
        if (this != &Bloc) {
            rect=Bloc.rect;
        }
        return *this;
    }
};

int main() {
    raylib::Window window(800, 600, "test");
    window.SetTargetFPS(60);
    Caracter c;
    std::cout << c.get_arc();
    while (!window.ShouldClose()) {
        window.BeginDrawing();
        ClearBackground(BLACK);
        c.DeseneazaCaracter(90, 0.1f);
        c.MutaCaracter();
        window.EndDrawing();
    }

    return 0;
}