#ifndef BLOC_HPP
#define BLOC_HPP

#include "basic_includes.hpp"
#include "entitate.hpp"

class Bloc : public Entitate{
    int8_t lifespan = 2;
    float progres_constructie = 0.0f;
    const float viteza_constructie = 3.0f;
    void OnCollision(Entitate& other) override;
public:
    Bloc() = delete;
    Bloc(float Width, float Height);
    Bloc(float posX, float posY, float width, float height);

    void Update(float dt) override;
    void _draw(raylib::Vector2) override;
    bool GetCollision(Sageata& s) override;



    [[nodiscard]] bool TrebuieSters() const;

    friend std::ostream& operator<<(std::ostream& os, const Bloc& b);
};

#endif