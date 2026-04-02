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
// namespace fizica {
//     constexpr float g = 9.8;
//     constexpr float MaxForta = 10;
//     constexpr float MaxDrawLength = 5*1.41;
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


//} //unghiul trebuie dat in radiani
enum tipSageti : unsigned char {
    Normala,
    Otravitoare,
    Aimbot,
    Healing,
    Giganta,
    NrTipuri,
    Invalid
};
class Sageata {
    tipSageti tip;
    static constexpr std::array<float, tipSageti::NrTipuri> damage{5, 2.5, 5, -10, 10};
    raylib::Vector2 pos;

public:

    explicit Sageata(tipSageti tip=Normala, float posX=-1, float posY=-1) :
    tip(tip), pos(posX, posY) {}
    Sageata(const Sageata& AltaSageata) = default;
    Sageata& operator=(const Sageata& AltaSageata) {
        if (this != &AltaSageata) {
            tip = AltaSageata.tip;
            pos = AltaSageata.pos;
        }
        return *this;
    }

    ~Sageata() {
        tip = Invalid;
        pos.x=pos.y=-1;
    }
    [[nodiscard]] tipSageti get_tip() const {return tip;}

    [[nodiscard]] raylib::Vector2 get_pos() const {return pos;}

    [[nodiscard]] static constexpr std::array<float, tipSageti::NrTipuri> get_damage_array() {return damage;}


    friend std::ostream& operator<< (std::ostream& os, const Sageata& s) {
        os << "Pozitia: (" << s.pos.x << ", " << s.pos.y << ")" << '\n';
        os << "Tipul sagetii: ";
        switch (s.tip) {
            case Normala:
                os << "Normala";
                break;
            case Otravitoare:
                os << "Otravitoare";
                break;
            case Aimbot:
                os << "Aimbot";
                break;
            case Healing:
                os << "Healing";
                break;
            case Giganta:
                os << "Giganta";
                break;
            default:
                os << "Tip necunoscut!";
                break;
        }
        os << std::endl;
        return os;
    }
    void MutaSageata(float dx, float dy) {
        pos.x=dx, pos.y=dy;
    }
};
class Arc {
    unsigned long long CapacitateArc;
    std::vector<Sageata> Sageti;
    std::vector<Sageata> SagetiTrase;
    public:
    explicit Arc(unsigned long long CapacitateArc=20, tipSageti tip=Normala) :
    CapacitateArc(CapacitateArc), Sageti(CapacitateArc, Sageata(tip)) {
        SagetiTrase.reserve(CapacitateArc);
    }
    explicit Arc(const std::vector<Sageata>& Sageti){
        CapacitateArc=Sageti.size();
        this->Sageti=Sageti;
    }
    Arc(const Arc& Arc) = default;
    Arc& operator=(const Arc& Arc) {
        if (this != &Arc) {
            Sageti = Arc.Sageti;
            CapacitateArc = Arc.CapacitateArc;
            SagetiTrase=Arc.SagetiTrase;
        }
        return *this;
    }

    // [[nodiscard]] unsigned long long get_capacitate_arc() const {return CapacitateArc;}

    // [[nodiscard]] std::vector<Sageata> const& get_sageti() const {return Sageti;}

    [[nodiscard]] std::vector<Sageata> const& get_sageti_trase() {return SagetiTrase;}

    void TrageLa(raylib::Vector2 pozitie) {
        Sageata s = Sageti.back();
        s.MutaSageata(pozitie.x, pozitie.y);
        SagetiTrase.emplace_back(s);
        Sageti.pop_back();
    }

    void MutaSageataTrasa(float dx, float dy, size_t i) {
        SagetiTrase[i].MutaSageata(SagetiTrase[i].get_pos().x+dx, SagetiTrase[i].get_pos().y+dy);
    }
    void StergeSageataTrasa(size_t i) {
        // SagetiTrase[SagetiTrase.size()-1].MutaSageata(-1, -1);
        if (i<SagetiTrase.size())
        SagetiTrase.erase(SagetiTrase.begin() + static_cast<long long>(i));
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
    float hp;
    raylib::Rectangle rect;
    const char* PathTextura;
    raylib::Texture2D textura;
    Arc arc;
    float scale;
public:
    explicit Caracter(float scale=1, float posX=0, float posY=0,
        const char* PathTextura="../assets/textures/pacman3.png", float hp=100):
    hp(hp), PathTextura(PathTextura), textura(PathTextura), arc(), scale(scale) {
        rect=raylib::Rectangle(posX, posY,
            static_cast<float>(textura.GetWidth())*scale, static_cast<float>(textura.GetHeight())*scale);
    }
    explicit Caracter(const Arc& arc, float scale=1, float posX=0, float posY=0,
        const char* PathTextura="../assets/textures/pacman3.png", float hp=100
        ) :hp(hp),PathTextura(PathTextura), textura(PathTextura), arc(arc), scale(scale) {
        rect=raylib::Rectangle(posX, posY,
            static_cast<float>(textura.GetWidth())*scale, static_cast<float>(textura.GetHeight())*scale);
    }
    Caracter(const Caracter &other) : hp(other.hp), rect(other.rect), PathTextura(other.PathTextura)
    , textura(other.PathTextura), arc(other.arc), scale(other.scale){}
    Caracter& operator=(const Caracter &other) {
        if (this != &other) {
            hp=other.hp;
            rect=other.rect;
            PathTextura=other.PathTextura;
            textura.Load(other.textura.GetData());
            arc=other.arc;
            scale=other.scale;
        }
        return *this;
    }
    [[nodiscard]] Arc& get_arc() {return arc;}
    // void MutaCaracter() {
    //     for (int i=0; i<4; ++i)
    //         if (raylib::Keyboard::IsKeyDown(miscare::MoveKeys[i]))
    //             rect.SetPosition(rect.GetPosition()+=miscare::ChangePos[i]);
    // }

    [[nodiscard]] float get_hp() const {
        return hp;
    }

    [[nodiscard]] raylib::Rectangle get_rect() const {
        return rect;
    }

    void DeseneazaCaracter(float rotation=0) const{
        textura.Draw(rect.GetPosition(), rotation, scale);
    }
    // void DeseneazaHitbox (float rotation=0) const {
    //     rect.Draw({0, 0}, rotation, {255, 0, 0, 100});
    // }
    // void VerColiziune (const Caracter& other) const {
    //     if (rect.CheckCollision(other.get_rect())) {
    //         DeseneazaHitbox();
    //         other.DeseneazaHitbox();
    //     }
    // }
    void IaDamage(float damage) {hp-=damage;}
    friend std::ostream& operator<< (std::ostream& os, const Caracter& c) {
        if (c.textura.IsValid()) {
            os << "Textura incarcata cu succes din " << c.PathTextura << ".\n";
        }
        else {
            os << "Ceva este gresit cu textura..." << '\n';
            return os;
        }
        os << "Hp: " << c.hp << '\n' << "Pozitie: " << '(' << c.rect.x << ", "  << c.rect.y << ")\n"
        << "Hitbox: " << '(' << c.rect.width << ", " << c.rect.height << ")" << std::endl;
        return os;
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
    friend std::ostream& operator<< (std::ostream& os, const Bloc& b) {
        return os << "Pozitie: " << '(' << b.rect.x << ", " << b.rect.y << ")\n" <<
            "Marimi: " << '(' << b.rect.width << ", " << b.rect.height << ")" << std::endl;
    }
};

class GameDemo {
    const int windowWidth = 800;
    const int windowHeight = 450;
    raylib::Window window;
    Arc arc{std::vector<Sageata>(50, Sageata(Giganta))};
    Caracter player{0.1f, 0.0f, static_cast<float>(windowHeight)/2};
    Caracter inamic{0.1f, static_cast<float>(windowWidth)-player.get_rect().width,
        static_cast<float>(windowHeight)/2};
public:
    GameDemo() : window(windowWidth, windowHeight) {std::cout << player << std::endl;}

    static bool Nimerit(const Sageata& s, Caracter& c) {
        if (c.get_rect().CheckCollision(raylib::Vector2{s.get_pos().x+20, s.get_pos().y})) {
            c.IaDamage(Sageata::get_damage_array()[s.get_tip()]);
            return true;
        }
        return false;
    }
    void run() {
        Arc arc_inamic(inamic.get_arc());
        window.SetTargetFPS(60);
        while (!window.ShouldClose()) {
            float PlayerHp=player.get_hp(), InamicHp=inamic.get_hp();
                window.BeginDrawing();
            if (PlayerHp>0 && InamicHp>0) {
                window.ClearBackground(RAYWHITE);
                player.DeseneazaCaracter();
                inamic.DeseneazaCaracter();
                std::vector<Sageata> trase=arc.get_sageti_trase();
                if (raylib::Keyboard::IsKeyPressed('C')) {
                    arc.TrageLa(player.get_rect().GetPosition());
                }
                for (size_t i=0; i<trase.size(); ++i) {
                    bool nimerit = Nimerit(trase[i], inamic);
                    float fstartX = trase[i].get_pos().GetX();
                    float fstartY = trase[i].get_pos().GetY();
                    int startX=static_cast<int>(fstartX);
                    int startY=static_cast<int>(fstartY);
                    arc.MutaSageataTrasa(20, 0, i);
                    DrawRectangle(startX, startY+100, 20, 20, BLUE);
                    if (nimerit) {
                        arc.StergeSageataTrasa(i);
                        trase.erase(trase.begin()+static_cast<long long>(i));
                        std::cout << inamic << std::endl;
                    }
                }
            }
            else {
                ClearBackground(RAYWHITE);
                const std::string mesaj="GAME OVER";
                DrawText(mesaj.c_str(), windowWidth/2, windowHeight/2, 20, BLACK);
            }
                window.EndDrawing();
        }
    }
};

int main() {
    GameDemo game;
    game.run();
    return 0;
}