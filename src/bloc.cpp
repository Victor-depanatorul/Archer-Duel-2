#include "bloc.hpp"

Bloc::Bloc(float posX, float posY, float Width, float Height)
    : rect(posX, posY, Width, Height) {}

void Bloc::Update() {
    lifespan--;
}

void Bloc::Deseneaza() const {
    rect.Draw(raylib::Color(120, 120, 120, 200));
    rect.DrawLines(DARKGRAY);
}

raylib::Rectangle Bloc::get_rect() const {
    return rect;
}

bool Bloc::TrebuieSters() const {
    return lifespan <= 0;
}

std::ostream& operator<<(std::ostream& os, const Bloc& b) {
    return os << "Pozitie: " << '(' << b.rect.x << ", " << b.rect.y << ")\n" <<
                 "Marimi: " << '(' << b.rect.width << ", " << b.rect.height << ")" << std::endl;
}