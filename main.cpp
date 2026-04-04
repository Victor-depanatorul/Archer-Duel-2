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
        tip = Invalid;
        pos.x=pos.y=-1;
    }
    [[nodiscard]] tipSageti get_tip() const {return tip;}

    [[nodiscard]] raylib::Vector2 get_pos() const {return pos;}

    [[nodiscard]] static float get_damage(tipSageti tip) {return damage.at(tip);}

    [[nodiscard]] static Color get_color(tipSageti tip) {return culori.at(tip);}
    void TintaAimbot(raylib::Vector2 pozitie) {
        pos=pozitie;
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

    [[nodiscard]] std::vector<Sageata>& get_sageti_trase() {return SagetiTrase;}

    void TrageLa(raylib::Vector2 pozitie) {
        if (Sageti.empty()) return;
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
    float timer_otrava=0.0f;
    float ticks_otrava=0.0f;
    static constexpr float dps_otrava=1.5f;
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
    friend std::ostream& operator<< (std::ostream& os, const Caracter& c) {
        os << "Hp: " << c.hp << '\n' << "Pozitie: " << '(' << c.rect.x << ", "  << c.rect.y << ")\n"
        << "Hitbox: " << '(' << c.rect.width << ", " << c.rect.height << ")" << std::endl;
        return os;
    }
};
class Bloc {
    Caracter& owner;
    raylib::Rectangle rect;
    float lifespan;
    bool trebuie_sters=false;
public:
    explicit Bloc(Caracter& owner, float posX=0, float posY=0, float Width=0, float Height=0, float durata=2.0f) :
    owner(owner), rect(posX, posY, Width, Height), lifespan(durata) {}
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

    [[nodiscard]] bool este_owner(const Caracter& c) const {return &c==&owner;}

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
        s_aimbot, s_giganta, s_aimbot, s_normala, s_lifesteal, s_giganta};
    Arc arc{sageti_test};
    Caracter player{arc, 0.1f, 0.0f, static_cast<float>(windowHeight)/2};
    Caracter inamic{0.1f, static_cast<float>(windowWidth)-player.get_rect().width,
        static_cast<float>(windowHeight)/2, "../assets/textures/pacman_intors.png"};
    static inline std::vector<Bloc> ziduri;
    float cooldown_zid_player=0.0f, cooldown_zid_inamic=0.0f;
    static bool ColiziuneZid(const Caracter& c, raylib::Vector2 punct) {
        return std::ranges::any_of(ziduri.begin(), ziduri.end(),
            [&punct, &c](const Bloc& b) {return !b.este_owner(c) && b.get_rect().CheckCollision(punct);});
    }
    static bool Nimerit(const Sageata& s, Caracter& c) {
        if (c.get_rect().CheckCollision(raylib::Vector2{s.get_pos().x+20, s.get_pos().y})) {
            c.IaDamage(Sageata::get_damage(s.get_tip()));
            if (s.get_tip()==tipSageti::Otravitoare) c.AplicaOtrava(5.0f);
            return true;
        }
        return false;
    }
    static void UpdateSageti(Caracter& c, Caracter& tinta, float dir) {
        Arc& arc_ch=c.get_arc();
        std::vector<Sageata>& trase=arc_ch.get_sageti_trase();
        for (int i=static_cast<int>(trase.size())-1; i>=0; --i) {
            if (trase[i].get_tip()==tipSageti::Aimbot)
                trase[i].TintaAimbot(tinta.get_rect().GetPosition());
            arc_ch.MutaSageataTrasa(dir, 0, i);
            raylib::Vector2 varf={trase[i].get_pos().x+(dir>0 ? 20.0f:0.0f), trase[i].get_pos().y};
            int w=20, h=10;
            if (trase[i].get_tip()==tipSageti::Giganta) w<<=2, h<<=2;
            DrawRectangle(static_cast<int>(varf.x), static_cast<int>(varf.y+c.get_rect().y/2), w, h,
                Sageata::get_color(trase[i].get_tip()));
            if (ColiziuneZid(c, varf)) {
                arc_ch.StergeSageataTrasa(i);
            }
            if (Nimerit(trase[i], tinta)) {
                if (trase[i].get_tip()==tipSageti::LifeSteal) {
                    c.IaDamage(-Sageata::get_damage(trase[i].get_tip()));
                }
                arc_ch.StergeSageataTrasa(i);
                std::cout << tinta << std::endl;
            }
            else if (trase[i].get_pos().x > windowWidth || trase[i].get_pos().x < 0.0f) {
                arc_ch.StergeSageataTrasa(i);
            }
        }
    }
public:
    GameDemo() : window(windowWidth, windowHeight) {std::cout << player << std::endl;}
    void run() {
        Arc arc_inamic(inamic.get_arc());
        window.SetTargetFPS(60);
        while (!window.ShouldClose()) {
            float PlayerHp=player.get_hp(), InamicHp=inamic.get_hp();
                window.BeginDrawing();
            if (PlayerHp>0 && InamicHp>0) {
                float dt=window.GetFrameTime();
                player.UpdateEfect(dt);
                inamic.UpdateEfect(dt);
                window.ClearBackground(RAYWHITE);
                for (int i=static_cast<int>(ziduri.size())-1; i>=0; --i) {
                    ziduri[i].Update(dt);
                    if (ziduri[i].TrebuieSters()) ziduri.erase(ziduri.begin()+i);
                }
                if (cooldown_zid_player>0) cooldown_zid_player-=dt;
                if (cooldown_zid_inamic>0) cooldown_zid_inamic-=dt;
                if (raylib::Keyboard::IsKeyPressed('P') && cooldown_zid_player<=0) {
                    ziduri.emplace_back(player,
                        player.get_rect().x+50.0f+player.get_rect().width, player.get_rect().y-10.0f,
                        15.0f, player.get_rect().height, 2.0f);
                    cooldown_zid_player=5.0f;
                }
                if (MyRand(1,20)==1 && cooldown_zid_inamic<=0) {
                    ziduri.emplace_back(inamic,
                        inamic.get_rect().x-20.0f, player.get_rect().y-10.0f,
                        15.0f, player.get_rect().height, 2.0f);
                    cooldown_zid_inamic=5.0f;
                }
                player.DeseneazaCaracter();
                inamic.DeseneazaCaracter();
                for (const Bloc& b : ziduri) b.Deseneaza();
                if (raylib::Keyboard::IsKeyPressed('C'))
                    player.get_arc().TrageLa(player.get_rect().GetPosition());
                UpdateSageti(player, inamic, 20.0f);
                if (MyRand(1,100)==1)
                    inamic.get_arc().TrageLa(inamic.get_rect().GetPosition());
                UpdateSageti(inamic, player, -20.0f);
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