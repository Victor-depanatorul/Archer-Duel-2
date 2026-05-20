//
// Created by user on 15.05.2026.
//

#ifndef OOP_ENTITATE_HPP
#define OOP_ENTITATE_HPP
#include "basic_includes.hpp"

class Entitate {
protected:
    raylib::Rectangle hitbox;
    float scale;
    static std::vector<Entitate*> entitati;
    float rotation; //in grade

    virtual void OnCollision() = 0;
    static void _inregistreaza_entitate(Entitate* e);
public:
    Entitate() = delete;

    Entitate(float width, float height, float posX, float posY, float scale = 1.0f, float rotation = 0.0f);

    explicit Entitate(raylib::Rectangle hitbox, float scale = 1.0f, float rotation = 0.0f);

    Entitate(const Entitate& other);

    Entitate& operator=(const Entitate& other);

    virtual ~Entitate();

    void GetCollision(const Entitate& other);

    void MoveWith(float dx, float dy);

    virtual void Draw() = 0;

    [[nodiscard]] raylib::Rectangle GetHitbox() const;
    [[nodiscard]] float GetRotation() const;

};


#endif //OOP_ENTITATE_HPP