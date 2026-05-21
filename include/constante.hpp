//
// Created by user on 13.05.2026.
//

#ifndef OOP_CONSTANTE_HPP
#define OOP_CONSTANTE_HPP
#include <raylib-cpp.hpp>
namespace miscare {
    // constexpr char MoveKeys[5] = "WASD";
    inline raylib::Vector2 ChangePos[4] = {{0, -10}, {-10, 0}, {0, 10}, {10, 0}};
}

inline bool TrecereTura=false;

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
    TuraPlayer1,
    Intermediar,
    TuraPlayer2,
    PauseMenu,
    GameOver
};
inline std::string StareToStr(GameStates stare) {
    switch (stare) {
        case GameStates::StartMenu: return "Meniu de Start";
        case GameStates::Controale: return "Controale";
        case GameStates::TuraPlayer1: return "Tura Player 1";
        case GameStates::TuraPlayer2: return "Tura Player 2";
        case GameStates::Intermediar: return "Intre Ture";
        case GameStates::PauseMenu: return "Pauza";
        case GameStates::GameOver: return "Game Over";
        default: return "Nedefinit";
    }
}
inline std::string GetNumeTip(tipSageti tip) {
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

constexpr std::array<float, tipSageti::NrTipuri> damage{5, 3.5, 5, -10, 10, 5};
constexpr std::array<Color, tipSageti::NrTipuri> culori{BLUE, VIOLET, BLACK, GREEN, BLUE, RED};
#endif //OOP_CONSTANTE_HPP