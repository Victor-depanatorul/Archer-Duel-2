#include <algorithm>
#include <iostream>
#include <array>
#include <chrono>
#include <random>
#include <vector>
#include <raylib-cpp.hpp>

namespace miscare {
    // constexpr char MoveKeys[5] = "WASD";
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
int MyRand(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    return std::uniform_int_distribution<>{min, max}(gen);
}
enum tipSageti : unsigned char {
    Normala,
    Otravitoare,
    Aimbot,
    Healing,
    Giganta,
    LifeSteal,
    NrTipuri,
    Invalid
};
class Sageata {
    tipSageti tip;
    static constexpr std::array<float, tipSageti::NrTipuri> damage{5, 3.5, 5, -10, 10, 5};
    static constexpr std::array<Color, tipSageti::NrTipuri> culori{BLUE, VIOLET, BLACK, GREEN, BLUE, RED};
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
        pos.x=pos.y=-1;
    }
    [[nodiscard]] tipSageti get_tip() const {return tip;}

    [[nodiscard]] raylib::Vector2 get_pos() const {return pos;}

    [[nodiscard]] static float get_damage(tipSageti tip) {
        if (tip!=tipSageti::Invalid)
            return damage.at(tip);
        return 0;
    }

    [[nodiscard]] static Color get_color(tipSageti tip) {
        if (tip!=tipSageti::Invalid)
            return culori.at(tip);
        return WHITE;
    }
    void TintaAimbot(raylib::Vector2 pozitie) {
        pos=raylib::Vector2{pozitie.x-20, pozitie.y};
    }
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

    void MiscaSageata(float dx, float dy) {
        pos.x+=dx, pos.y+=dy;
    }
};
class Arc {
    unsigned long long CapacitateArc;
    std::vector<Sageata> Sageti;
    public:
    explicit Arc(unsigned long long CapacitateArc=20, tipSageti tip=Normala) :
    CapacitateArc(CapacitateArc), Sageti(CapacitateArc, Sageata(tip)) {}
    explicit Arc(const std::vector<Sageata>& Sageti) : CapacitateArc(Sageti.size()), Sageti(Sageti){
    }
    Arc(const Arc& Arc) = default;
    Arc& operator=(const Arc& Arc) {
        if (this != &Arc) {
            Sageti = Arc.Sageti;
            CapacitateArc = Arc.CapacitateArc;
        }
        return *this;
    }

    [[nodiscard]] unsigned long long get_capacitate() const {return CapacitateArc;}

    // [[nodiscard]] std::vector<Sageata> const& get_sageti() const {return Sageti;}

    [[nodiscard]] bool AreSageti() const {return !Sageti.empty();}

    Sageata Trage() {
        if (AreSageti()) {
            Sageata s = Sageti.back();
            Sageti.pop_back();
            return s;
        }
        return Sageata(tipSageti::Invalid);
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
    float scale;
    const char* PathTextura;
    raylib::Texture2D textura;
    raylib::Rectangle rect;
    Arc arc;
    std::vector<Sageata> sageti_trase;
    static std::vector<Caracter*> caractere;
    float timer_otrava=0.0f;
    float ticks_otrava=0.0f;
    static constexpr float dps_otrava=1.5f;
public:
    explicit Caracter(float scale=1.0f, float posX=0.0f, float posY=0.0f,
        const char* PathTextura="../assets/textures/pacman3.png", float hp=100):
    hp(hp), scale(scale), PathTextura(PathTextura), textura(PathTextura),
    rect(posX, posY,
         static_cast<float>(textura.GetWidth()) * scale,
         static_cast<float>(textura.GetHeight()) * scale), arc(){
        sageti_trase.reserve(arc.get_capacitate());
    }

    explicit Caracter(const Arc& arc, float scale=1.0f, float posX=0.0f, float posY=0.0f,
        const char* PathTextura="../assets/textures/pacman3.png", float hp=100
        ) :hp(hp), scale(scale), PathTextura(PathTextura), textura(PathTextura),
    rect(posX, posY,
     static_cast<float>(textura.GetWidth()) * scale,
     static_cast<float>(textura.GetHeight()) * scale), arc(arc){
        sageti_trase.reserve(arc.get_capacitate());
    }

    Caracter(const Caracter &other) : hp(other.hp), scale(other.scale), PathTextura(other.PathTextura)
    , textura(other.PathTextura), rect(other.rect), arc(other.arc){}
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

    ~Caracter() {
        std::erase(caractere, this);
    }
    // void MutaCaracter() {
    //     for (int i=0; i<4; ++i)
    //         if (raylib::Keyboard::IsKeyDown(miscare::MoveKeys[i]))
    //             rect.SetPosition(rect.GetPosition()+=miscare::ChangePos[i]);
    // }
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

    static void InregistreazaCaracter(Caracter& c) {
            auto it =
                std::ranges::find(caractere.begin(), caractere.end(), &c);
            if (it == caractere.end()) {
                caractere.emplace_back(&c);
            }
    }
    void IaDamage(float damage) {hp-=damage;}
    void AplicaOtrava(float durata){timer_otrava=durata;}
    void UpdateEfect(float deltaTime=0) {
        if (timer_otrava > 0.0f) {
            timer_otrava-=deltaTime;
            ticks_otrava+=deltaTime;
            if (ticks_otrava >= 1.0f) {
                IaDamage(dps_otrava);
                ticks_otrava=0.0f;
            }
        }
        else ticks_otrava=0.0f;
    }

    void Trage(const uint8_t dir, const Caracter* tinta) /*Se pune nullptr daca nu are o tinta */ {
        if (arc.AreSageti()) {
            Sageata s = arc.Trage();
            if (s.get_tip()==tipSageti::Aimbot && tinta!=nullptr) {
                s.TintaAimbot(tinta->get_rect().GetPosition());
            }
            else {
                auto fdir=static_cast<float>(dir);
                s.MutaSageata(rect.x+(rect.width*fdir+25)-(50*(1-fdir)), rect.y);
            }
            sageti_trase.emplace_back(s);
        }
    }

    bool Nimerit(const Sageata& s) {
        raylib::Vector2 varf=raylib::Vector2(s.get_pos().x, s.get_pos().y);
        if (rect.CheckCollision(varf)) {
            hp-=Sageata::get_damage(s.get_tip());
            if (s.get_tip()==tipSageti::Otravitoare) AplicaOtrava(5.0f);
            return true;
        }
        return false;
    }

    void UpdateSagetiTrase(float dir, std::vector<raylib::Rectangle>& others) {
        for (int i=static_cast<int>(sageti_trase.size())-1; i>=0; --i) {
            Sageata& s=sageti_trase[i];
            s.MiscaSageata(dir, 0);
            float w=20, h=10;
            if (s.get_tip() == Giganta) w*=2, h*=2;
            raylib::Rectangle r(s.get_pos(), raylib::Vector2(w, h));
            r.Draw(Sageata::get_color(s.get_tip()));
            for (Caracter* c : caractere)
                if (c->Nimerit(s)) {
                    sageti_trase.erase(sageti_trase.begin()+i);
                    std::cout << *c << std::endl;
                    break;
                }
            for (raylib::Rectangle& re : others) {
                if (re.CheckCollision(s.get_pos())) {
                    sageti_trase.erase(sageti_trase.begin()+i);
                    break;
                }
            }
        }
    }

    [[nodiscard]] bool InViata() const {return hp>0;}
    friend std::ostream& operator<< (std::ostream& os, const Caracter& c) {
        os << "Hp: " << c.hp << '\n' << "Pozitie: " << '(' << c.rect.x << ", "  << c.rect.y << ")\n"
        << "Hitbox: " << '(' << c.rect.width << ", " << c.rect.height << ")" << std::endl;
        return os;
    }
};
class Bloc {
    // Caracter& owner;
    raylib::Rectangle rect;
    float lifespan;
    bool trebuie_sters=false;
public:
    explicit Bloc(float posX=0, float posY=0, float Width=0, float Height=0, float durata=2.0f) :
    rect(posX, posY, Width, Height), lifespan(durata) {}
    Bloc(const Bloc& Bloc) = default;
    Bloc& operator=(const Bloc& Bloc) {
        if (this != &Bloc) {
            rect=Bloc.rect;
            lifespan=Bloc.lifespan;
            trebuie_sters=Bloc.trebuie_sters;
        }
        return *this;
    }
    void Update(float dt) {
        lifespan -= dt;
        if (lifespan <= 0) trebuie_sters = true;
    }
    void Deseneaza() const {
        rect.Draw(raylib::Color(120, 120, 120, 200));
        rect.DrawLines(DARKGRAY);
    }
        [[nodiscard]] bool TrebuieSters() const { return trebuie_sters; }
        [[nodiscard]] raylib::Rectangle get_rect() const { return rect; }

    // [[nodiscard]] bool este_owner(const Caracter& c) const {return &c==&owner;}

    friend std::ostream& operator<< (std::ostream& os, const Bloc& b) {
        return os << "Pozitie: " << '(' << b.rect.x << ", " << b.rect.y << ")\n" <<
            "Marimi: " << '(' << b.rect.width << ", " << b.rect.height << ")" << std::endl;
    }
};

class GameDemo {
    static constexpr int windowWidth = 800;
    static constexpr int windowHeight = 450;
    raylib::Window window;
    Sageata s_normala{tipSageti::Normala};
    Sageata s_otravitoare{tipSageti::Otravitoare};
    Sageata s_aimbot{tipSageti::Aimbot};
    Sageata s_giganta{tipSageti::Giganta};
    Sageata s_lifesteal{tipSageti::LifeSteal};
    std::vector<Sageata> sageti_test{s_normala, s_normala, s_aimbot, s_otravitoare,
        s_aimbot, s_giganta, s_aimbot, s_normala, s_lifesteal, s_giganta, s_aimbot};
    Arc arc{sageti_test};
    Caracter player{arc, 0.1f, 0.0f, static_cast<float>(windowHeight)/2};
    Caracter inamic{0.1f, static_cast<float>(windowWidth)-player.get_rect().width,
    static_cast<float>(windowHeight)/2, "../assets/textures/pacman_intors.png"};
    static inline std::vector<Bloc> ziduri;
    float cooldown_zid_player=0.0f, cooldown_zid_inamic=0.0f;

    static void UpdateSageti(Caracter& p, Caracter& i, std::vector<raylib::Rectangle>& rectangles) {
        p.UpdateSagetiTrase(20, rectangles);
        i.UpdateSagetiTrase(-20, rectangles);
    }
public:
    GameDemo() : window(windowWidth, windowHeight){
        Caracter::InregistreazaCaracter(player);
        Caracter::InregistreazaCaracter(inamic);
    }
    void run() {
        window.SetTargetFPS(60);
        while (!window.ShouldClose()) {
                window.BeginDrawing();
            if (player.InViata() && inamic.InViata()) {
                float dt=window.GetFrameTime();
                player.UpdateEfect(dt);
                inamic.UpdateEfect(dt);
                window.ClearBackground(RAYWHITE);
                std::vector<raylib::Rectangle> others;
                for (const Bloc& b: ziduri)
                    others.emplace_back(b.get_rect());
                UpdateSageti(player, inamic, others);
                for (int i=static_cast<int>(ziduri.size())-1; i>=0; --i) {
                    ziduri[i].Update(dt);
                    if (ziduri[i].TrebuieSters()) ziduri.erase(ziduri.begin()+i);
                }
                if (cooldown_zid_player>0) cooldown_zid_player-=dt;
                if (cooldown_zid_inamic>0) cooldown_zid_inamic-=dt;
                if (raylib::Keyboard::IsKeyPressed('P') && cooldown_zid_player<=0) {
                    ziduri.emplace_back(
                        player.get_rect().x+50.0f+player.get_rect().width, player.get_rect().y-10.0f,
                        15.0f, player.get_rect().height, 2.0f);
                    cooldown_zid_player=5.0f;
                }
                if (MyRand(1,20)==1 && cooldown_zid_inamic<=0) {
                    ziduri.emplace_back(
                        inamic.get_rect().x-20.0f, player.get_rect().y-10.0f,
                        15.0f, player.get_rect().height, 2.0f);
                    cooldown_zid_inamic=5.0f;
                }
                player.DeseneazaCaracter();
                inamic.DeseneazaCaracter();
                for (const Bloc& b : ziduri) b.Deseneaza();
                if (raylib::Keyboard::IsKeyPressed('C'))
                   player.Trage(1, &inamic);
                if (MyRand(1,100)==1)
                    inamic.Trage(0, &player);
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

std::vector<Caracter*> Caracter::caractere;

int main() {
    GameDemo game;
    game.run();
    return 0;
}