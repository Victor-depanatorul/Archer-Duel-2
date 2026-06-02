#ifndef OOP_SAGEATA_HPP
#define OOP_SAGEATA_HPP

#include "basic_includes.hpp"
#include <memory>
#include <string>

class Caracter;
class Entitate;

class Sageata {
protected:
    raylib::Rectangle hitbox;
    float rotation = 0.0f;
    raylib::Vector2 viteza{0.0f, 0.0f};

    Caracter* tragator = nullptr;
    bool armata = false;
    bool trebuie_distrusa = false;
    bool a_nimerit = false;

    virtual void afiseaza(std::ostream& os) const;
    virtual void SetVelocity(raylib::Vector2 tintaMouse,
                                         float forta,
                                         const Caracter* inamic);

public:
    explicit Sageata(float posX = -1, float posY = -1,
                     float width = 40, float height = 20);
    virtual ~Sageata() = default;

    [[nodiscard]] virtual std::unique_ptr<Sageata> clone() const = 0;

    virtual void aplica_efect(Caracter& tinta) const = 0;

    [[nodiscard]] virtual Color get_color() const = 0;
    [[nodiscard]] virtual std::string nume() const = 0;

    [[nodiscard]] raylib::Rectangle get_hitbox() const {
        return hitbox;
    }

    [[nodiscard]] float get_rotation() const {
        return rotation;
    }

    [[nodiscard]] Caracter * get_tragator() const {
        return tragator;
    }

    void Draw() const;
    void lanseaza(Caracter& cine, raylib::Vector2 tintaMouse, float forta,
                  const Caracter* inamic = nullptr);

    void update(float dt, const std::vector<Entitate*>& obstacole);

    [[nodiscard]] bool trebuie_stearsa() const { return trebuie_distrusa; }

    friend std::ostream& operator<<(std::ostream& os, const Sageata& s) {
        s.afiseaza(os);
        return os;
    }
};

#endif // OOP_SAGEATA_HPP
