#ifndef GAMEDEMO_HPP
#define GAMEDEMO_HPP

#include "basic_includes.hpp"

#include "caracter.hpp"

#include "PowerUp.hpp"

#include "perks.hpp"


//Singleton pentru joc
class Game {
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
    static constexpr float BASE_CHR_SCALE = 0.05f;

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
    void AdapteazaLaFereastra() const;
    [[nodiscard]] float FactorScalare() const;
    static constexpr float inaltime_zid_factor = 1.8f;
    std::array<std::unique_ptr<Perk>, Perks::NrPerks> perks_ = Perk_factory::predefinite();
    void Logica(float dt);
    [[nodiscard]] static bool FaraSageti(const Caracter& c1, const Caracter& c2);


    Game();

public:
    // Design pattern: Singleton (Meyers). Copy/assign sterse, instanta unica
    // returnata de get_GameInstance.
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    static Game& get_GameInstance();
    void run();

    void SchimbaStare(GameStates s);
    void RuleazaTura(float dt);
    void RuleazaIntermediar(float dt);

    void Inchide();
    void Reia();
    void Restart();
    void PlayAgain();
    void AlegeMod(GameModes mod);
    void IncepeMeci(tipCaracter t1, tipCaracter t2);
    void CumparaPerk(int idx) const;

    [[nodiscard]] int latime() const;
    [[nodiscard]] int inaltime() const;
    [[nodiscard]] float ScalaMeniu() const;
    [[nodiscard]] const Caracter* jucator_curent() const;
    [[nodiscard]] const Caracter* jucator1_() const;
    [[nodiscard]] const Caracter* jucator2_() const;
    [[nodiscard]] const std::array<std::unique_ptr<Perk>, Perks::NrPerks>& perks() const;
    [[nodiscard]] bool joc_a_inceput() const;
    [[nodiscard]] Castigator determina_castigator() const;

    friend std::ostream& operator<<(std::ostream& os, const Game& g);
};

#endif