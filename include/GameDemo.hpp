#ifndef GAMEDEMO_HPP
#define GAMEDEMO_HPP

#include "basic_includes.hpp"

#include "caracter.hpp"

#include "PowerUp.hpp"

class GameDemo {
    float podea = 0.0f;
    PowerUp p_up;

    int windowWidth = 800;
    int windowHeight = 450;
    raylib::Window window;
    raylib::AudioDevice audio;

    std::unique_ptr<Caracter> player1;
    std::unique_ptr<Caracter> player2;
    Caracter* player_crt = nullptr;

    bool TrecereTura = false;
    float forta_tragere = 0.0f;
    static constexpr float latime_zid = 30.0f;
    static constexpr float distanta_zid = 65.0f;
    static constexpr float BASE_WIDTH = 1280.0f;
    static constexpr float BASE_HEIGHT = 720.0f;

    GameStates stare = GameStates::StartMenu;
    GameStates stareUrm = GameStates::TuraPlayer;
    GameStates starePrev = GameStates::TuraPlayer;
    GameModes game_modes_ = GameModes::Normal;
    bool close_window = false;
    bool joc_inceput = false;
    uint32_t nr_ture = 0;
    std::vector<std::unique_ptr<Sageata>> sageti_zbor;

    void ResetGame();
    void DeseneazaHUD() const;
    void DeseneazaStartMenu();
    void DeseneazaPauseMenu();
    void DeseneazaGameOver();
    void DeseneazaStats();
    void DeseneazaControale();
    void DeseneazaGameMode();
    void Logica(Caracter* c1, const Caracter* c2, float offset_zid, float dt);
    [[nodiscard]] static bool FaraSageti(const Caracter& c1, const Caracter& c2);

    enum class Castigator { Player1, Player2, Egalitate };
    [[nodiscard]] Castigator determina_castigator() const;

    GameDemo();

public:
    GameDemo(const GameDemo&) = delete;
    GameDemo& operator=(const GameDemo&) = delete;
    static GameDemo& get_GameDemo();
    void run();
    friend std::ostream& operator<<(std::ostream& os, const GameDemo& g);
};

#endif