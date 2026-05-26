//
// Created by user on 15.05.2026.
//

#ifndef OOP_ENTITATE_HPP
#define OOP_ENTITATE_HPP
#include "basic_includes.hpp"
#include "sageata.hpp"

class Entitate {
protected:
    raylib::Rectangle hitbox;
    float scale;
    static std::vector<Entitate*> entitati;
    float rotation; //in grade

    virtual void OnCollision(Entitate& other) = 0;
    static void _inregistreaza_entitate(Entitate* e);
    virtual void _draw(raylib::Vector2) = 0;
public:
    Entitate() = delete;

    Entitate(float width, float height, float posX, float posY, float scale = 1.0f, float rotation = 0.0f);

    explicit Entitate(raylib::Rectangle hitbox, float scale = 1.0f, float rotation = 0.0f);

    Entitate(const Entitate& other);

    Entitate& operator=(const Entitate& other);

    virtual ~Entitate();

    bool GetCollision(Entitate& other);

    virtual bool GetCollision(Sageata& s) = 0;

    void MoveWith(float dx, float dy);

    virtual void SetPosition(float x, float y);

    virtual void Update(float dt) = 0;

    [[nodiscard]] raylib::Rectangle GetHitbox() const;
    [[nodiscard]] float GetRotation() const;

    void Draw();
    static const std::vector<Entitate*>& get_entitati();
};


#endif //OOP_ENTITATE_HPP