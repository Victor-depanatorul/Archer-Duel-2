//
// Created by user on 02.06.2026.
//

#ifndef OOP_STATISTICI_HPP
#define OOP_STATISTICI_HPP
#include <ostream>
#include <raylib.h>


class Statistici {
    int sageti_trase = 0;
    float damage_dat = 0.0f;
    int powerups_luate = 0;
    int sageti_nimerite = 0;
public:
    void inregistreaza_damage(float d);
    void inregistreaza_powerup();
    void inregistreaza_nimerita();
    void inregistreaza_trasa();
    [[nodiscard]] float acuratete() const;

    void Deseneaza(int x, int y, Color culoare, std::string&& aditional = "", float scale = 1.0f) const;

    friend std::ostream& operator<<(std::ostream &os, const Statistici &obj);
};


#endif //OOP_STATISTICI_HPP