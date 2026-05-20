#ifndef BLOC_HPP
#define BLOC_HPP

#include "basic_includes.hpp"

class Bloc {
    raylib::Rectangle rect;
    int8_t lifespan = 2;

public:
    explicit Bloc(float posX = 0, float posY = 0, float Width = 0, float Height = 0);

    void Update();
    void Deseneaza() const;

    [[nodiscard]] raylib::Rectangle get_rect() const;
    [[nodiscard]] bool TrebuieSters() const;

    friend std::ostream& operator<<(std::ostream& os, const Bloc& b);
};

#endif