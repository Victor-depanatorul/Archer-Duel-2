//
// Created by user on 13.05.2026.
//

#ifndef OOP_CONSTANTE_HPP
#define OOP_CONSTANTE_HPP
#include <raylib-cpp.hpp>
#include <random>
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


enum GameStates {
    StartMenu,
    Controale,
    TuraPlayer,
    Intermediar,
    PauseMenu,
    GameOver,
    MeniuGameModes,
    MeniuStatistici,
    MeniuPerk,
    AlegeCaracter,
    Explicatii,
    NrStates
};

enum GameModes {
    Normal,
    Randomized,
    Beserker,
    NrModuri
};

enum tipCaracter {
    CaracterNormal,
    CaracterAsasin,
    CaracterTank,
    CaracterMage,
    CaracterReinforcer,
    NrCaractere
};

enum class Castigator { Player1, Player2, Egalitate };

#endif //OOP_CONSTANTE_HPP