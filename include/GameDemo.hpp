#ifndef GAMEDEMO_HPP
#define GAMEDEMO_HPP

#include "basic_includes.hpp"

#include "caracter.hpp"

#include "PowerUp.hpp"

#include "perks.hpp"


enum class Castigator { Player1, Player2, Egalitate };

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
    [[nodiscard]] float FactorScalare() const {
        return std::min(static_cast<float>(windowWidth) / 800.0f,
                        static_cast<float>(windowHeight) / 450.0f);
    }
    static constexpr float inaltime_zid_factor = 1.8f;
    std::array<std::unique_ptr<Perk>, Perks::NrPerks> perks_ = Perk_factory::predefinite();
    void Logica(float dt);
    [[nodiscard]] static bool FaraSageti(const Caracter& c1, const Caracter& c2);


    GameDemo();

public:
    GameDemo(const GameDemo&) = delete;
    GameDemo& operator=(const GameDemo&) = delete;
    static GameDemo& get_GameDemo();
    void run();

    void SchimbaStare(GameStates s) { stare = s; }
    void RuleazaTura(float dt);
    void RuleazaIntermediar(float dt);

    void Inchide() { close_window = true; }
    void Reia() { stare = starePrev; }
    void Restart() { ResetGame(); }
    void PlayAgain() { stare = starePrev; ResetGame(); }
    void AlegeMod(GameModes mod);
    void CumparaPerk(int idx) const;

    [[nodiscard]] int latime() const { return windowWidth; }
    [[nodiscard]] int inaltime() const { return windowHeight; }
    [[nodiscard]] float ScalaMeniu() const {
        return std::min(static_cast<float>(windowWidth) / BASE_WIDTH,
                        static_cast<float>(windowHeight) / BASE_HEIGHT);
    }
    [[nodiscard]] const Caracter* jucator_curent() const { return player_crt; }
    [[nodiscard]] const Caracter* jucator1_() const { return player1.get(); }
    [[nodiscard]] const Caracter* jucator2_() const { return player2.get(); }
    [[nodiscard]] const std::array<std::unique_ptr<Perk>, Perks::NrPerks>& perks() const { return perks_; }
    [[nodiscard]] bool joc_a_inceput() const { return joc_inceput; }
    [[nodiscard]] Castigator determina_castigator() const;

    friend std::ostream& operator<<(std::ostream& os, const GameDemo& g);
};

#endif