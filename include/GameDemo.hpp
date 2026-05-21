#ifndef GAMEDEMO_HPP
#define GAMEDEMO_HPP

#include "basic_includes.hpp"
#include "sageata.hpp"
#include "arc.hpp"
#include "caracter.hpp"
#include "bloc.hpp"

class GameDemo {
    int windowWidth = 800;
    int windowHeight = 450;
    raylib::Window window;

    tipSageti s_normala{tipSageti::Normala};
    tipSageti s_otravitoare{tipSageti::Otravitoare};
    tipSageti s_aimbot{tipSageti::Aimbot};
    tipSageti s_giganta{tipSageti::Giganta};
    tipSageti s_lifesteal{tipSageti::LifeSteal};
    tipSageti s_healing{tipSageti::Healing};

    std::vector<tipSageti> sageti_test{s_normala, s_normala, s_normala, s_normala, s_normala, s_normala, s_giganta,
                                    s_otravitoare, s_aimbot, s_lifesteal, s_healing};
    Arc arc{sageti_test};

    Entitate* player1;
    Entitate* player2;

    static std::vector<Bloc> ziduri;

    bool trage_arc = false;
    float forta_tragere = 0.0f;
    const float max_forta_tragere = 1500.0f;
    const float viteza_trager = 800.0f;
    const float forta_de_baza = 400.0f;

    GameStates stare = GameStates::StartMenu;
    GameStates stareUrm = GameStates::TuraPlayer1;
    GameStates starePrev = GameStates::TuraPlayer1;
    int meniuSelectat = 0;
    bool close_window = false;
    bool joc_inceput = false;

    void ResetGame();
    // Modificat în const reference
    void UpdateSageti(const std::vector<raylib::Rectangle>& rectangles, float dt, float max_height) const;
    static void DeseneazaButon(raylib::Rectangle rect, const char* text, bool selectat);
    void DeseneazaHUD() const;
    void DeseneazaStartMenu();
    void DeseneazaPauseMenu();
    void DeseneazaGameOver();
    void DeseneazaControale();
    void Logica(Entitate* c1, Entitate* c2, float offset_zid, float dt, float max_height);
    [[nodiscard]] static bool FaraSageti(const Caracter& c1, const Caracter& c2);
    GameDemo();

public:
    GameDemo(const GameDemo&) = delete;
    GameDemo& operator=(const GameDemo&) = delete;
    static GameDemo& get_GameDemo();
    void run();
    friend std::ostream& operator<<(std::ostream& os, const GameDemo& g);
};

#endif