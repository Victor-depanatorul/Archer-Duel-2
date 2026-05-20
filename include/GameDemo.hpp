#ifndef GAMEDEMO_HPP
#define GAMEDEMO_HPP

#include "basic_includes.hpp"
#include "sageata.hpp"
#include "arc.hpp"
#include "caracter.hpp"
#include "Bloc.hpp"

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

    Caracter player{arc, 0.1f, 0.0f, static_cast<float>(windowHeight)/2.0f};
    Caracter inamic{arc, 0.1f, static_cast<float>(windowWidth)-player.get_rect().width,
                    static_cast<float>(windowHeight)/2.0f, "assets/textures/pacman_intors.png", 1.0f};

    static std::vector<Bloc> ziduri;

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
    bool joc_inceput = false;

    void ResetGame();
    // Modificat în const reference
    static void UpdateSageti(Caracter& p, Caracter& i, const std::vector<raylib::Rectangle>& rectangles, float dt, float max_height);
    static void DeseneazaButon(raylib::Rectangle rect, const char* text, bool selectat);
    void DeseneazaHUD() const;
    void DeseneazaStartMenu();
    void DeseneazaPauseMenu();
    void DeseneazaGameOver();
    void DeseneazaControale();
    void Logica(Caracter& c1, Caracter& c2, float offset_zid, float dt, float max_height);
    [[nodiscard]] static bool FaraSageti(const Caracter& c1, const Caracter& c2);

public:
    GameDemo();
    void run();
    friend std::ostream& operator<<(std::ostream& os, const GameDemo& g);
};

#endif