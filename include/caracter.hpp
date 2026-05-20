#ifndef CARACTER_HPP
#define CARACTER_HPP

#include "basic_includes.hpp"
#include "arc.hpp"
#include "sageata.hpp"

class Caracter {
    float hp;
    float scale;
    const char* PathTextura;
    raylib::Texture2D textura;
    raylib::Rectangle rect;
    Arc arc;
    std::vector<Sageata> sageti_trase;
    
    static std::vector<Caracter*> caractere;
    int runde_otrava = 0;
    static constexpr float dps_otrava = 1.5f;

    static void InregistreazaCaracter(Caracter* c);
    bool Nimerit(const Sageata& s);

public:
    // Constructori
    explicit Caracter(float scale = 1.0f, float posX = 0.0f, float posY = 0.0f,
                      const char* PathTextura = "assets/textures/pacman3.png", float hp = 100);

    explicit Caracter(const Arc& arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f,
                      const char* PathTextura = "assets/textures/pacman3.png", float hp = 100);

    Caracter(const Caracter &other);
    Caracter& operator=(const Caracter &other);
    ~Caracter();

    // Getters
    [[nodiscard]] raylib::Rectangle get_rect() const;
    [[nodiscard]] float get_hp() const;
    [[nodiscard]] tipSageti TipUrmatoareaSageata() const;
    [[nodiscard]] bool InViata() const;
    [[nodiscard]] bool AreSageti() const;

    // Logica de joc
    void DeseneazaCaracter(float rotation = 0) const;
    void IaDamage(float damage);
    void AplicaOtrava(int runde);
    void UpdateEfect();
    void set_pozitie(float x, float y);
    
    void Trage(raylib::Vector2 targetPos, float forta, const Caracter* tinta = nullptr);
    void UpdateSagetiTrase(float dt, const std::vector<raylib::Rectangle>& others, float max_height);

    // Operator afișare
    friend std::ostream& operator<<(std::ostream& os, const Caracter& c);
};

#endif