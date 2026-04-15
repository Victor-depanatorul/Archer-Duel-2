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
namespace fizica {
    constexpr float gravitate=980.0f;
}

bool TrecereTura=false;

// int MyRand(int min, int max) {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     return std::uniform_int_distribution<>{min, max}(gen);
// }
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

enum GameStates {
    StartMenu,
    Controale,
    TuraPlayer,
    Intermediar,
    TuraInamic,
    PauseMenu,
    GameOver
};
std::string StareToStr(GameStates stare) {
    switch (stare) {
        case GameStates::StartMenu: return "Meniu de Start";
        case GameStates::Controale: return "Controale";
        case GameStates::TuraPlayer: return "Tura Player 1";
        case GameStates::TuraInamic: return "Tura Player 2";
        case GameStates::Intermediar: return "Intre Ture";
        case GameStates::PauseMenu: return "Pauza";
        case GameStates::GameOver: return "Game Over";
        default: return "Nedefinit";
    }
}
std::string GetNumeTip(tipSageti tip) {
    switch (tip) {
        case tipSageti::Normala:     return "Normala";
        case tipSageti::Otravitoare: return "Otravitoare";
        case tipSageti::Aimbot:      return "Aimbot";
        case tipSageti::Healing:     return "Healing";
        case tipSageti::Giganta:     return "Giganta";
        case tipSageti::LifeSteal:   return "LifeSteal";
        default:          return "Epuizat";
    }
}
class Sageata {
    tipSageti tip;
    raylib::Vector2 pos;
    raylib::Vector2 vel{0.0f, 0.0f};
    float varsta=0.0f;
    static constexpr std::array<float, tipSageti::NrTipuri> damage{5, 3.5, 5, -10, 10, 5};
    static constexpr std::array<Color, tipSageti::NrTipuri> culori{BLUE, VIOLET, BLACK, GREEN, BLUE, RED};

public:
    explicit Sageata(tipSageti tip=Normala, float posX=-1, float posY=-1) :
    tip(tip), pos(posX, posY){}
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

    [[nodiscard]] raylib::Vector2 get_velocity() const {
        return vel;
    }
    void CresteVarsta() {varsta+=0.1f;}

    [[nodiscard]] bool este_veche() const {return varsta>=2.0f;}
    void UpdateVelocity(float dx, float dy) {
    vel.x+=dx, vel.y+=dy;
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
            case LifeSteal:
                os << "LifeSteal";
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

    [[nodiscard]] unsigned long long get_capacitate() const {return CapacitateArc;}

    // [[nodiscard]] std::vector<Sageata> const& get_sageti() const {return Sageti;}

    [[nodiscard]] bool AreSageti() const {return !Sageti.empty();}

    [[nodiscard]] tipSageti VeziUrmatoarea() const {
        if (!Sageti.empty()) {
            return Sageti.back().get_tip();
        }
        return tipSageti::Invalid;
    }
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
        for (const Sageata& sageata : a.Sageti) {
            os << sageata << '\n';
        }
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
    int runde_otrava=0;
    static constexpr float dps_otrava=1.5f;
public:
    explicit Caracter(float scale=1.0f, float posX=0.0f, float posY=0.0f,
        const char* PathTextura="assets/textures/pacman3.png", float hp=100):
    hp(hp), scale(scale), PathTextura(PathTextura), textura(PathTextura),
    rect(posX, posY,
         static_cast<float>(textura.GetWidth()) * scale,
         static_cast<float>(textura.GetHeight()) * scale), arc(){
        sageti_trase.reserve(arc.get_capacitate());
    }

    explicit Caracter(const Arc& arc, float scale=1.0f, float posX=0.0f, float posY=0.0f,
        const char* PathTextura="assets/textures/pacman3.png", float hp=100)
        :hp(hp), scale(scale), PathTextura(PathTextura), textura(PathTextura),
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

    [[nodiscard]] float get_hp() const {return hp;}
    void DeseneazaCaracter(float rotation=0) const{
        textura.Draw(rect.GetPosition(), rotation, scale);
    }

    // În interiorul clasei Caracter:
    [[nodiscard]] tipSageti TipUrmatoareaSageata() const {
        return arc.VeziUrmatoarea();
    }
    // void DeseneazaHitbox (float rotation=0) const {
    //     rect.Draw({0, 0}, rotation, {255, 0, 0, 100});
    // }

    static void InregistreazaCaracter(Caracter& c) {
            auto it =
                std::ranges::find(caractere.begin(), caractere.end(), &c);
            if (it == caractere.end()) {
                caractere.emplace_back(&c);
            }
    }
    void IaDamage(float damage) {hp-=damage;}
    void AplicaOtrava(int runde){runde_otrava=std::max(runde_otrava+runde, 5);}
    void UpdateEfect() {
        if (runde_otrava>0) {
            IaDamage(dps_otrava);
            --runde_otrava;
        }
    }

    void Trage(raylib::Vector2 targetPos, float forta, const Caracter* tinta=nullptr) {
        if (arc.AreSageti()) {
            Sageata s = arc.Trage();
            raylib::Vector2 centru = {rect.x + rect.width / 2, rect.y + rect.height / 2};
            s.MutaSageata(centru.x, centru.y);
            raylib::Vector2 vitezaInitiala{0, 0};

            if (s.get_tip()==tipSageti::Aimbot && tinta!=nullptr) {
                raylib::Vector2 coordTinta = {tinta->get_rect().x + tinta->get_rect().width/2,
                                          tinta->get_rect().y + tinta->get_rect().height/2};

            // Deplasarea relativă față de țintă
            float dx = coordTinta.x - centru.x;
            float dy = centru.y - coordTinta.y;
            float v = forta; // Viteza lansării
            float g = fizica::gravitate;
            float v2 = v * v;
            float v4 = v2 * v2;
            float radical = v4 - g * (g * dx * dx + 2 * dy * v2);
            if (radical >= 0) {
                float unghi = std::atan2(v2 + std::sqrt(radical), g * dx);
                float directieX = (dx >= 0) ? 1.0f : -1.0f;
                vitezaInitiala.x = std::abs(std::cos(unghi) * v) * directieX;
                vitezaInitiala.y = -std::sin(unghi) * v;
            } else {
                // Forța e prea mică pentru a ajunge la țintă. Tragem normal spre coordonate.
                std::cout << "Aimbot: Tinta prea departe pentru forta curenta! Tragere normala.\n";
                float dist = std::sqrt(dx*dx + (centru.y - targetPos.y)*(centru.y - targetPos.y));
                if (dist > 0) {
                    vitezaInitiala.x = (dx / dist) * forta;
                    vitezaInitiala.y = ((targetPos.y - centru.y) / dist) * forta;
                }
            }
            }
            else {
                float dx = targetPos.x - centru.x;
                float dy = targetPos.y - centru.y;
                float d = std::sqrt(dx*dx + dy*dy);
                if (d > 0.0f) {
                    vitezaInitiala.x = (dx / d) * forta;
                    vitezaInitiala.y = (dy / d) * forta;
                }
            }
            s.UpdateVelocity(vitezaInitiala.x, vitezaInitiala.y);
            sageti_trase.emplace_back(s);
        }
    }

    bool Nimerit(const Sageata& s) {
        raylib::Vector2 varf=raylib::Vector2(s.get_pos().x, s.get_pos().y);
        if (rect.CheckCollision(varf)) {
            hp-=Sageata::get_damage(s.get_tip());
            if (s.get_tip()==tipSageti::Otravitoare) AplicaOtrava(2);
            return true;
        }
        return false;
    }

    void UpdateSagetiTrase(float dt, std::vector<raylib::Rectangle>& others, float max_height) {
        for (int i=static_cast<int>(sageti_trase.size())-1; i>=0; --i) {
            Sageata& s=sageti_trase[i];
            s.UpdateVelocity(0.0f, dt*fizica::gravitate);
            s.MiscaSageata(s.get_velocity().x*dt, s.get_velocity().y*dt);
            float w=20, h=10;
            if (s.get_tip() == Giganta) w*=2, h*=2;
            raylib::Rectangle r(s.get_pos(), raylib::Vector2(w, h));
            r.Draw(Sageata::get_color(s.get_tip()));
            for (Caracter* c : caractere) {
                if (c==this && !s.este_veche()) continue;
                if (c->Nimerit(s)) {
                    TrecereTura=true;
                    sageti_trase.erase(sageti_trase.begin()+i);
                    std::cout << *c << std::endl;
                    break;
                }
            }
            for (raylib::Rectangle& re : others) {
                if (re.CheckCollision(s.get_pos())) {
                    TrecereTura=true;
                    sageti_trase.erase(sageti_trase.begin()+i);
                    break;
                }
            }
            if (s.get_pos().y>max_height) {
                TrecereTura=true;
                sageti_trase.erase(sageti_trase.begin()+i);
            }
            s.CresteVarsta();
        }
    }

    void set_pozitie(float x, float y) {
        rect.x=x, rect.y=y;
    }

    [[nodiscard]] bool InViata() const {return hp>0;}

    [[nodiscard]] bool AreSageti() const {return arc.AreSageti();}
    friend std::ostream& operator<< (std::ostream& os, const Caracter& c) {
        os << "Hp: " << c.hp << '\n' << "Pozitie: " << '(' << c.rect.x << ", "  << c.rect.y << ")\n"
        << "Hitbox: " << '(' << c.rect.width << ", " << c.rect.height << ")" << std::endl;
        os << c.arc << std::endl;
        return os;
    }
};
class Bloc {
    raylib::Rectangle rect;
    int8_t lifespan=2;
public:
    explicit Bloc(float posX=0, float posY=0, float Width=0, float Height=0) :
    rect(posX, posY, Width, Height) {}

    void Update(){lifespan--;}
    void Deseneaza() const {
        rect.Draw(raylib::Color(120, 120, 120, 200));
        rect.DrawLines(DARKGRAY);
    }

    [[nodiscard]] raylib::Rectangle get_rect() const { return rect; }
    [[nodiscard]] bool TrebuieSters() const {return lifespan<=0;}

    friend std::ostream& operator<< (std::ostream& os, const Bloc& b) {
        return os << "Pozitie: " << '(' << b.rect.x << ", " << b.rect.y << ")\n" <<
            "Marimi: " << '(' << b.rect.width << ", " << b.rect.height << ")" << std::endl;
    }
};

class GameDemo {
    int windowWidth = 800;
    int windowHeight = 450;
    raylib::Window window;
    Sageata s_normala{tipSageti::Normala};
    Sageata s_otravitoare{tipSageti::Otravitoare};
    Sageata s_aimbot{tipSageti::Aimbot};
    Sageata s_giganta{tipSageti::Giganta};
    Sageata s_lifesteal{tipSageti::LifeSteal};
    Sageata s_healing{tipSageti::Healing};
    std::vector<Sageata> sageti_test{s_normala, s_normala, s_normala, s_normala, s_normala, s_normala, s_giganta,
    s_otravitoare, s_aimbot, s_lifesteal, s_healing};
    Arc arc{sageti_test};
    Caracter player{arc, 0.1f, 0.0f, static_cast<float>(windowHeight)/2};
    Caracter inamic{arc, 0.1f, static_cast<float>(windowWidth)-player.get_rect().width,
    static_cast<float>(windowHeight)/2, "assets/textures/pacman_intors.png"};
    static inline std::vector<Bloc> ziduri;

    bool trage_arc = false;
    float forta_tragere = 0.0f;
    const float max_forta_tragere = 1500.0f;
    const float viteza_trager = 800.0f;
    const float forta_de_baza = 400.0f;

    GameStates stare = GameStates::StartMenu;
    GameStates stareUrm = GameStates::TuraPlayer;
    GameStates starePrev = GameStates::TuraPlayer;
    int meniuSelectat = 0;
    bool close_window = false;

    void ResetGame() {
        player = Caracter(arc, 0.1f, 0.0f, static_cast<float>(windowHeight)/2);
        inamic = Caracter(arc, 0.1f, static_cast<float>(windowWidth)-player.get_rect().width,
    static_cast<float>(windowHeight)/2, "assets/textures/pacman_intors.png");
        ziduri.clear();
        stare = TuraPlayer;
        stareUrm = TuraInamic;
        starePrev = TuraPlayer;
    }

    static void UpdateSageti(Caracter& p, Caracter& i, std::vector<raylib::Rectangle>& rectangles, float dt, float max_height) {
        p.UpdateSagetiTrase(dt, rectangles, max_height);
        i.UpdateSagetiTrase(dt, rectangles, max_height);
    }

    static void DeseneazaButon(raylib::Rectangle rect, const char* text, bool selectat) {
        Color culoareBaza = selectat ? SKYBLUE : LIGHTGRAY;
        Color culoareText = selectat ? BLUE : DARKGRAY;

        rect.Draw(culoareBaza);
        rect.DrawLines(selectat ? BLUE : GRAY, 3);

        int textWidth = MeasureText(text, 20);
        DrawText(text, static_cast<int>(rect.width/2) - textWidth/2 + static_cast<int>(rect.x), static_cast<int>(rect.y) + 15, 20, culoareText);
    }

    void DeseneazaHUD() const {
        int fontSize = 20;
        int padding = 20;

        if (stare == TuraPlayer) {
            tipSageti tipP = player.TipUrmatoareaSageata();
            if (tipP != tipSageti::Invalid) {
                std::string textP = "Urmeaza: " + GetNumeTip(tipP);
                Color colP = Sageata::get_color(tipP);

                DrawText("TURA PLAYER 1", padding, padding, 15, DARKGRAY);
                DrawText(textP.c_str(), padding, padding+20, fontSize, colP);
            }
        }
        else if (stare == TuraInamic) {
            tipSageti tipI = inamic.TipUrmatoareaSageata();
            if (tipI != tipSageti::Invalid) {
                std::string textI = "Urmeaza: " + GetNumeTip(tipI);
                Color colI = Sageata::get_color(tipI);

                int textWidth = MeasureText(textI.c_str(), fontSize);
                int labelWidth = MeasureText("TURA PLAYER 2", 15);

                DrawText("TURA PLAYER 2", windowWidth - padding - labelWidth, padding, 15, DARKGRAY);
                DrawText(textI.c_str(), windowWidth - padding - textWidth, padding+20, fontSize, colI);
            }
        }
    }
    void DeseneazaStartMenu() {
        constexpr int optiuni_start = 3;
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            meniuSelectat = (meniuSelectat + 1) % optiuni_start;
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            meniuSelectat = (meniuSelectat - 1 + optiuni_start) % optiuni_start;
        }

        raylib::Rectangle btnStart(static_cast<float>(windowWidth) / 2 - 100, 150, 200, 50);
        raylib::Rectangle btnControale(static_cast<float>(windowWidth) / 2 - 100, 230, 200, 50);
        raylib::Rectangle btnExit(static_cast<float>(windowWidth) / 2 - 100, 310, 200, 50);


        if (btnStart.CheckCollision(GetMousePosition())) meniuSelectat = 0;
        if (btnControale.CheckCollision(GetMousePosition())) meniuSelectat = 1;
        if (btnExit.CheckCollision(GetMousePosition())) meniuSelectat = 2;

        if (IsKeyPressed(KEY_ENTER) ||
           (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
               (btnStart.CheckCollision(GetMousePosition()) ||
               btnControale.CheckCollision(GetMousePosition()) ||
               btnExit.CheckCollision(GetMousePosition()))))
            switch (meniuSelectat) {
            case 0:
                    stare = starePrev;
                    break;
            case 1:
                    stare = GameStates::Controale;
                    break;
            case 2:
                    close_window = true;
                    break;
            default:
                    DrawText("How did we get here?", windowWidth/2, windowHeight/2, 50, BLACK);
                    break;
            }

        DeseneazaButon(btnStart, "START GAME", meniuSelectat == 0);
        DeseneazaButon(btnControale, "CONTROALE", meniuSelectat == 1);
        DeseneazaButon(btnExit, "EXIT", meniuSelectat == 2);
    }

    void DeseneazaPauseMenu() {
        constexpr int optiuni_pauza = 4;
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) meniuSelectat = (meniuSelectat + 1) % optiuni_pauza;
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) meniuSelectat = (meniuSelectat - 1 + optiuni_pauza) % optiuni_pauza;

        raylib::Rectangle btnResume(static_cast<float>(windowWidth) / 2 - 100, 120, 200, 50);
        raylib::Rectangle btnRestart(static_cast<float>(windowWidth) / 2 - 100, 190, 200, 50);
        raylib::Rectangle btnControale(static_cast<float>(windowWidth) / 2 - 100, 260, 200, 50);
        raylib::Rectangle btnExit(static_cast<float>(windowWidth) / 2 - 100, 330, 200, 50);

        if (btnResume.CheckCollision(GetMousePosition())) meniuSelectat = 0;
        if (btnRestart.CheckCollision(GetMousePosition())) meniuSelectat = 1;
        if (btnControale.CheckCollision(GetMousePosition())) meniuSelectat = 2;
        if (btnExit.CheckCollision(GetMousePosition())) meniuSelectat = 3;

        if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
           (btnResume.CheckCollision(GetMousePosition()) || btnRestart.CheckCollision(GetMousePosition()) || 
            btnControale.CheckCollision(GetMousePosition()) || btnExit.CheckCollision(GetMousePosition())))) {
            
            if (meniuSelectat == 0) stare = starePrev; // Inapoi la joc
            else if (meniuSelectat == 1) ResetGame(); // Restart
            else if (meniuSelectat == 2) stare = Controale;
            else if (meniuSelectat == 3) close_window = true;
            }

        DeseneazaButon(btnResume, "RESUME", meniuSelectat == 0);
        DeseneazaButon(btnRestart, "RESTART", meniuSelectat == 1);
        DeseneazaButon(btnControale, "CONTROALE", meniuSelectat == 2);
        DeseneazaButon(btnExit, "EXIT", meniuSelectat == 3);
    }

    void DeseneazaGameOver() {
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            meniuSelectat = (meniuSelectat + 1) % 2;
        }

        float centerX = static_cast<float>(GetScreenWidth()) / 2.0f;
        float centerY = static_cast<float>(GetScreenHeight()) / 2.0f;

        raylib::Rectangle btnPlayAgain(centerX - 100, centerY, 200, 50);
        raylib::Rectangle btnExit(centerX - 100, centerY + 70, 200, 50);

        if (btnPlayAgain.CheckCollision(GetMousePosition())) meniuSelectat = 0;
        if (btnExit.CheckCollision(GetMousePosition())) meniuSelectat = 1;

        const char* titlu = "GAME OVER";
        int titluWidth = MeasureText(titlu, 40);
        DrawText(titlu, static_cast<int>(centerX) - titluWidth / 2, static_cast<int>(centerY) - 100, 40, RED);

        std::string castigator;
        if (player.InViata() && !inamic.InViata()) castigator="PLAYER CASTIGA";
        else if (inamic.InViata() && !player.InViata()) castigator="INAMIC CASTIGA";
        else if (!(player.InViata() || inamic.InViata())) castigator="EGALITATE";
        else if (!(player.AreSageti() || inamic.AreSageti())) {
            float p_hp=player.get_hp(), i_hp=inamic.get_hp();
            if (p_hp>i_hp) castigator="NICIUNUL NU MAI ARE SAGETI. PLAYER CASTIGA";
            else if (p_hp<i_hp) castigator="NICIUNUL NU MAI ARE SAGETI. INAMIC CASTIGA";
            else castigator="NICIUNUL NU MAI ARE SAGETI. EGALITATE";
        }
        int subtitluWidth = MeasureText(castigator.c_str(), 20);
        DrawText(castigator.c_str(), static_cast<int>(centerX) - subtitluWidth / 2, static_cast<int>(centerY) - 50, 20, DARKGRAY);

        if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           (btnPlayAgain.CheckCollision(GetMousePosition()) || btnExit.CheckCollision(GetMousePosition())))) {

            if (meniuSelectat == 0) {
                ResetGame();
                stare = TuraPlayer;
            } else {
                close_window = true;
            }
           }

        DeseneazaButon(btnPlayAgain, "PLAY AGAIN", meniuSelectat == 0);
        DeseneazaButon(btnExit, "EXIT GAME", meniuSelectat == 1);
    }

    void DeseneazaControale() {
        int textX = windowWidth / 2 - 250;
        int textY = windowHeight / 2 - 120;
        int spacing = 55;
        int fontSize = 20;
        int offsetDescriere = 150;
        const char* titlu = "CONTROALE JOC";
        int titluWidth = MeasureText(titlu, 30);
        DrawText(titlu, windowWidth / 2 - titluWidth / 2, textY - 60, 30, DARKGRAY);
        DrawText("ESCAPE:", textX, textY, fontSize, BLUE);
        DrawText("Deschide Main Menu.", textX + offsetDescriere, textY, fontSize, DARKGRAY);
        textY += spacing;
        DrawText("LEFT CLICK:", textX, textY, fontSize, BLUE);
        DrawText("Pregateste tragerea cu arcul.", textX + offsetDescriere, textY, fontSize, DARKGRAY);
        DrawText("Cu cat tii apasat mai mult, cu atat creste forta.", textX + offsetDescriere, textY + 25, 18, GRAY);
        textY += spacing + 25;
        DrawText("C:", textX, textY, fontSize, BLUE);
        DrawText("Anuleaza actiunea de tragere cu arcul.", textX + offsetDescriere, textY, fontSize, DARKGRAY);
        textY += spacing;
        DrawText("P:", textX, textY, fontSize, BLUE);
        DrawText("Spawneaza un perete pentru urmatoarele 2 ture.", textX + offsetDescriere, textY, fontSize, DARKGRAY);
        raylib::Rectangle btnBack(static_cast<float>(windowWidth) / 2 - 100, static_cast<float>(windowHeight) - 80, 200, 50);
        bool hover = btnBack.CheckCollision(GetMousePosition());
        DeseneazaButon(btnBack, "INAPOI", hover);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hover) {
            stare = GameStates::StartMenu;
        }
    }

    void Logica(Caracter& c1, Caracter& c2, float offset_zid, float dt, float max_height) {
        c1.DeseneazaCaracter();
        c2.DeseneazaCaracter();
        std::vector<raylib::Rectangle> others;
        for (int i = static_cast<int>(ziduri.size()) - 1; i >= 0; --i) {
            const Bloc& b = ziduri[i];
            others.emplace_back(b.get_rect());
            b.Deseneaza();
        }
        if (stare!=GameStates::Intermediar) {
            c1.UpdateEfect();
            if (raylib::Keyboard::IsKeyPressed('P'))
                ziduri.emplace_back(
                                    c1.get_rect().x+offset_zid, c1.get_rect().y-10.0f,
                                    15.0f, c1.get_rect().height
                                    );
            if (!c1.AreSageti()) {
                std::cout << "Nu mai ai sageti!\n";
                stare=stareUrm;
                return;
            }
            if (raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
                trage_arc=true;
                forta_tragere=forta_de_baza;
            }
            if (trage_arc) {
                if (raylib::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT)) {
                    forta_tragere+=viteza_trager*dt;
                    forta_tragere=std::min(forta_tragere,max_forta_tragere);
                }
                if (raylib::Keyboard::IsKeyPressed('C')) {
                    trage_arc=false;
                    forta_tragere=0.0f;
                }
                if (trage_arc && raylib::Mouse::IsButtonReleased(MOUSE_BUTTON_LEFT)) {
                    c1.Trage(raylib::Mouse::GetPosition(), forta_tragere, &c2);
                    trage_arc=false;
                    stare=GameStates::Intermediar;
                }
                raylib::Vector2 pCenter = {c1.get_rect().x + c1.get_rect().width / 2,
                                           c1.get_rect().y + c1.get_rect().height / 2};
                raylib::Vector2 mousePos = GetMousePosition();
                float dx = mousePos.x - pCenter.x;
                float dy = mousePos.y - pCenter.y;
                float dist = std::sqrt(dx*dx + dy*dy);

                if (dist > 0) {
                    raylib::Vector2 simViteza;
                    simViteza.x = (dx / dist) * forta_tragere;
                    simViteza.y = (dy / dist) * forta_tragere;

                    raylib::Vector2 punctCurent = pCenter;
                    raylib::Vector2 punctUrmator;
                    constexpr int maxPuncte = 80;

                    Color culoareTraiectorie = ColorAlpha(RED, forta_tragere/max_forta_tragere + 0.2f);

                    for (int i = 0; i < maxPuncte; i++) {
                        float pasTimp = 0.03f;
                        simViteza.y += fizica::gravitate * pasTimp; // Gravitația trage în jos viteza
                        punctUrmator.x = punctCurent.x + simViteza.x * pasTimp;
                        punctUrmator.y = punctCurent.y + simViteza.y * pasTimp;

                        float grosime = 3.0f * (1.0f - static_cast<float>(i)/static_cast<float>(maxPuncte));
                        DrawLineEx(punctCurent, punctUrmator, grosime, culoareTraiectorie);

                        punctCurent = punctUrmator;
                    }
                }
            }
        }
        else {
            UpdateSageti(c1, c2, others, dt, max_height);
        }
    }
    [[nodiscard]] static bool FaraSageti(const Caracter& c1, const Caracter& c2){return !(c1.AreSageti() || c2.AreSageti());}
public:
    GameDemo() : window(windowWidth, windowHeight, "Archer Duel", FLAG_WINDOW_RESIZABLE){
        SetExitKey(KEY_NULL);
        window.SetMinSize(400, 300);
        Caracter::InregistreazaCaracter(player);
        Caracter::InregistreazaCaracter(inamic);
    }
    void run() {
        window.SetTargetFPS(60);
        while (!window.ShouldClose()) {
            if (close_window) break;
            if (raylib::Keyboard::IsKeyPressed(KEY_ESCAPE) && stare!=GameStates::StartMenu)
                stare = GameStates::PauseMenu;
            if (window.GetWidth()!=windowWidth || window.GetHeight()!=windowHeight) {
                windowWidth = window.GetWidth();
                windowHeight = window.GetHeight();
                player.set_pozitie(0.0f, static_cast<float>(windowHeight)/2);
                inamic.set_pozitie(static_cast<float>(windowWidth)-player.get_rect().width,
    static_cast<float>(windowHeight)/2);
            }
            if (!player.InViata() || !inamic.InViata()) {
                stare=GameStates::GameOver;
            }
            float dt=window.GetFrameTime();
            window.BeginDrawing();
            window.ClearBackground(RAYWHITE);
            switch (stare) {
                case GameStates::GameOver:
                    starePrev=stare;
                    DeseneazaGameOver();
                    break;
                case GameStates::StartMenu:
                    DeseneazaStartMenu();
                    break;
                case GameStates::TuraPlayer:
                    starePrev=stare;
                    stareUrm=GameStates::TuraInamic;
                    if (FaraSageti(player, inamic)) {
                        stare=GameStates::GameOver;
                        break;
                    }
                    Logica(player, inamic, 50.0f+player.get_rect().width ,dt,
                        static_cast<float>(windowHeight));
                    DeseneazaHUD();
                    break;
                case GameStates::Intermediar:
                    starePrev=stare;
                    Logica(player, inamic,0.0f, dt, static_cast<float>(windowHeight));
                    if (TrecereTura) {
                        for (int i = static_cast<int>(ziduri.size()) - 1; i >= 0; --i) {
                            Bloc& b = ziduri[i];
                            b.Update();
                            if (b.TrebuieSters()) ziduri.erase(ziduri.begin()+i);
                        }
                        stare=stareUrm;
                        TrecereTura=false;
                    }
                    break;
                case GameStates::TuraInamic:
                    starePrev=stare;
                    stareUrm=GameStates::TuraPlayer;
                    if (FaraSageti(player, inamic)) {
                        stare=GameStates::GameOver;
                        break;
                    }
                    Logica(inamic, player,-20.0f, dt, static_cast<float>(windowHeight));
                    DeseneazaHUD();
                    break;
                case GameStates::Controale:
                    DeseneazaControale();
                    break;
                case GameStates::PauseMenu:
                    DeseneazaPauseMenu();
                    break;
                default:
                DrawText("How did we get here?", windowWidth/2, windowHeight/2, 50, BLACK);
                    break;
            }
                window.EndDrawing();
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const GameDemo& g) {
        os << "Marimea ferestrei jocului: " << g.windowWidth << "x" << g.windowHeight << std::endl;
        os << "Atributele caracterelor din joc: " << std::endl;
        os << "Player 1:\n" << g.player << std::endl;
        os << "Player 2:\n" << g.inamic << std::endl;
        os << "Stare actuala: " << StareToStr(g.stare) << std::endl;
        os << "Starea de dinainte: " << StareToStr(g.starePrev) << std::endl;
        os << "Starea care urmeaza: " << StareToStr(g.stareUrm) << std::endl;
        return os;
    }
};

std::vector<Caracter*> Caracter::caractere;

int main() {
    GameDemo game;
    std::cout << game;
    game.run();
    std::cout << game;
    return 0;
}