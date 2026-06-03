//
// Created by user on 13.05.2026.
//

#ifndef OOP_CONSTANTE_HPP
#define OOP_CONSTANTE_HPP
#include <raylib-cpp.hpp>
namespace miscare {
    inline raylib::Vector2 ChangePos[4] = {{0, -10}, {-10, 0}, {0, 10}, {10, 0}};
}

template <typename T>
inline T MyRand(T min, T max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    if constexpr (std::is_integral_v<T>)
        return std::uniform_int_distribution<>{min, max}(gen);
    else if (std::is_floating_point_v<T>)
        return std::uniform_real_distribution<T>{min, max}(gen);
    else
        throw std::invalid_argument{"Invalid type"};
}
enum tipSageti {
    Normala,
    Otravitoare,
    Aimbot,
    Healing,
    Giganta,
    LifeSteal,
    Burn,
    GlassCannon,
    Random,
    NrTipuri,
    Invalid
};

enum GameStates {
    StartMenu,
    Controale,
    TuraPlayer,
    Intermediar,
    PauseMenu,
    GameOver,
    MeniuGameModes,
    MeniuStatistici
};

enum GameModes {
    Normal,
    Randomized,
    Beserker
};

enum tipPowerUp {
    ARROW,
    MULTI_SHOT,
    DOUBLE_MOVE,
    NrPowerUps
};
constexpr std::array<Color, tipSageti::NrTipuri> culori{BLUE, VIOLET, BLACK, GREEN, DARKBLUE, RED, ORANGE};
#endif //OOP_CONSTANTE_HPP